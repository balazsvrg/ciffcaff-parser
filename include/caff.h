#ifndef CAFF_H
#define CAFF_H

#include <string>
#include <vector>

#include "ciff.h"

// Structure for storing CAFF header information
struct CaffHeader {
    std::string magic;
    uint64_t headerSize;
    uint64_t numAnim;
};

// Structure for storing CAFF credits information
struct CaffCredits {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint64_t creatorLen;
    std::string creator;
};

// Structure for storing CAFF animation information
struct CaffAnimation {
    uint64_t duration;
    CIFF ciffData;
};

// Class for parsing a CAFF image
class CAFF {
public:
    // Constructor
    CAFF(const std::string& filename);

    // Function to parse the CAFF image
    bool parseImage();

    // Getters for CAFF data
    const CaffHeader& getHeader() const;
    const CaffCredits& getCredits() const;
    const std::vector<CaffAnimation>& getAnimations() const;

private:
    std::string filename;
    CaffHeader header;
    CaffCredits credits;
    std::vector<CaffAnimation> animations;

};

#endif  // CAFF_PARSER_H
