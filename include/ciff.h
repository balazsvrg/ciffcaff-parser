#ifndef CIFF_PARSER_H
#define CIFF_PARSER_H

#include <cstdint>
#include <string>
#include <vector>

struct CiffHeader {
    std::string magic;
    uint64_t header_size;
    uint64_t content_size;
    uint64_t width;
    uint64_t height;
    std::string caption;
    std::vector<std::string> tags;
};


class CIFF {
public:
    //COnstructor
    CIFF(const std::string& file_path);
    CIFF();

    //bool parse_image();
    bool parse_image_from_caff(std::ifstream& filestream);

    const CiffHeader& get_header() const;
    const std::vector<uint8_t>& get_data() const;

private:
    std::string file_path;
    CiffHeader header;
    std::vector<uint8_t> image_data;
};

#endif
