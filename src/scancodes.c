
#include "scancodes.h"
#include "usb_hid_keys.h"

/**
 * Use negative values for keyup, positive for keydown
 * Shift new scancodes into buffer, one at a time
 * and invoke this function until it returns 0
 **/
int16_t ps2Set2ToUsb(uint64_t *scancodeBuffer)
{
    uint64_t code = *scancodeBuffer;
    *scancodeBuffer = 0;
    switch (code >> 8)
    {
    // Single byte
    case 0:
        switch (code)
        {
        case 0x00:
            return KEY_NONE;
        case 0x01:
            return KEY_F9;
        case 0x02:
            return KEY_NONE;
        case 0x03:
            return KEY_F5;
        case 0x04:
            return KEY_F3;
        case 0x05:
            return KEY_F1;
        case 0x06:
            return KEY_F2;
        case 0x07:
            return KEY_F12;
        case 0x08:
            return KEY_NONE;
        case 0x09:
            return KEY_F10;
        case 0x0A:
            return KEY_F8;
        case 0x0B:
            return KEY_F6;
        case 0x0C:
            return KEY_F4;
        case 0x0D:
            return KEY_TAB;
        case 0x0E:
            return KEY_GRAVE;
        case 0x0F:
            return KEY_NONE;
        case 0x10:
            return KEY_NONE;
        case 0x11:
            return KEY_MOD_LALT;
        case 0x12:
            return KEY_MOD_LSHIFT;
        case 0x13:
            return KEY_NONE;
        case 0x14:
            return KEY_MOD_LCTRL;
        case 0x15:
            return KEY_Q;
        case 0x16:
            return KEY_1;
        case 0x17:
            return KEY_NONE;
        case 0x18:
            return KEY_NONE;
        case 0x19:
            return KEY_NONE;
        case 0x1A:
            return KEY_Z;
        case 0x1B:
            return KEY_S;
        case 0x1C:
            return KEY_A;
        case 0x1D:
            return KEY_W;
        case 0x1E:
            return KEY_2;
        case 0x1F:
            return KEY_NONE;
        case 0x20:
            return KEY_NONE;
        case 0x21:
            return KEY_C;
        case 0x22:
            return KEY_X;
        case 0x23:
            return KEY_D;
        case 0x24:
            return KEY_E;
        case 0x25:
            return KEY_4;
        case 0x26:
            return KEY_3;
        case 0x27:
            return KEY_NONE;
        case 0x28:
            return KEY_NONE;
        case 0x29:
            return KEY_SPACE;
        case 0x2A:
            return KEY_V;
        case 0x2B:
            return KEY_F;
        case 0x2C:
            return KEY_T;
        case 0x2D:
            return KEY_R;
        case 0x2E:
            return KEY_5;
        case 0x2F:
            return KEY_NONE;
        case 0x30:
            return KEY_NONE;
        case 0x31:
            return KEY_N;
        case 0x32:
            return KEY_B;
        case 0x33:
            return KEY_H;
        case 0x34:
            return KEY_G;
        case 0x35:
            return KEY_Y;
        case 0x36:
            return KEY_6;
        case 0x37:
            return KEY_NONE;
        case 0x38:
            return KEY_NONE;
        case 0x39:
            return KEY_NONE;
        case 0x3A:
            return KEY_M;
        case 0x3B:
            return KEY_J;
        case 0x3C:
            return KEY_U;
        case 0x3D:
            return KEY_7;
        case 0x3E:
            return KEY_8;
        case 0x3F:
            return KEY_NONE;
        case 0x40:
            return KEY_NONE;
        case 0x41:
            return KEY_COMMA;
        case 0x42:
            return KEY_K;
        case 0x43:
            return KEY_I;
        case 0x44:
            return KEY_O;
        case 0x45:
            return KEY_0;
        case 0x46:
            return KEY_9;
        case 0x47:
            return KEY_NONE;
        case 0x48:
            return KEY_NONE;
        case 0x49:
            return KEY_DOT;
        case 0x4A:
            return KEY_SLASH;
        case 0x4B:
            return KEY_L;
        case 0x4C:
            return KEY_SEMICOLON;
        case 0x4D:
            return KEY_P;
        case 0x4E:
            return KEY_MINUS;
        case 0x4F:
            return KEY_NONE;
        case 0x50:
            return KEY_NONE;
        case 0x51:
            return KEY_NONE;
        case 0x52:
            return KEY_APOSTROPHE;
        case 0x53:
            return KEY_NONE;
        case 0x54:
            return KEY_LEFTBRACE;
        case 0x55:
            return KEY_EQUAL;
        case 0x56:
            return KEY_NONE;
        case 0x57:
            return KEY_NONE;
        case 0x58:
            return KEY_CAPSLOCK;
        case 0x59:
            return KEY_RIGHTSHIFT;
        case 0x5A:
            return KEY_ENTER;
        case 0x5B:
            return KEY_RIGHTBRACE;
        case 0x5C:
            return KEY_NONE;
        case 0x5D:
            return KEY_BACKSLASH;
        case 0x5E:
            return KEY_NONE;
        case 0x5F:
            return KEY_NONE;
        case 0x60:
            return KEY_NONE;
        case 0x61:
            return KEY_NONE;
        case 0x62:
            return KEY_NONE;
        case 0x63:
            return KEY_NONE;
        case 0x64:
            return KEY_NONE;
        case 0x65:
            return KEY_NONE;
        case 0x66:
            return KEY_BACKSPACE;
        case 0x67:
            return KEY_NONE;
        case 0x68:
            return KEY_NONE;
        case 0x69:
            return KEY_KP1;
        case 0x6A:
            return KEY_NONE;
        case 0x6B:
            return KEY_KP4;
        case 0x6C:
            return KEY_KP7;
        case 0x6D:
            return KEY_NONE;
        case 0x6E:
            return KEY_NONE;
        case 0x6F:
            return KEY_NONE;
        case 0x70:
            return KEY_KP0;
        case 0x71:
            return KEY_KPDOT;
        case 0x72:
            return KEY_KP2;
        case 0x73:
            return KEY_KP5;
        case 0x74:
            return KEY_KP6;
        case 0x75:
            return KEY_KP8;
        case 0x76:
            return KEY_ESC;
        case 0x77:
            return KEY_NUMLOCK;
        case 0x78:
            return KEY_F11;
        case 0x79:
            return KEY_KPPLUS;
        case 0x7A:
            return KEY_KP3;
        case 0x7B:
            return KEY_KPMINUS;
        case 0x7C:
            return KEY_KPASTERISK;
        case 0x7D:
            return KEY_KP9;
        case 0x7E:
            return KEY_SCROLLLOCK;
        case 0x7F:
            return KEY_NONE;
        case 0x80:
            return KEY_NONE;
        case 0x81:
            return KEY_NONE;
        case 0x82:
            return KEY_NONE;
        case 0x83:
            return KEY_F7;
        case 0x84:
            return KEY_NONE;
        case 0x85:
            return KEY_NONE;
        case 0x86:
            return KEY_NONE;
        case 0x87:
            return KEY_NONE;
        case 0x88:
            return KEY_NONE;
        case 0x89:
            return KEY_NONE;
        case 0x8A:
            return KEY_NONE;
        case 0x8B:
            return KEY_NONE;
        case 0x8C:
            return KEY_NONE;
        case 0x8D:
            return KEY_NONE;
        case 0x8E:
            return KEY_NONE;
        case 0x8F:
            return KEY_NONE;

        case 0xE0:
            *scancodeBuffer = code;
            return KEY_NONE;
        case 0xE1:
            *scancodeBuffer = code;
            return KEY_NONE;
        case 0xF0:
            *scancodeBuffer = code;
            return KEY_NONE;
        default:
            return KEY_NONE;
        }
    // Multiple bytes
    case 0xE0:
        switch (code)
        {
        case 0xE010:
            return KEY_MEDIA_WWW;
        case 0xE011:
            return KEY_RIGHTALT;
        case 0xE014:
            return KEY_RIGHTCTRL;
        case 0xE015:
            return KEY_MEDIA_PREVIOUSSONG;
        case 0xE018:
            return KEY_NONE; // KEY_MEDIA_FAVORITES;
        case 0xE01F:
            return KEY_LEFTMETA;
        case 0xE020:
            return KEY_MEDIA_REFRESH;
        case 0xE021:
            return KEY_MEDIA_VOLUMEDOWN;
        case 0xE023:
            return KEY_MEDIA_MUTE;
        case 0xE027:
            return KEY_RIGHTMETA;
        case 0xE028:
            return KEY_MEDIA_STOP;
        case 0xE02B:
            return KEY_MEDIA_CALC;
        case 0xE02F:
            return KEY_COMPOSE;
        case 0xE030:
            return KEY_MEDIA_FORWARD;
        case 0xE032:
            return KEY_MEDIA_VOLUMEUP;
        case 0xE034:
            return KEY_MEDIA_PLAYPAUSE;
        case 0xE037:
            return KEY_POWER;
        case 0xE038:
            return KEY_MEDIA_BACK;
        case 0xE03A:
            return KEY_NONE; // KEY_MEDIA_HOME;
        case 0xE03B:
            return KEY_MEDIA_STOPCD;
        case 0xE03F:
            return KEY_MEDIA_SLEEP;
        case 0xE040:
            return KEY_NONE; // KEY_MEDIA_MYCOMPUTER;
        case 0xE048:
            return KEY_NONE; // KEY_MEDIA_EMAIL
        case 0xE04A:
            return KEY_KPSLASH;
        case 0xE04D:
            return KEY_MEDIA_NEXTSONG;
        case 0xE050:
            return KEY_NONE; // KEY_MEDIA_SELECT;
        case 0xE05A:
            return KEY_KPENTER;
        case 0xE05E:
            return KEY_MEDIA_COFFEE; // KEY_MEDIA_WAKE
        case 0xE069:
            return KEY_END;
        case 0xE06B:
            return KEY_LEFT;
        case 0xE06C:
            return KEY_HOME;
        case 0xE070:
            return KEY_INSERT;
        case 0xE071:
            return KEY_DELETE;
        case 0xE072:
            return KEY_DOWN;
        case 0xE074:
            return KEY_RIGHT;
        case 0xE075:
            return KEY_UP;
        case 0xE07A:
            return KEY_PAGEDOWN;
        case 0xE07D:
            return KEY_PAGEUP;
        case 0xE0F0:
        case 0xE07C:
        case 0xE012:
            *scancodeBuffer = code;
        default:
            return KEY_NONE;
        }
    case 0xE0F0:
        *scancodeBuffer = (code & 0xff) | 0xE000;
        return -ps2Set2ToUsb(scancodeBuffer);
    case 0xF0:
        *scancodeBuffer = code & 0xff;
        return -ps2Set2ToUsb(scancodeBuffer);
    default:
        switch (code)
        {
        case 0xE012E07C:
            return KEY_SYSRQ;
        case 0xE07CE0F012:
            return -KEY_SYSRQ;
        case 0xE11477E1F014F077:
            return KEY_PAUSE;
        case 0xE012E0:
        case 0xE07CE0:
        case 0xE07CE0F0:
        case 0xE114:
        case 0xE11477:
        case 0xE11477E1:
        case 0xE11477E1F0:
        case 0xE11477E1F014:
        case 0xE11477E1F014F0:
            *scancodeBuffer = code;
        default:
            return KEY_NONE;
        }
    }
}