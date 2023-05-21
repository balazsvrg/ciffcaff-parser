#include "caff.h"
#include <fstream>
#include <iostream>
#include <stdexcept>

const std::string VALID_MAGIC_VALUE = "CAFF";

const uint32_t BLOCK_ID_LENGTH = 1;
const uint32_t BLOCK_SIZE_LENGTH = 8;
const uint32_t MAGIC_LENGTH = 4;
const uint32_t HEADER_SIZE_LENGTH = 8;
const uint32_t NUM_ANIM_LENGTH = 8;

const uint32_t YEAR_LENGTH = 2;
const uint32_t MONTH_LENGTH = 1;
const uint32_t DAY_LENGTH = 1;
const uint32_t HOUR_LENGTH = 1;
const uint32_t MINUTE_LENGTH = 1;
const uint32_t CREATOR_LENGTH = 8;

const uint32_t DURATION_LENGTH = 8;

const uint32_t CAFF_HEADER_ID = 1;
const uint32_t CAFF_CREDITS_ID = 2;
const uint32_t CAFF_ANIM_ID = 3;



CAFF::CAFF(const std::string& filename) : filename(filename) { }

// Function to parse the CAFF image
bool CAFF::parse_image() {
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
        file.read(reinterpret_cast<char*>(&header_block_id), BLOCK_ID_LENGTH);

        if (header_block_id != CAFF_HEADER_ID){
            throw std::invalid_argument("CAFF header block ID must be 1");
        }

        file.read(reinterpret_cast<char*>(&header_block_size), BLOCK_SIZE_LENGTH);
        
        header.magic.resize(4);
        file.read(reinterpret_cast<char*>(&header.magic[0]), MAGIC_LENGTH);

        if (header.magic.compare(VALID_MAGIC_VALUE) != 0){
            throw std::invalid_argument("Magic value is not \"CAFF\"");
        }


        file.read(reinterpret_cast<char*>(&header.headerSize), HEADER_SIZE_LENGTH);
        file.read(reinterpret_cast<char*>(&header.numAnim), NUM_ANIM_LENGTH);

        // Read CAFF credits block 
        uint8_t credits_block_id;
        uint64_t credits_block_size;
        file.read(reinterpret_cast<char*>(&credits_block_id), BLOCK_ID_LENGTH);

        if (credits_block_id != CAFF_CREDITS_ID){
            throw std::invalid_argument("CAFF credits block ID must be 2");
        }

        file.read(reinterpret_cast<char*>(&credits_block_size), BLOCK_SIZE_LENGTH);
        file.read(reinterpret_cast<char*>(&credits.year), YEAR_LENGTH);
        file.read(reinterpret_cast<char*>(&credits.month), MONTH_LENGTH);
        file.read(reinterpret_cast<char*>(&credits.day), DAY_LENGTH);
        file.read(reinterpret_cast<char*>(&credits.hour), HOUR_LENGTH);
        file.read(reinterpret_cast<char*>(&credits.minute), MINUTE_LENGTH);
        file.read(reinterpret_cast<char*>(&credits.creatorLen), CREATOR_LENGTH);
        if (credits.creatorLen > 0) {
            credits.creator.resize(credits.creatorLen);
            file.read(&credits.creator[0], credits.creatorLen);
        }

        // Read CAFF animations
        animations.resize(header.numAnim);
        for (uint64_t i = 0; i < header.numAnim; ++i) {
            uint8_t anim_block_id;
            uint64_t anim_block_size;
            file.read(reinterpret_cast<char*>(&anim_block_id), BLOCK_ID_LENGTH);

            if (anim_block_id != CAFF_ANIM_ID){
                throw std::invalid_argument("CAFF header block ID must be 3");
            }

            file.read(reinterpret_cast<char*>(&anim_block_size), BLOCK_SIZE_LENGTH);
            CaffAnimation& animation = animations[i];
            file.read(reinterpret_cast<char*>(&animation.duration), DURATION_LENGTH);

            // Read CIFF data
            animation.ciff_data.parse_image_from_stream(file);

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
const CaffHeader& CAFF::get_header() const {
    return header;
}

// Getter for CAFF credits
const CaffCredits& CAFF::get_credits() const {
    return credits;
}

// Getter for CAFF animations
const std::vector<CaffAnimation>& CAFF::get_animations() const {
    return animations;
}
