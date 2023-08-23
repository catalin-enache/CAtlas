
#ifndef ATLAS_ERRORS_H
#define ATLAS_ERRORS_H

//#define MAGICKCORE_QUANTUM_DEPTH 32
//#define MAGICKCORE_HDRI_ENABLE 1
#include <ImageMagick-7/MagickWand/MagickWand.h>

#include <stdarg.h>

int exit_with_error(const char* format, ...);
#endif //ATLAS_ERRORS_H
