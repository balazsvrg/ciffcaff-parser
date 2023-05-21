#include <fstream>
#include <iostream>

#include "ciff.h"

CIFF::CIFF(const std::string& file_path) : file_path(file_path) { }
CIFF::CIFF() { }
/*
bool CIFF::parse_image() {
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
}*/

bool CIFF::parse_image_from_caff(std::ifstream& filestream){

    //read header
    header.magic.resize(4);
    filestream.read(reinterpret_cast<char*>(&header.magic[0]), 4);
    filestream.read(reinterpret_cast<char*>(&header.header_size), sizeof(uint64_t));
    filestream.read(reinterpret_cast<char*>(&header.content_size), sizeof(uint64_t));
    filestream.read(reinterpret_cast<char*>(&header.width), sizeof(uint64_t));
    filestream.read(reinterpret_cast<char*>(&header.height), sizeof(uint64_t));
    /*
    std::string remainder;
    uint32_t remainder_size = header.header_size - 36;
    remainder.resize(remainder_size);
    
    filestream.read(reinterpret_cast<char*>(&remainder), remainder_size);
    */

    // Read the caption
    char ch;
    while (filestream.get(ch)) {
        if (ch == '\n')
            break;
        header.caption += ch;
    }

    //read tags
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
    
    for (size_t i = 0; i < header.content_size; i++){
        filestream.get(ch);
        image_data.push_back(ch);
    }

    return true;
}
