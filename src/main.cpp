#include <iostream>

#include "caff.h"
#include "ciff.h"
#include "JpegEncoder.h"

int main() {
    // Specify the path to the CAFF file
    std::string caffFile = "/workspaces/ciffcaff-parser/data/3.caff";

    // Create an instance of the CAFF class
    CAFF caff(caffFile);

    // Parse the CAFF image
    if (caff.parseImage()) {
        
        CIFF image = caff.getAnimations().front().ciffData;
        CiffHeader header = image.get_header();
        JpegEncoder encoder("out.jpeg", header.width, header.height, 100);
        encoder.encode(image.get_data());

        // Output the header information

    }

    return 0;
}
