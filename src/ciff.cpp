#include "include/ciff.h"
#include <fstream>
#include <iostream>

CIFF::Image CIFF::parse_image(const std::string& file_path) {
    std::ifstream ciffFile(file_path, std::ios::binary);

    if (!ciffFile) {
        std::cerr << "Failed to open CIFF file." << std::endl;
        return {};
    }

    Image imageData;

    // Read the CIFF header
    char magic[5];
    uint64_t headerSize, contentSize;

    ciffFile.read(magic, 4);
    magic[4] = '\0';

    if (std::string(magic) != "CIFF") {
        std::cerr << "Invalid CIFF file." << std::endl;
        return {};
    }

    ciffFile.read(reinterpret_cast<char*>(&headerSize), sizeof(uint64_t));
    ciffFile.read(reinterpret_cast<char*>(&contentSize), sizeof(uint64_t));

    ciffFile.read(reinterpret_cast<char*>(&imageData.width), sizeof(uint64_t));
    ciffFile.read(reinterpret_cast<char*>(&imageData.height), sizeof(uint64_t));

    // Read the caption
    char ch;
    while (ciffFile.get(ch)) {
        if (ch == '\n')
            break;
        imageData.caption += ch;
    }

    // Read the tags
    std::string tag;
    while (ciffFile.get(ch)) {
        if (ch == '\0') {
            if (!tag.empty()) {
                imageData.tags.push_back(tag);
                tag.clear();
            }
        } else {
            tag += ch;
        }
    }

    // Read the image data
    imageData.pixels.resize(contentSize);
    ciffFile.read(reinterpret_cast<char*>(imageData.pixels.data()), contentSize);

    ciffFile.close();

    return imageData;
}
