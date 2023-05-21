#include "caff.h"
#include <fstream>
#include <iostream>
#include <stdexcept>

// CAFF class constructor
CAFF::CAFF(const std::string& filename) : filename(filename) { }

// Function to parse the CAFF image
bool CAFF::parseImage() {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }

    try {
        // Read CAFF header

        //read header block 
        uint8_t header_block_id;
        uint64_t header_block_size;
        file.read(reinterpret_cast<char*>(&header_block_id), 1);
        file.read(reinterpret_cast<char*>(&header_block_size), 8);
        header.magic.resize(4);
        file.read(reinterpret_cast<char*>(&header.magic[0]), 4);
        file.read(reinterpret_cast<char*>(&header.headerSize), sizeof(uint64_t));
        file.read(reinterpret_cast<char*>(&header.numAnim), sizeof(uint64_t));

        // Read CAFF credits block 
        uint8_t credits_block_id;
        uint64_t credits_block_size;
        file.read(reinterpret_cast<char*>(&credits_block_id), 1);
        file.read(reinterpret_cast<char*>(&credits_block_size), 8);
        file.read(reinterpret_cast<char*>(&credits.year), sizeof(uint16_t));
        file.read(reinterpret_cast<char*>(&credits.month), sizeof(uint8_t));
        file.read(reinterpret_cast<char*>(&credits.day), sizeof(uint8_t));
        file.read(reinterpret_cast<char*>(&credits.hour), sizeof(uint8_t));
        file.read(reinterpret_cast<char*>(&credits.minute), sizeof(uint8_t));
        file.read(reinterpret_cast<char*>(&credits.creatorLen), sizeof(uint64_t));
        if (credits.creatorLen > 0) {
            credits.creator.resize(credits.creatorLen);
            file.read(&credits.creator[0], credits.creatorLen);
        }

        // Read CAFF animations
        animations.resize(header.numAnim);
        for (uint64_t i = 0; i < header.numAnim; ++i) {
            uint8_t anim_block_id;
            uint64_t anim_block_size;
            file.read(reinterpret_cast<char*>(&anim_block_id), 1);
            file.read(reinterpret_cast<char*>(&anim_block_size), 8);
            CaffAnimation& animation = animations[i];
            file.read(reinterpret_cast<char*>(&animation.duration), sizeof(uint64_t));

            // Read CIFF data
            animation.ciffData.parse_image_from_caff(file);


            /*
            uint64_t dataSize = header.headerSize - sizeof(uint64_t) - credits.creatorLen - 38;  // Assuming CIFF header size is 38 bytes
            animation.ciffData.resize(dataSize);
            file.read(reinterpret_cast<char*>(&animation.ciffData[0]), dataSize);*/
        }
        std::cout << "animations read" << std::endl;
        file.close();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error while parsing the CAFF image: " << e.what() << std::endl;
        file.close();
        return false;
    }
}

// Getter for CAFF header
const CaffHeader& CAFF::getHeader() const {
    return header;
}

// Getter for CAFF credits
const CaffCredits& CAFF::getCredits() const {
    return credits;
}

// Getter for CAFF animations
const std::vector<CaffAnimation>& CAFF::getAnimations() const {
    return animations;
}
