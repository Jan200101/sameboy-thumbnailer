#ifndef EMBED_H
#define EMBED_H

#include <stddef.h>
#include <stdint.h>

extern const uint8_t cgb_boot_bin[];
extern const size_t cgb_boot_bin_size;

extern unsigned char CartridgeTemplate_png[];
extern unsigned int CartridgeTemplate_png_size;

extern unsigned char ColorCartridgeTemplate_png[];
extern unsigned int ColorCartridgeTemplate_png_size;

extern unsigned char UniversalCartridgeTemplate_png[];
extern unsigned int UniversalCartridgeTemplate_png_size;

#endif