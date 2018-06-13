# libtga
## Library for work with .tga format in C language.

### Install
  * Copy .h & .c files to any directory
  * Compile .c with:
  ```bash
  gcc -c tga_img.c -o tga_img.o
  ```
  * Move .o file to archive with:
  ```bash
  ar -q -c -s libtga.a tga_img.o
  ```
  * Move .h & .a files to any directory (I'm using the root directory - /libs)
  ```bash
  mkdir /libs
  mv tga_img.h /libs/
  mv libtga.o /libs/
  ```
  
### Using
  * In your .c file add next string:
  ```C
  #include "tga_img.h"
  ```
  * Use functions from .h file in your code (comments in .h are quite understandable)
  * Compile your program with:
  ```bash
  gcc your_prog.c -o out_file -ltga -I/libs -L/libs
  ```
  * Done!


(The documentation will be completed some time later...)
