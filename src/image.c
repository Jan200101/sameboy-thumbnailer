#include "image.h"
#include "common.h"

unsigned decode32(uint32_t** out, unsigned* w, unsigned* h,
                  unsigned char* in, size_t insize)
{
#if defined(USE_LIBPNG)
    unsigned retval = 1;

    FILE* fp = fmemopen(in, insize, "rb");;
    png_bytep* row_pointers = NULL;

    if (insize < 8 || png_sig_cmp(in, 0, 8))  abort();

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info = png_create_info_struct(png);

    if(setjmp(png_jmpbuf(png))) goto cleanup;

    png_init_io(png, fp);
    png_read_info(png, info);

    if(png_get_bit_depth(png, info) == 16) png_set_strip_16(png);

    *w = png_get_image_width(png, info);
    *h = png_get_image_height(png, info);

    png_byte color_type = png_get_color_type(png, info);

    if(color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);
 
    if(png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    row_pointers = local_malloc(sizeof(png_bytep) * *h);
    *out = local_malloc(sizeof(**out) * (*w * *h));
    for(int y = 0; y < *h; y++)
    {
        row_pointers[y] = (png_byte*)(*out+((*w)*y));
    }

    png_read_image(png, row_pointers);

    retval = 0;

    cleanup:
    png_destroy_read_struct(&png, &info, NULL);

    if (fp) fclose(fp);
    if (row_pointers) free(row_pointers);

    return retval;
#elif defined(USE_LODEPNG)
    return lodepng_decode32((unsigned char**)out, w, h, in, insize);
#else
    #error "No decode implementation"
#endif
}

unsigned encode32_file(const char* filename, const uint32_t* image, unsigned w, unsigned h)
{
#if defined(USE_LIBPNG)
    /* create file */
    FILE *fp = fopen(filename, "wb");
    png_bytep* row_pointers = NULL;

    /* initialize stuff */
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info_ptr = png_create_info_struct(png);
    if(setjmp(png_jmpbuf(png))) goto cleanup;

    png_init_io(png, fp);


    png_set_IHDR(png, info_ptr, w, h,
                 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_write_info(png, info_ptr);

    row_pointers = local_malloc(sizeof(png_bytep) * h);
    for(int y = 0; y < h; y++)
    {
        row_pointers[y] = (png_byte*)(image+(w*y));
    }

    png_write_image(png, row_pointers);

    png_write_end(png, NULL);

    cleanup:

    free(row_pointers);
    fclose(fp);
#elif defined(USE_LODEPNG)
    return lodepng_encode32_file(filename, image, w, h);
#else
    #error "No encode implementation"
#endif
}