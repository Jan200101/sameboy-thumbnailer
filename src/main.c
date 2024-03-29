#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "common.h"
#include "embed.h"
#include "get_image_for_rom.h"
#include "image.h"

static uint32_t alpha_blend(const uint32_t dest, const uint32_t src)
{
    uint8_t r1 = (src >> 0)  & 0xFF;
    uint8_t g1 = (src >> 8)  & 0xFF;
    uint8_t b1 = (src >> 16) & 0xFF;
    double a1 = (double)((src >> 24) & 0xFF) / 255.0;

    uint8_t r2 = (dest >> 0)  & 0xFF;
    uint8_t g2 = (dest >> 8)  & 0xFF;
    uint8_t b2 = (dest >> 16) & 0xFF;
    double a2 = (double)((dest >> 24) & 0xFF) / 255.0;

    uint8_t a = (a1 + (1.0 - a1) * a2) * 255.0;

    if (a > 0) {
        uint8_t r = (255 / a) * ((a1 * r1) + (1 - a1) * r2);
        uint8_t g = (255 / a) * ((a1 * g1) + (1 - a1) * g2);
        uint8_t b = (255 / a) * ((a1 * b1) + (1 - a1) * b2);

        return (a << 24) | (b << 16) | (g << 8) | r;
    }

    return 0;
}

static uint32_t average(const uint32_t dest, const uint32_t src)
{
    uint8_t r1 = (src >> 0)  & 0xFF;
    uint8_t g1 = (src >> 8)  & 0xFF;
    uint8_t b1 = (src >> 16) & 0xFF;
    uint8_t a1 = (src >> 24) & 0xFF;

    uint8_t r2 = (dest >> 0)  & 0xFF;
    uint8_t g2 = (dest >> 8)  & 0xFF;
    uint8_t b2 = (dest >> 16) & 0xFF;
    uint8_t a2 = (dest >> 24) & 0xFF;

    if (a2 > 0) {
        uint8_t r = (r1 + r2) / 2;
        uint8_t g = (g1 + g2) / 2;
        uint8_t b = (b1 + b2) / 2;
        uint8_t a = (a1 + a2) / 2;

        return (a << 24) | (b << 16) | (g << 8) | r;
    }

    return src;
}

static void scale_image(const uint32_t* input, const signed input_width, const signed input_height,
                        uint32_t* output, const double multiplier, signed samples)
{
    if (samples <= 0)
        samples = 1;

    signed output_width = input_width * multiplier;
    uint32_t pixel;

    for (signed h = 0; h < input_height * multiplier; ++h) {
        for (signed w = 0; w < input_width * multiplier; ++w) {
            pixel = 0;

            signed h_input = h/multiplier;
            signed w_input = w/multiplier;

            signed h_input_max = (h+1)/multiplier-1;
            signed w_input_max = (w+1)/multiplier-1;

            signed h_step = h_input_max - h_input + 2;
            signed w_step = w_input_max - w_input + 2;

            for (signed xa = w_input; xa < w_input_max + 2 && xa < input_width; xa += w_step / samples)
                for (signed ya = h_input; ya < h_input_max + 2 && ya < input_width; ya += h_step / samples)
                    pixel = average(pixel, input[(signed)xa + ((signed)ya * input_width)]);

            output[w + (h * output_width)] = pixel;
        }
    }
}

int main (int argc , char** argv)
{
    const uint8_t BITMAP_WIDTH = 160;
    const uint8_t BITMAP_HEIGHT = 144;
    const uint8_t SCALE_MULTIPLIER = 4;

    const uint16_t UPSCALED_WIDTH = BITMAP_WIDTH * SCALE_MULTIPLIER;
    const uint16_t UPSCALED_HEIGHT = BITMAP_HEIGHT * SCALE_MULTIPLIER;

    const uint16_t X_OFFSET = 192;
    const uint16_t Y_OFFSET = 298;

    int size = 256;

    char* input = NULL;
    char* output = NULL;

    const uint8_t* bootrom = cgb_boot_bin;
    const size_t bootrom_size  = cgb_boot_bin_size;

    char* bootrom_path = NULL;

    for (int i = 1; i < argc; ++i) {
        if (argc > i + 1) {
            if (strcmp(argv[i], "--size") == 0) {
                size = atoi(argv[++i]);
                if (size < 0) {
                    fprintf(stderr, "Size cannot be smaller than 0\n");
                    return -1;
                }
                fprintf(stderr, "Setting size to %i\n", size);
                continue;
            }

            if (strcmp(argv[i], "--duration") == 0) {
                unsigned int arg_dur = atoi(argv[++i]);
                if (arg_dur < 1) {
                    fprintf(stderr, "Duration cannot be smaller than 1\n");
                    return -1;
                }
                fprintf(stderr, "Setting duration to %i seconds\n", arg_dur);
                duration = arg_dur * 60;
                continue;
            }

            if (strcmp(argv[i], "--boot") == 0) {
                bootrom_path = argv[++i];
                fprintf(stderr, "Using boot ROM %s\n", bootrom_path);
                continue;
            }
        }

        if (!input) {
            input = argv[i];
            continue;
        }

        if (!output) {
            output = argv[i];
            continue;
        }
    }

    if (!input || !output)
    {
        fprintf(stderr,
            "Usage: %s input output [options]\n"
            "\n"
            "Options:\n"
            "\t--size [%i]\t\tset the size of the emulated screen\n"
            "\t--duration [%i]\tset duration of emulation in seconds\n"
            "\t--boot [path]\tspecify an external bootrom\n",
            argv[0], size, duration/10);

        return 0;
    }

    uint32_t bitmap[BITMAP_WIDTH*BITMAP_HEIGHT];
    uint8_t cgbFlag = 0;

    if (bootrom_path ? get_image_for_rom(input, bootrom_path, bitmap, &cgbFlag)
                     : get_image_for_rom_alt(input, bootrom, bootrom_size, bitmap, &cgbFlag)) {
        return -1;
    }

    unsigned template_width, template_height;
    uint32_t* template;
    unsigned char* template_data;
    size_t template_size;

    switch (cgbFlag) {
        case 0xC0:
            template_data = ColorCartridgeTemplate_png;
            template_size = ColorCartridgeTemplate_png_size;
            break;

        case 0x80:
            template_data = UniversalCartridgeTemplate_png;
            template_size = UniversalCartridgeTemplate_png_size;
            break;

        default:
            template_data = CartridgeTemplate_png;
            template_size = CartridgeTemplate_png_size;
    }

    if (decode32(&template, &template_width, &template_height, template_data, template_size)) {
        fprintf(stderr, "Failed to decode template\n");
        return -1;
    }

    uint32_t* canvas = local_malloc(template_width * template_height * sizeof(uint32_t));
    // clear the canvas so we don't end up with garbage data somewhere
    memset(canvas, 0, template_width * template_height * sizeof(uint32_t));

    uint32_t* screen = local_malloc(UPSCALED_WIDTH * UPSCALED_HEIGHT * sizeof(uint32_t));
    scale_image(bitmap, BITMAP_WIDTH, BITMAP_HEIGHT, screen, SCALE_MULTIPLIER, 1);

    for (signed y = 0; y < UPSCALED_HEIGHT; y++) {
        for (signed x = 0; x < UPSCALED_WIDTH; x++) {
            canvas[x + X_OFFSET + ((y + Y_OFFSET) * template_height)] = screen[x + (y * UPSCALED_WIDTH)];
        }
    }

    free(screen);

    for (signed y = 0; y < template_height; y++) {
        for (signed x = 0; x < template_width; x++) {
            canvas[x + (y * template_width)] = alpha_blend(
                canvas[x + (y * template_width)],
                template[x + (y * template_width)]
            );
        }
    }

    uint32_t* final = local_malloc(size * size * sizeof(uint32_t));
    scale_image(canvas, template_width, template_height, final, (double)size/template_width, template_width/size);

    encode32_file(output, final, size, size);

    free(final);
    free(canvas);
    free(template);
}
