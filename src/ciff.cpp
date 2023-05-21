#include <fstream>
#include <iostream>

#include "ciff.h"

const uint32_t MAGIC_LENGTH = 4;
const uint32_t HEADER_SIZE_LENGTH = 8;
const uint32_t CONTENT_SIZE_LENGTH = 8;
const uint32_t WIDTH_LENGTH = 8;
const uint32_t HEIGHT_LENGTH = 8;

const uint32_t HEADER_FIXED_LENGTH_VALUES_LENGTH = 36;
const std::string VALID_MAGIC_VALUE = "CIFF";

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
    try{
        // Read the header
        header.magic.resize(MAGIC_LENGTH);
        filestream.read(reinterpret_cast<char*>(&header.magic[0]), MAGIC_LENGTH);

        if (header.magic.compare(VALID_MAGIC_VALUE) != 0){
            throw std::invalid_argument("Magic value is not \"CIFF\"");
        }

        filestream.read(reinterpret_cast<char*>(&header.header_size), HEADER_SIZE_LENGTH);
        filestream.read(reinterpret_cast<char*>(&header.content_size), CONTENT_SIZE_LENGTH);
        filestream.read(reinterpret_cast<char*>(&header.width), WIDTH_LENGTH);
        filestream.read(reinterpret_cast<char*>(&header.height), HEIGHT_LENGTH);

        if (header.content_size != header.width * header.height * 3) {
            throw std::domain_error("Content size does not match Width and Height values in CIFF");
        }

        // Read the caption
        char ch;
        while (filestream.get(ch)) {
            if (ch == '\n')
                break;
            header.caption += ch;
        }

        // Read tags
        uint32_t remainder_size = header.header_size - HEADER_FIXED_LENGTH_VALUES_LENGTH - header.caption.length() - 1;
        std::string tag;
        for (size_t i = 0; i < remainder_size; i++) {
            filestream.get(ch);
            if (ch == '\0') {
                if (!tag.empty()) {
                    header.tags.push_back(tag);
                    tag.clear();
                }
            } else if (ch == '\n'){
                throw std::domain_error("Tags cannot contain line breaks");    
            } else{
                tag += ch;
            }
        }

        // Read image data
        image_data.resize(header.content_size);
        filestream.read(reinterpret_cast<char*>(image_data.data()), header.content_size);

        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error while parsing the CIFF image: " << e.what() << std::endl;
        filestream.close();
        return false;
    }
    
}

const CiffHeader& CIFF::get_header() const {
    return header;
}

const std::vector<uint8_t>& CIFF::get_data() const {
    return image_data;
}