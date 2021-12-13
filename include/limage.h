#ifndef __LIMAGE_H__
#define __LIMAGE_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <utility>
#include "lmath.h"
#include "lglobal.h"

#define BI_BM 0x4d42    // ASCII code for 'BM'
#define BI_FILE_HEADER_SIZE 14L

#define BI_RGB 0    // no compression
#define BI_RLE8 1   // 8bit RLE encoding
#define BI_RLE4 2   // 4bit RLE encoding

namespace Lurdr {

// BMP File Format
// reference : http://www.ece.ualberta.ca/~elliott/ee552/studentAppNotes/2003_w/misc/bmp_file_format/bmp_file_format.htm
// also based on my previous course assignment in DIP (digital image processing, ZJU, lectured by M. Song)

// BitMapFileHeader :
// in x86,    sizeof(BITMAPFILEHEADER) = 14, use fread to load this struct (not implemented yet)
// in x86_64, sizeof(BITMAPFILEHEADER) = 16, use loadFileHeaderx64() to load this struct
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
    pixel_t   red;
    pixel_t   green;
    pixel_t   blue;
    pixel_t   reserved;
} BMPColorTable;

class BMPImage {
private:
    BMPFileHeader m_file_header;
    BMPInfoHeader m_info_header;
    BMPColorTable *m_color_tables;
    pixel_t *m_buffer;              // color is arranged in RGB order
                                    // rows are arranged from bottom to top
    bool          m_is_loaded;
    bool          m_use_color_table;
    char          *m_filename;

    void loadFileHeaderx64(FILE *fp);
    void writeFileHeaderx64(FILE *fp) const;
    void loadImage();
    void clean();
    size_t getDataLine() const;

public:
    BMPImage(): m_color_tables(nullptr),
                m_buffer(nullptr),
                m_filename(nullptr),
                m_is_loaded(false),
                m_use_color_table(false) {}
    BMPImage(const char* filename);
    BMPImage(const BMPImage & image);
    ~BMPImage() { clean(); }

    ulong_t getBufferSize()  const { return m_info_header.size; }
    long_t  getImageWidth()  const { return m_info_header.width; }
    long_t  getImageHeight() const { return m_info_header.height; }
    bool    isLoaded()       const { return m_is_loaded; }
    size_t  getChannelNum()  const;

    /* Operator Overload Functions */
    unsigned char& operator() (const size_t & row, const size_t & column, const size_t & channel);

    /* Member Functions */
    void writeImage(const char* filename) const;
    void printImageInfo() const;
    pixel_t* & getImageBuffer();
    pixel_t* getImageBufferConst() const;
};

#define COLOR_RGB 0U
#define COLOR_BGR 1U
#define COLOR_YUV 2U
#define COLOR_HSV 3U

class UniformImage {
private:
    size_t          m_width;
    size_t          m_height;
    pixel_t         *m_buffer;     // buffer rows are arranged from top to bottom
    unsigned short  m_color_space; // RGB, BGR, YUV, HSV

    void swap(pixel_t & t1, pixel_t & t2);

    void RGB2BGR();
    void RGB2BGR_SIMD();
    void BGR2RGB();
    void BGR2RGB_SIMD();
    void RGB2YUV();
    void YUV2RGB();
    void YUV2BGR();
    void BGR2YUV();
    void RGB2HSV();
    void HSV2RGB();
    void BGR2HSV();
    void HSV2BGR();
    void YUV2HSV();
    void HSV2YUV();
public:
    UniformImage(): m_width(0),
                    m_height(0),
                    m_buffer(nullptr),
                    m_color_space(COLOR_RGB) {}
    UniformImage(size_t width, size_t height);
    UniformImage(const BMPImage & bmp);
    ~UniformImage();

    pixel_t& operator() (const size_t & row, const size_t & column, const size_t & channel);

    void createFromBMPImage(const BMPImage & bmp);
    void convertColorSpace(const unsigned short mode);
    pixel_t* & getImageBuffer();
    pixel_t* getImageBufferConst() const;
};

}

#endif