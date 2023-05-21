#ifndef JPEG_ENCODER_H
#define JPEG_ENCODER_H

#include <vector>
#include <cstdint>
#include <string>

class JpegEncoder {
public:
    JpegEncoder(const std::string& filename, int width, int height, int quality = 90);

    bool encode(const std::vector<uint8_t>& image);

private:
    std::string filename_;
    int width_;
    int height_;
    int quality_;
};

#endif  // JPEG_ENCODER_H
