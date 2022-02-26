#ifndef IMAGE_H
#define IMAGE_H

#if defined(USE_LIBPNG)
#include <png.h>
#elif defined(USE_LODEPNG)
#include "lodepng.h"
#else
#error "No png implementation"
#endif

#include <stdint.h>
#include <stddef.h>

// API structured after lodepng API

unsigned decode32(uint32_t** out, unsigned* w, unsigned* h, unsigned char* in, size_t insize);
unsigned encode32_file(const char* filename, const uint32_t* image, unsigned w, unsigned h);

#endif