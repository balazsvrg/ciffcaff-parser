#include "include/caff.h"
#include <fstream>
#include <iostream>
#include <stdexcept>

// CAFF class constructor
CAFF::CAFF(const std::string& filename) : filename(filename) {}

// Function to parse the CAFF image
bool CAFF::parseImage() {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }

    try {
        // Read CAFF header
        file.read(reinterpret_cast<char*>(&header.magic[0]), 4);
        header.magic.resize(4);
        file.read(reinterpret_cast<char*>(&header.headerSize), sizeof(uint64_t));
        file.read(reinterpret_cast<char*>(&header.numAnim), sizeof(uint64_t));

        // Read CAFF credits
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
            CaffAnimation& animation = animations[i];
            file.read(reinterpret_cast<char*>(&animation.duration), sizeof(uint64_t));

            // Read CIFF data
            uint64_t dataSize = header.headerSize - sizeof(uint64_t) - credits.creatorLen - 38;  // Assuming CIFF header size is 38 bytes
            animation.ciffData.resize(dataSize);
            file.read(reinterpret_cast<char*>(&animation.ciffData[0]), dataSize);
        }

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
