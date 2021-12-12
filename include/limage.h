#ifndef __LIMAGE_H__
#define __LIMAGE_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "lmath.h"

#define BI_BM 0x4d42    // ASCII code for 'BM'
#define BI_FILE_HEADER_SIZE 14L

#define BI_RGB 0    // no compression
#define BI_RLE8 1   // 8bit RLE encoding
#define BI_RLE4 2   // 4bit RLE encoding

// x86_64
typedef unsigned char  image_t;  // 1 bytes
typedef unsigned short ushort_t; // 2 bytes
typedef unsigned int   ulong_t;  // 4 bytes
typedef int            long_t;   // 4 bytes

// TODO:
// - handle x86 structure
// - split .h and .cpp

namespace Lurdr {

// BMP File Format
// reference : http://www.ece.ualberta.ca/~elliott/ee552/studentAppNotes/2003_w/misc/bmp_file_format/bmp_file_format.htm
// also based on my previous course assignment in DIP (digital image processing, ZJU, lectured by M. Song)
typedef struct BITMAPFILEHEADER {
	ushort_t  signature;
	ulong_t   file_size;
	ulong_t   reserved;
	ulong_t   data_offset;
} BMPFileHeader;

typedef struct BITMAPINFOHEADER {
	ulong_t         size;       // info header size
	long_t          width;
 	long_t          height;     // negative height indicate upside-down data buffer
 	ushort_t        planes;
	ushort_t        bits_per_pixel;
                                // 1 = monochrome palette.  NumColors = 1  
                                // 4 = 4bit palletized.     NumColors = 16  
                                // 8 = 8bit palletized.     NumColors = 256 
                                // 16 = 16bit RGB.          NumColors = 65536
                                // 24 = 24bit RGB.          NumColors = 16M
	ulong_t         compression;
	ulong_t         image_size; // compressed size of image, 0 if compression == 0
 	long_t          X_pixels_per_M;
	long_t          Y_pixels_per_M;
	ulong_t         colors_used;
	ulong_t         important_colors;
} BMPInfoHeader;

// used when bits_per_pixel <= 8
typedef struct BITMAPCOLORTABLE {
    image_t   red;
    image_t   green;
    image_t   blue;
    image_t   reserved;
} BMPColorTable;

class BMPImage {
private:
    BMPFileHeader m_file_header;
    BMPInfoHeader m_info_header;
    BMPColorTable *m_color_tables;
    image_t *m_buffer;
    bool          m_is_loaded;
    bool          m_use_color_table;
    char          *m_filename;

    void loadFileHeaderx64(FILE *fp) {
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

    void writeFileHeaderx64(FILE *fp) {
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

    void loadImage() {
        FILE *fp;
        assert(m_filename != nullptr);
        fp = fopen(m_filename, "rb");
        if (fp == nullptr) {
            printf("BMP image file: %s open failed\n", m_filename);
            return;
        }
        loadFileHeaderx64(fp);
        fseek(fp, BI_FILE_HEADER_SIZE, SEEK_SET);
        fread(&m_info_header, sizeof(BMPInfoHeader), 1, fp);
        if (m_file_header.signature != BI_BM) {
            printf("BMP file format error\n");
            clean();
            fclose(fp);
            return;
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
        size_t height = m_info_header.height;
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

    void clean() {
        delete[] m_color_tables;
        delete[] m_buffer;
        delete[] m_filename;
        m_color_tables = nullptr;
        m_buffer = nullptr;
        m_is_loaded = false;
        m_filename = nullptr;
    }

    size_t getDataLine() const {
        ushort_t bits = m_info_header.bits_per_pixel;
        size_t width = m_info_header.width;
        size_t line = -1;
        switch (bits) {
            case 1:
                line = (width + 7) / 8;
                break;
            case 4:
                line = (width + 1) / 2;
                break;
            case 8:
                line = width;
                break;
            case 24:
                line = width * 3;
                break;
            case 32:
                line = width * 4;
                break;
            default:
                line = -1;
                break;
        }
        return line;
    }
public:
    BMPImage(): m_color_tables(nullptr),
                m_buffer(nullptr),
                m_filename(nullptr),
                m_is_loaded(false),
                m_use_color_table(false) {}

    BMPImage(const char* filename): m_color_tables(nullptr),
                                    m_buffer(nullptr),
                                    m_filename(nullptr),
                                    m_is_loaded(false),
                                    m_use_color_table(false) {
        size_t f_len = strlen(filename);
        m_filename = new char[f_len + 1];
        strcpy(m_filename, filename);

        loadImage();
    }

    BMPImage(const BMPImage & image): m_color_tables(nullptr),
                                      m_buffer(nullptr),
                                      m_filename(nullptr),
                                      m_is_loaded(false),
                                      m_use_color_table(false) {
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

        m_is_loaded = true;
    }

    ~BMPImage() {
        clean();
    }

    ulong_t getBufferSize() const {
        return m_info_header.size;
    }
    long_t getImageWidth() const {
        return m_info_header.width;
    }
    long_t getImageHeight() const {
        return m_info_header.height;
    }
    bool isLoaded() const {
        return m_is_loaded;
    }

    void writeImage(const char* filename) {
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
        size_t height = m_info_header.height;
        size_t data_line = getDataLine();
	    size_t scan_line = ((data_line + 3) / 4) * 4;
        size_t pad_size = scan_line - data_line;

        for (size_t i = 0; i < height; i++) {
            fwrite(m_buffer + i * data_line, data_line, 1, fp);
            fseek(fp, pad_size, SEEK_CUR);
        }


        fclose(fp);
    }

    void printImageInfo() const {
        if (m_is_loaded) {
            printf("-- BMPImage info -----------------------------\n");
            printf("      filename : %s\n", m_filename);
            printf("    image size : %-6d * %-6d\n", getImageHeight(), getImageWidth());
            printf("     data size : %-8lu bytes\n", getImageHeight() * getDataLine());
            printf("    pixel size : %-8d bits\n", m_info_header.bits_per_pixel);
            printf("----------------------------------------------\n");
        } else {
            printf("BMPImage unloaded ----------------------------\n");
        }
    }
};

}

#endif