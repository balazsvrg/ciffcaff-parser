#include "JpegEncoder.h"
#include <iostream>
#include <cstdio>
#include <jpeglib.h>

JpegEncoder::JpegEncoder(const std::string& filename, int width, int height, int quality)
    : filename_(filename), width_(width), height_(height), quality_(quality) {}

bool JpegEncoder::encode(const std::vector<uint8_t>& image) {
    // Step 1: Create JPEG structures and initialize error handling
    jpeg_compress_struct cinfo;
    jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

    // Step 2: Open the output file
    FILE* outfile = fopen(filename_.c_str(), "wb");
    if (!outfile) {
        std::cerr << "Failed to open output file." << std::endl;
        return false;
    }

    // Step 3: Set up compression parameters
    jpeg_stdio_dest(&cinfo, outfile);
    cinfo.image_width = width_;
    cinfo.image_height = height_;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality_, TRUE);

    // Step 4: Start compression
    jpeg_start_compress(&cinfo, TRUE);

    // Step 5: Compress each scanline
    JSAMPROW row_pointer[1];
    int row_stride = width_ * 3; // RGB: 3 bytes per pixel

    while (cinfo.next_scanline < cinfo.image_height) {
        row_pointer[0] = const_cast<JSAMPROW>(&image[cinfo.next_scanline * row_stride]);
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    // Step 6: Finish compression
    jpeg_finish_compress(&cinfo);

    // Step 7: Clean up and close the output file
    fclose(outfile);
    jpeg_destroy_compress(&cinfo);

    return true;
}
