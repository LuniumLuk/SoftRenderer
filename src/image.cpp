#include "../include/image.h"

using namespace Lurdr;

/**
 * TODOs :
 *  - handle x86 structure
 *  - limit the use of platte image
 */

void BMPImage::loadFileHeaderx64(FILE *fp)
{
    byte_t *temp_buffer = new byte_t[BI_FILE_HEADER_SIZE];
    byte_t *ptr = temp_buffer;

    fseek(fp, 0L, SEEK_SET);
    fread(temp_buffer, BI_FILE_HEADER_SIZE, 1, fp);
    memcpy(&(m_file_header.signature), ptr, sizeof(ushort_t));
    ptr += sizeof(ushort_t);
    memcpy(&(m_file_header.file_size), ptr, sizeof(ulong_t));
    ptr += sizeof(ulong_t);
    memcpy(&(m_file_header.reserved), ptr, sizeof(ulong_t));
    ptr += sizeof(ulong_t);
    memcpy(&(m_file_header.data_offset), ptr, sizeof(ulong_t));

    delete[] temp_buffer;
}
void BMPImage::writeFileHeaderx64(FILE *fp) const
{
    byte_t *temp_buffer = new byte_t[BI_FILE_HEADER_SIZE];
    byte_t *ptr = temp_buffer;

    memcpy(ptr, &(m_file_header.signature), sizeof(ushort_t));
    ptr += sizeof(ushort_t);
    memcpy(ptr, &(m_file_header.file_size), sizeof(ushort_t));
    ptr += sizeof(ulong_t);
    memcpy(ptr, &(m_file_header.reserved), sizeof(ushort_t));
    ptr += sizeof(ulong_t);
    memcpy(ptr, &(m_file_header.data_offset), sizeof(ushort_t));

    fseek(fp, 0L, SEEK_SET);
    fwrite(temp_buffer, BI_FILE_HEADER_SIZE, 1, fp);

    delete[] temp_buffer;
}

void BMPImage::loadImage()
{
    FILE *fp;
    assert(m_filename != nullptr);
    fp = fopen(m_filename, "rb");
    if (fp == nullptr)
    {
        printf("BMPImage : image file: %s open failed\n", m_filename);
        return;
    }
    loadFileHeaderx64(fp);
    if (m_file_header.signature != BI_BM)
    {
        printf("BMPImage : file format error\n");
        clean();
        fclose(fp);
        return;
    }
    fseek(fp, BI_FILE_HEADER_SIZE, SEEK_SET);
    fread(&m_info_header, sizeof(BMPInfoHeader), 1, fp);
    if (m_info_header.height < 0)
    {
        m_info_header.height = abs(m_info_header.height);
    }

    if (m_info_header.bits_per_pixel <= 8)
    {
        m_use_color_table = true;
        size_t color_tables_offset = BI_FILE_HEADER_SIZE + sizeof(BMPInfoHeader);
        fseek(fp, color_tables_offset, SEEK_SET);

        size_t color_num = m_info_header.colors_used;
        m_color_tables = new BMPColorTable[color_num];
        fread(m_color_tables, sizeof(BMPColorTable), color_num, fp);
    }
    else
    {
        m_use_color_table = false;
    }

    size_t offset = m_file_header.data_offset;
    fseek(fp, offset, SEEK_SET);
    size_t height = getImageHeight();
    size_t data_line = getDataLine();

    if (data_line == -1)
    {
        printf("BMPImage : unsupported bits per pixel\n");
        clean();
        fclose(fp);
        return;
    }
    size_t scan_line = ((data_line + 3) / 4) * 4;

    m_buffer = new byte_t[data_line * height];
    byte_t *temp_buffer = new byte_t[scan_line];
    for (size_t i = 0; i < height; i++)
    {
        fread(temp_buffer, sizeof(byte_t), scan_line, fp);
        memcpy(m_buffer + i * data_line, temp_buffer, data_line);
    }
    
    delete[] temp_buffer;

    m_is_loaded = true;
    fclose(fp);
}
void BMPImage::clean() 
{
    delete[] m_color_tables;
    delete[] m_buffer;
    delete[] m_filename;
    m_color_tables = nullptr;
    m_buffer = nullptr;
    m_is_loaded = false;
    m_filename = nullptr;
}
size_t BMPImage::getDataLine() const
{
    ushort_t bits = m_info_header.bits_per_pixel;
    size_t width = m_info_header.width;
    size_t channel_num = getChannelNum();
    if (channel_num < 0)
    {
        return (width + (-channel_num - 1)) / (-channel_num);
    } 
    else if (channel_num > 0)
    {
        return width * channel_num;
    }
    else
    {
        return -1;
    }
}
BMPImage::BMPImage(const char* filename): m_color_tables(nullptr),
                                          m_buffer(nullptr),
                                          m_filename(nullptr),
                                          m_is_loaded(false),
                                          m_use_color_table(false)
{
    size_t f_len = strlen(filename);
    m_filename = new char[f_len + 1];
    strcpy(m_filename, filename);

    loadImage();
}
BMPImage::BMPImage(const BMPImage & image): m_color_tables(nullptr),
                                            m_buffer(nullptr),
                                            m_filename(nullptr),
                                            m_is_loaded(false),
                                            m_use_color_table(false)
{
    if (image.m_is_loaded == false)
    {
        clean();
        return;
    }
    m_file_header = image.m_file_header;
    m_info_header = image.m_info_header;

    m_use_color_table = image.m_use_color_table;
    if (m_use_color_table)
    {
        size_t color_num = m_info_header.colors_used;
        m_color_tables = new BMPColorTable[color_num];
        memcpy(m_color_tables, image.m_color_tables, color_num * sizeof(BMPColorTable));
    }

    size_t buffer_size = image.getBufferSize();
    m_buffer = new byte_t[buffer_size];
    memcpy(m_buffer, image.m_buffer, buffer_size * sizeof(byte_t));

    size_t f_len = strlen(image.m_filename);
    m_filename = new char[f_len + 1];
    strcpy(m_filename, image.m_filename);
    
    m_is_loaded = true;
}
size_t BMPImage::getChannelNum() const
{
    ushort_t bits = m_info_header.bits_per_pixel;
    switch (bits)
    {
        case 1:  return -8;
        case 4:  return -2;
        case 8:  return 1;
        case 24: return 3;
        case 32: return 4;
        default: 
            printf("BMPImage : image unloaded or unsupported channel num");
            return 0;
    }
}
byte_t& BMPImage::operator() (const size_t & row, const size_t & column, const size_t & channel)
{
    size_t channel_num = getChannelNum();
    size_t data_line = getDataLine();
    assert(channel_num > 0);
    assert(channel < channel_num);
    assert(row < getImageHeight());
    assert(column < getImageWidth());
    return m_buffer[row * data_line + column * channel_num + channel];
}

void BMPImage::writeImage(const char* filename) const
{
    if (m_is_loaded == false)
    {
        printf("BMPImage : image unloaded\n");
        return;
    }
    FILE *fp;
    fp = fopen(filename, "wb");
    if (fp == nullptr)
    {
        printf("BMPImage : image file: %s open failed\n", filename);
        return;
    }
    writeFileHeaderx64(fp);
    fseek(fp, BI_FILE_HEADER_SIZE, SEEK_SET);
    fwrite(&m_info_header, sizeof(BMPInfoHeader), 1, fp);
    if (m_use_color_table)
    {
        fwrite(&m_color_tables, 1, sizeof(m_color_tables), fp);
    }

    size_t offset = m_file_header.data_offset;
    fseek(fp, offset, SEEK_SET);
    size_t height = getImageHeight();
    size_t data_line = getDataLine();
    size_t scan_line = ((data_line + 3) / 4) * 4;
    size_t pad_size = scan_line - data_line;

    for (size_t i = 0; i < height; i++)
    {
        fwrite(m_buffer + i * data_line, data_line, 1, fp);
        fseek(fp, pad_size, SEEK_CUR);
    }

    fclose(fp);
}
byte_t* & BMPImage::getImageBuffer()
{
    return m_buffer;
}
byte_t* BMPImage::getImageBufferConst() const
{
    return m_buffer;
}
void BMPImage::printImageInfo() const
{
    if (m_is_loaded)
    {
        printf("-- BMPImage info -----------------------------\n");
        printf("      filename : %s\n", m_filename);
        printf("    image size : %-6d * %-6d\n", getImageHeight(), getImageWidth());
        printf("     data size : %-8lu bytes\n", getImageHeight() * getDataLine());
        printf("    pixel size : %-8d bits\n",   m_info_header.bits_per_pixel);
        printf("----------------------------------------------\n");
    }
    else
    {
        printf("-- BMPImage unloaded -------------------------\n");
    }
}


UniformImage::UniformImage(size_t width, size_t height): m_width(width),
                                                         m_height(height),
                                                         m_buffer(nullptr),
                                                         m_color_space(COLOR_RGB) {}
UniformImage::UniformImage(const BMPImage & bmp): m_width(0),
                                                  m_height(0),
                                                  m_buffer(nullptr),
                                                  m_color_space(COLOR_RGB)
{
    createFromBMPImage(bmp);
}
UniformImage::~UniformImage()
{
    delete[] m_buffer;
}

// implemented base on std::swap
// reference : https://stackoverflow.com/questions/35154516/most-efficient-way-of-swapping-values-c
void UniformImage::swap(byte_t & t1, byte_t & t2)
{
    byte_t temp(std::move(t1));
    t1 = std::move(t2);
    t2 = std::move(temp);
}
// brutal froce method (abandoned)
void UniformImage::RGB2BGR()
{
    size_t row_size = m_width * 3;
    for (size_t i = 0; i < m_height; i++)
    {
        for (size_t j = 0; j < m_width * 3; j += 3)
        {
            // swap the 'R' and 'B' channel
            swap(m_buffer[i * row_size + j], m_buffer[i * row_size + j + 2]);
        }
    }
}
// SIMD method to convert RGB to BGR
// __uint128_t may not supported in some system, need to switch to other sturcture or type
void UniformImage::RGB2BGR_SIMD() 
{
    // a more readable way to initialize a uint128
    simd_128_t mask = { .c = { 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00 }};
    size_t row_size = m_width * 3;
    size_t i, j;
    __uint128_t s0, sr, sg, sb;

    byte_t *buffer_ptr = m_buffer;
    byte_t *buffer_end = m_buffer + m_width * m_height * 3 - 15;
    while (buffer_ptr < buffer_end)
    {
        memcpy(&s0, buffer_ptr, 15);
        sr = mask.i & s0;
        sg = mask.i & (s0 >> 8);
        sb = mask.i & (s0 >> 16);
        s0 = std::move(sb); // faster than memcpy
        s0 |= (sr << 16);
        s0 |= (sg << 8);
        memcpy(buffer_ptr, &s0, 15);
        buffer_ptr += 15;
    }

    size_t rest = buffer_end + 15 - buffer_ptr;
    if (rest > 0)
    {
        memcpy(&s0, buffer_ptr, rest);
        sr = mask.i & s0;
        sg = mask.i & (s0 >> 8);
        sb = mask.i & (s0 >> 16);
        s0 = std::move(sb);
        s0 |= (sr << 16);
        s0 |= (sg << 8);
        memcpy(buffer_ptr, &s0, rest);
    }
}
void UniformImage::BGR2RGB()
{
    RGB2BGR();
}
void UniformImage::BGR2RGB_SIMD()
{
    RGB2BGR_SIMD();
}

byte_t& UniformImage::operator() (const size_t & row, const size_t & column, const size_t & channel)
{
    size_t row_size = m_width * 3;
    assert(channel < 3);
    assert(row < m_height);
    assert(column < m_width);
    return m_buffer[row * row_size + column * 3 + channel];
}

void UniformImage::createFromBMPImage(const BMPImage & bmp)
{
    if (!bmp.isLoaded())
    {
        printf("UniformImage : BMP image is unloaded\n");
        return;
    }
    if (bmp.getChannelNum() != 3)
    {
        printf("UniformImage : BMP channel number unsupported\n");
        return;
    }
    m_width = bmp.getImageWidth();
    m_height = bmp.getImageHeight();
    m_color_space = COLOR_BGR;

    size_t row_size = m_width * 3;
    size_t buffer_size = row_size * m_height;

    byte_t *src_buffer = bmp.getImageBufferConst();
    m_buffer = new byte_t[buffer_size];
    for (size_t i = 0; i < m_height; i++)
    {
        memcpy(m_buffer + i * row_size, src_buffer + (m_height - i - 1) * row_size, row_size);
    }
}

void UniformImage::convertColorSpace(const unsigned short mode)
{
    assert(mode >= 0 && mode < 4);
    if (m_buffer == nullptr)
    {
        printf("UniformImage : empty buffer");
        return;
    }
    if (mode == m_color_space)
    {
        return;
    }
    
    if (mode == COLOR_RGB && m_color_space == COLOR_BGR)
    { 
        RGB2BGR_SIMD();
        m_color_space = COLOR_BGR;
    }
    if (mode == COLOR_BGR && m_color_space == COLOR_RGB)
    { 
        BGR2RGB_SIMD();
        m_color_space = COLOR_RGB;
    }
    // ...
}

byte_t* & UniformImage::getImageBuffer()
{
    return m_buffer;
}
byte_t* UniformImage::getImageBufferConst() const
{
    return m_buffer;
}
// determin the endianness of the system
// reference : https://stackoverflow.com/questions/1001307/detecting-endianness-programmatically-in-a-c-program
bool UniformImage::isBigEndian()
{
    union 
    {
        uint32_t      i;
        byte_t c[4];
    } bint = { 0x01020304 };
    return bint.c[0] == 1; 
}
void UniformImage::printImageInfo() const
{
    if (m_width > 0 && m_height > 0)
    {
        printf("-- UniformImage info -------------------------\n");
        printf("    image size : %-6lu * %-6lu\n", m_width, m_height);
        if (m_color_space == COLOR_RGB)
            printf("   color space : RGB\n");
        if (m_color_space == COLOR_BGR)
            printf("   color space : BGR\n");
        if (m_color_space == COLOR_YUV)
            printf("   color space : YUV\n");
        if (m_color_space == COLOR_HSV)
            printf("   color space : HSV\n");
        printf("----------------------------------------------\n");
    }
    else
    {
        printf("-- BMPImage unloaded -------------------------\n");
    }
}
