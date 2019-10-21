#include <stdint.h>

#include "portio.h"
#include "print.h"
#include "scancodes.h"

void ps2data(uint8_t data)
{
    // spinlock wait for the buffer to be empty
    while (2 & inb(0x64))
    {
    }
    outb(0x60, data);
}
void ps2command(uint8_t cmd)
{
    outb(0x64, cmd);
}
void ps2command2(uint8_t cmd, uint8_t data)
{
    ps2command(cmd);
    ps2data(data);
}

uint8_t ps2readSpin()
{
    while (!(1 & inb(0x64)))
    {
    }
    return inb(0x60);
}

void initps2()
{
    // TODO check if ps2 exists
    // TODO check if USB is in compatibility mode

    // Disable devices during initalization
    // Disable first port
    ps2command(0xad);
    // Disable second port
    ps2command(0xa7);

    // flush the buffer
    while (1 & inb(0x64))
    {
        inb(0x60);
    }
    // disable IRQ and translation in the controller configuration byte
    ps2command(0x20);
    uint8_t cc = ps2readSpin();
    cc &= 0x34;
    ps2command2(0x60, cc);
    print("PS2 Controller configuration: ");
    printhex(cc);
    println("");

    // perform self test
    ps2command(0xaa);
    uint8_t testResult = ps2readSpin();
    printhex(testResult);

    // TODO count channels
    // TODO perform interface test
    // Enable PS1 and IRQ 1 (keyboard)
    ps2command(0xae);
    // cc |= 0x01;
}

struct keyboardCommand
{
    uint8_t command;
    uint8_t data_length;
    uint8_t data[2];
};

static struct keyboardCommand kbdCommandQueue[16];
static uint8_t kbdCommandQueueBegin;
static uint8_t kbdCommandQueueEnd;

void kbdCommandQueuePush(struct keyboardCommand cmd)
{
    uint8_t nextEnd = (kbdCommandQueueEnd + 1) % 16;
    if (nextEnd == kbdCommandQueueBegin)
    {
        asm volatile("int $200");
        return;
    }
    kbdCommandQueue[kbdCommandQueueEnd] = cmd;
    kbdCommandQueueEnd = nextEnd;
}

struct keyboardCommand kbdCommandQueueFront()
{
    if (kbdCommandQueueBegin == kbdCommandQueueEnd)
    {
        asm volatile("hlt");
    }
    return kbdCommandQueue[kbdCommandQueueBegin];
}
void kbdCommandQueuePop()
{
    if (kbdCommandQueueBegin == kbdCommandQueueEnd)
    {
        asm volatile("hlt");
    }
    kbdCommandQueueBegin++;
    kbdCommandQueueBegin %= 16;
}

uint8_t kbdCommandQueueLength()
{
    return (kbdCommandQueueEnd + 16 - kbdCommandQueueBegin) % 16;
}

void processUsbCode(uint16_t usbCode) {
    printhex(usbCode);
    println("");
}

void processScancode(uint8_t scanCode) {
    static uint64_t codeBuffer = 0;
    codeBuffer <<= 8;
    codeBuffer += scanCode;
    int16_t usbCode = 0;;
    while ((usbCode = ps2Set2ToUsb(&codeBuffer))) {
        processUsbCode(usbCode);
    };
}

void keyboard_test_loop()
{
    initps2();
    for (;;)
    {
        do
        {
            // read scan codes
            while (1 & inb(0x64))
            {
                uint8_t scanCode = inb(0x60);
                processScancode(scanCode);
            }
        } while (kbdCommandQueueLength() == 0);
        struct keyboardCommand nextCommand = kbdCommandQueueFront();
        ps2data(nextCommand.command);
        for (int i = 0; i < nextCommand.data_length; i++)
        {
            ps2data(nextCommand.data[i]);
        }
        uint8_t result = ps2readSpin();
        if (result == 0xfa)
        {
            kbdCommandQueuePop();
        }
        else if (result != 0xfe)
        {
            print("unknown byte from keyboard");
            printhex(result);
            asm volatile("hlt");
            return;
        }
    }
}