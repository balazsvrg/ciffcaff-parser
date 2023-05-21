#include <iostream>
#include "caff.h"

int main() {
    // Specify the path to the CAFF file
    std::string caffFile = "/workspaces/ciffcaff-parser/data/1.caff";

    // Create an instance of the CAFF class
    CAFF caff(caffFile);

    // Parse the CAFF image
    if (caff.parseImage()) {
        // Get the CAFF header
        const CaffHeader& header = caff.getHeader();

        // Output the header information
        std::cout << "CAFF Header:" << std::endl;
        std::cout << "Magic: " << header.magic << std::endl;
        std::cout << "Header Size: " << header.headerSize << std::endl;
        std::cout << "Number of Animations: " << header.numAnim << std::endl;
    }

    return 0;
}
