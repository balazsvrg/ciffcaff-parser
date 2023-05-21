#include <iostream>
#include <string>
#include "ciff.h"
#include "caff.h"
#include "JpegEncoder.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " -[ciff|caff] <path_to_image>" << std::endl;
        return -1;
    }

    std::string mode = argv[1];
    std::string image_file_path = argv[2];

    if (mode == "-ciff") {
        CIFF ciff_image(image_file_path);

        if (!ciff_image.parse_image()) {
            std::cerr << "Failed to parse CIFF file: " << image_file_path << std::endl;
            return -1;
        }

        // Generate output JPEG filename
        std::string output_filename = image_file_path.substr(0, image_file_path.find_last_of('.')) + ".jpeg";

        // Encode CIFF image as JPEG
        JpegEncoder jpeg_encoder(output_filename, ciff_image.get_header().width, ciff_image.get_header().height);
        if (!jpeg_encoder.encode(ciff_image.get_data())) {
            std::cerr << "Failed to encode CIFF image to JPEG: " << output_filename << std::endl;
            return -1;
        }
    } else if (mode == "-caff") {
        CAFF caff_image(image_file_path);

        if (!caff_image.parse_image()) {
            std::cerr << "Failed to parse CAFF file: " << image_file_path << std::endl;
            return -1;
        }

        const std::vector<CaffAnimation>& animations = caff_image.get_animations();
        if (animations.empty()) {
            std::cerr << "No animations found in CAFF file: " << image_file_path << std::endl;
            return -1;
        }

        // Extract the first frame from the CAFF animation
        const CIFF& ciff_frame = animations[0].ciff_data;

        // Generate output JPEG filename
        std::string output_filename = image_file_path.substr(0, image_file_path.find_last_of('.')) + ".jpeg";

        // Encode CIFF frame as JPEG
        JpegEncoder jpeg_encoder(output_filename, ciff_frame.get_header().width, ciff_frame.get_header().height, 80);
        if (!jpeg_encoder.encode(ciff_frame.get_data())) {
            std::cerr << "Failed to encode CIFF frame to JPEG: " << output_filename << std::endl;
            return -1;
        }
    } else {
        std::cerr << "Invalid mode specified. Use '-ciff' or '-caff'." << std::endl;
        return -1;
    }

    std::cout << "Image conversion completed successfully." << std::endl;
    return 0;
}
