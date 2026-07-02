#include <iostream>
#include <vector>
#include <string>

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

class Framebuffer {
private:
    uint16_t fbwidth;
    uint16_t fbheight;

	std::vector<uint8_t> fb;
    std::string buffer;
    std::string textbuffer;

public:

uint16_t GetWidth();

uint16_t GetHeight();

void PutPixel(uint16_t x, uint16_t y, bool state);

// swap bits a and b in a byte
uint8_t SwapBits(uint8_t byte, unsigned int a, unsigned int b);

// translate the dots represented as bits to an offset for Unicode
uint8_t ByteToBraille(uint8_t byte);

// Everything over which I have might that cannot be torn from me remains my property!
// Translates a codepoint to a Unicode character.
std::string UnicodeToUTF8(unsigned int codepoint);

uint8_t GetByte(uint16_t x, uint16_t y);

std::string BitmapToBraille();

void BufferText(uint16_t x, uint16_t y, std::string msg);
	
void Refresh();

Framebuffer(uint16_t x, uint16_t y);
};

#endif
