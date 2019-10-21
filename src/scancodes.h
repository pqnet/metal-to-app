#ifndef SCANCODES_H
#define SCANCODES_H
#include <stdint.h>

/**
 * Use negative values for keyup, positive for keydown
 * Shift new scancodes into buffer, one at a time
 * and invoke this function until it returns 0.
 * Produced codes are in the usb_hid_keys.h file
 **/
extern int16_t ps2Set2ToUsb(uint64_t *scancodeBuffer);

#endif// SCANCODES_H