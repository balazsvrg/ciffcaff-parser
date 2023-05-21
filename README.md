# Software Security Homework Assignment - Parser

This is an open-source project written in C/C++ that aims to generate preview images in JPEG or WebP format from CIFF and CAFF image files. The purpose of this project is to apply the learned methods in practice and develop a security-critical software.

## File Formats Used

### CIFF
The CrySyS Image File Format is an uncompressed image format that contains direct pixel information along with metadata associated with the image. The format specification can be found [here](https://www.crysys.hu/downloads/vihima06/2020/CIFF.txt).

### CAFF
The CrySyS Animation File Format is an uncompressed animation format that allows storing CIFF images along with metadata associated with animated images. The format specification can be found [here](link-to-specification-caff).

## Libraries
The application uses the `libjpeg` library to create JPEG images from the image data stored in the `.ciff` or `.caff` files, thus the library must be installed on the system.

Installation on Ubuntu 22.04 LTS:
``` bash
sudo apt-get update
sudo apt-get install libjpeg-dev
```
## Compilation
A makefile is provided for the application. Run `make` to compile the code. The executable can be found in the `bin` folder.

`make clean` removes all files from the `bin` folder.

## Execution
The application processes a file received as a command line parameter.

Example:

``` bash
./parser -caff [path-to-caff].caff
./parser -ciff [path-to-ciff].ciff
```
