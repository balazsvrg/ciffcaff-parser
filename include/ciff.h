#ifndef CIFF_PARSER_H
#define CIFF_PARSER_H

#include <cstdint>
#include <string>
#include <vector>

class CIFF {
public:
    struct Image {
        uint64_t width;
        uint64_t height;
        std::string caption;
        std::vector<std::string> tags;
        std::vector<uint8_t> pixels;
    };

    static Image parse_image(const std::string& file_path);
};

#endif
