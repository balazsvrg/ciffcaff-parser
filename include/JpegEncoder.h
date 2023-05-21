#ifndef JPEG_ENCODER_H
#define JPEG_ENCODER_H

#include <vector>
#include <cstdint>
#include <string>

class JpegEncoder {
public:
    JpegEncoder(const std::string& filename, 
                uint64_t width, 
                uint64_t height, 
                uint32_t quality);

    bool encode(const std::vector<uint8_t>& image);

private:
    std::string filename_;
    uint64_t width_;
    uint64_t height_;
    uint32_t quality_;
};

#endif  // JPEG_ENCODER_H
