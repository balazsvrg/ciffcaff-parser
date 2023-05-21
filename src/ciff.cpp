#include <fstream>
#include <iostream>

#include "ciff.h"

CIFF::CIFF(const std::string& file_path) : file_path(file_path) {}

CIFF::CIFF() {}

bool CIFF::parse_image() {
    std::ifstream filestream(file_path, std::ios::binary);

    if (!filestream) {
        std::cerr << "Failed to open CIFF file." << std::endl;
        return false;
    }

    const bool success = parse_image_from_stream(filestream);

    filestream.close();
    return success;
}

bool CIFF::parse_image_from_stream(std::ifstream& filestream) {
    // Read the header
    filestream.read(reinterpret_cast<char*>(&header.magic[0]), 4);
    filestream.read(reinterpret_cast<char*>(&header.header_size), sizeof(uint64_t));
    filestream.read(reinterpret_cast<char*>(&header.content_size), sizeof(uint64_t));
    filestream.read(reinterpret_cast<char*>(&header.width), sizeof(uint64_t));
    filestream.read(reinterpret_cast<char*>(&header.height), sizeof(uint64_t));

    // Read the caption
    char ch;
    while (filestream.get(ch)) {
        if (ch == '\n')
            break;
        header.caption += ch;
    }

    // Read tags
    uint32_t remainder_size = header.header_size - 36 - header.caption.length() - 1;
    std::string tag;
    for (size_t i = 0; i < remainder_size; i++) {
        filestream.get(ch);
        if (ch == '\0') {
            if (!tag.empty()) {
                header.tags.push_back(tag);
                tag.clear();
            }
        } else {
            tag += ch;
        }
    }

    // Read image data
    image_data.resize(header.content_size);
    filestream.read(reinterpret_cast<char*>(image_data.data()), header.content_size);

    return true;
}

const CiffHeader& CIFF::get_header() const {
    return header;
}

const std::vector<uint8_t>& CIFF::get_data() const {
    return image_data;
}