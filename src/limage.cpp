#include "../include/limage.h"

using namespace Lurdr;

/**
 * TODOs :
 *  - handle x86 structure
 *  - limit the use of platte image
 */

void BMPImage::loadFileHeaderx64(FILE *fp) {
    unsigned char *temp_buffer = new unsigned char[BI_FILE_HEADER_SIZE];
    unsigned char *ptr = temp_buffer;
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
void BMPImage::writeFileHeaderx64(FILE *fp) const {
    unsigned char *temp_buffer = new unsigned char[BI_FILE_HEADER_SIZE];
    unsigned char *ptr = temp_buffer;
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

void BMPImage::loadImage() {
    FILE *fp;
    assert(m_filename != nullptr);
    fp = fopen(m_filename, "rb");
    if (fp == nullptr) {
        printf("BMP image file: %s open failed\n", m_filename);
        return;
    }
    loadFileHeaderx64(fp);
    if (m_file_header.signature != BI_BM) {
        printf("BMP file format error\n");
        clean();
        fclose(fp);
        return;
    }
    fseek(fp, BI_FILE_HEADER_SIZE, SEEK_SET);
    fread(&m_info_header, sizeof(BMPInfoHeader), 1, fp);
    if (m_info_header.height < 0) {
        m_info_header.height = abs(m_info_header.height);
        m_is_upside_down = false;
    }

    if (m_info_header.bits_per_pixel <= 8) {
        m_use_color_table = true;
        size_t color_tables_offset = BI_FILE_HEADER_SIZE + sizeof(BMPInfoHeader);
        fseek(fp, color_tables_offset, SEEK_SET);

        size_t color_num = m_info_header.colors_used;
        m_color_tables = new BMPColorTable[color_num];
        fread(m_color_tables, sizeof(BMPColorTable), color_num, fp);
    } else {
        m_use_color_table = false;
    }

    size_t offset = m_file_header.data_offset;
    fseek(fp, offset, SEEK_SET);
    size_t height = getImageHeight();
    size_t data_line = getDataLine();

    if (data_line == -1) {
        printf("BMP unsupported bits per pixel\n");
        clean();
        fclose(fp);
        return;
    }
    size_t scan_line = ((data_line + 3) / 4) * 4;

    m_buffer = new image_t[data_line * height];
    image_t *temp_buffer = new image_t[scan_line];
    for (size_t i = 0; i < height; i++) {
        fread(temp_buffer, sizeof(image_t), scan_line, fp);
        memcpy(m_buffer + i * data_line, temp_buffer, data_line);
    }
    delete[] temp_buffer;

    m_is_loaded = true;
    fclose(fp);
}
void BMPImage::clean() {
    delete[] m_color_tables;
    delete[] m_buffer;
    delete[] m_filename;
    m_color_tables = nullptr;
    m_buffer = nullptr;
    m_is_loaded = false;
    m_is_upside_down = true;
    m_filename = nullptr;
}
size_t BMPImage::getDataLine() const {
    ushort_t bits = m_info_header.bits_per_pixel;
    size_t width = m_info_header.width;
    size_t channel_num = getChannelNum();
    if (channel_num < 0) {
        return (width + (-channel_num - 1)) / (-channel_num);
    } else if (channel_num > 0) {
        return width * channel_num;
    } else {
        return -1;
    }
}
BMPImage::BMPImage(const char* filename): m_color_tables(nullptr),
                                          m_buffer(nullptr),
                                          m_filename(nullptr),
                                          m_is_loaded(false),
                                          m_use_color_table(false),
                                          m_is_upside_down(true) {
    size_t f_len = strlen(filename);
    m_filename = new char[f_len + 1];
    strcpy(m_filename, filename);

    loadImage();
}
BMPImage::BMPImage(const BMPImage & image): m_color_tables(nullptr),
                                            m_buffer(nullptr),
                                            m_filename(nullptr),
                                            m_is_loaded(false),
                                            m_use_color_table(false),
                                            m_is_upside_down(true) {
    if (image.m_is_loaded == false) {
        clean();
        return;
    }
    m_file_header = image.m_file_header;
    m_info_header = image.m_info_header;

    m_use_color_table = image.m_use_color_table;
    if (m_use_color_table) {
        size_t color_num = m_info_header.colors_used;
        m_color_tables = new BMPColorTable[color_num];
        memcpy(m_color_tables, image.m_color_tables, color_num * sizeof(BMPColorTable));
    }

    size_t buffer_size = image.getBufferSize();
    m_buffer = new image_t[buffer_size];
    memcpy(m_buffer, image.m_buffer, buffer_size * sizeof(image_t));

    size_t f_len = strlen(image.m_filename);
    m_filename = new char[f_len + 1];
    strcpy(m_filename, image.m_filename);
    
    m_is_upside_down = image.m_is_upside_down;
    m_is_loaded = true;
}
size_t BMPImage::getChannelNum() const {
    ushort_t bits = m_info_header.bits_per_pixel;
    switch (bits) {
        case 1:  return -8;
        case 4:  return -2;
        case 8:  return 1;
        case 24: return 3;
        case 32: return 4;
        default: 
            printf("BMP image unloaded or unsupported channel num");
            return 0;
    }
}
unsigned char& BMPImage::operator() (const size_t & row, const size_t & column, const size_t & channel) {
    size_t channel_num = getChannelNum();
    size_t data_line = getDataLine();
    assert(channel_num > 0);
    assert(channel < channel_num);
    assert(row < getImageHeight());
    assert(column < getImageWidth());
    if (m_is_upside_down) {
        return m_buffer[(getImageHeight() - row - 1) * data_line + column * channel_num + channel];
    } else {
        return m_buffer[row * data_line + column * channel_num + channel];
    }
}

void BMPImage::writeImage(const char* filename) const {
    if (m_is_loaded == false) {
        printf("BMP image unloaded\n");
        return;
    }
    FILE *fp;
    fp = fopen(filename, "wb");
    if (fp == nullptr) {
        printf("BMP image file: %s open failed\n", filename);
        return;
    }
    writeFileHeaderx64(fp);
    fseek(fp, BI_FILE_HEADER_SIZE, SEEK_SET);
    fwrite(&m_info_header, sizeof(BMPInfoHeader), 1, fp);
    if (m_use_color_table) {
        fwrite(&m_color_tables, 1, sizeof(m_color_tables), fp);
    }

    size_t offset = m_file_header.data_offset;
    fseek(fp, offset, SEEK_SET);
    size_t height = getImageHeight();
    size_t data_line = getDataLine();
    size_t scan_line = ((data_line + 3) / 4) * 4;
    size_t pad_size = scan_line - data_line;

    for (size_t i = 0; i < height; i++) {
        fwrite(m_buffer + i * data_line, data_line, 1, fp);
        fseek(fp, pad_size, SEEK_CUR);
    }

    fclose(fp);
}
void BMPImage::printImageInfo() const {
    if (m_is_loaded) {
        printf("-- BMPImage info -----------------------------\n");
        printf("      filename : %s\n", m_filename);
        printf("    image size : %-6d * %-6d\n", getImageHeight(), getImageWidth());
        printf("     data size : %-8lu bytes\n", getImageHeight() * getDataLine());
        printf("    pixel size : %-8d bits\n",   m_info_header.bits_per_pixel);
        printf("----------------------------------------------\n");
    } else {
        printf("BMPImage unloaded ----------------------------\n");
    }
}