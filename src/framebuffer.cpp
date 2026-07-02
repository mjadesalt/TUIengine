#include <iostream>
#include <vector>
#include <string>
#include "framebuffer.h"

uint16_t Framebuffer::GetWidth() {
    return fbwidth;
}

uint16_t Framebuffer::GetHeight() {
    return fbheight;
}

void Framebuffer::Clear() {
    std::fill(fb.begin(), fb.end(), 0);    
}

void Framebuffer::PutPixel(uint16_t x, uint16_t y, bool state) {
    uint8_t bit;
    if (state)
        bit = 0b10000000;
    else
        bit = 0;
    
    fb[(y * fbwidth / 4) + (x / 8)] &= (~(0b10000000) >> (x % 8));
    fb[(y * fbwidth / 4) + (x / 8)] |= (bit >> (x % 8));
}

// swap bits a and b in a byte
uint8_t Framebuffer::SwapBits(uint8_t byte, unsigned int a, unsigned int b) {
    // get the bits
    uint8_t tmpa = (byte >> (a - 1)) & 0x01;
    uint8_t tmpb = (byte >> (b - 1)) & 0x01;

    // clear the bth bit
    byte = ~(0x01 << (b - 1)) & byte;
    // set the bth bit
    byte = byte | (tmpa << (b - 1));

    // clear the ath bit
    byte = ~(0x01 << (a - 1)) & byte;
    // set the ath bit
    byte = byte | (tmpb << (a - 1));

    return byte;
}

// translate the dots represented as bits to an offset for Unicode
uint8_t Framebuffer::ByteToBraille(uint8_t byte) {
    byte = SwapBits(byte, 6, 4);
    byte = SwapBits(byte, 7, 4);
    byte = SwapBits(byte, 6, 3);
    byte = SwapBits(byte, 7, 2);
    byte = SwapBits(byte, 8, 1);
    return byte;
}

// Everything over which I have might that cannot be torn from me remains my property!
// Translates a codepoint to a Unicode character.
std::string Framebuffer::UnicodeToUTF8(unsigned int codepoint)
{
    std::string out;

    if (codepoint <= 0x7f)
        out.append(1, static_cast<char>(codepoint));
    else if (codepoint <= 0x7ff)
    {
        out.append(1, static_cast<char>(0xc0 | ((codepoint >> 6) & 0x1f)));
        out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
    }
    else if (codepoint <= 0xffff)
    {
        out.append(1, static_cast<char>(0xe0 | ((codepoint >> 12) & 0x0f)));
        out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
        out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
    }
    else
    {
        out.append(1, static_cast<char>(0xf0 | ((codepoint >> 18) & 0x07)));
        out.append(1, static_cast<char>(0x80 | ((codepoint >> 12) & 0x3f)));
        out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
        out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
    }
    return out;
}

uint8_t Framebuffer::GetByte(uint16_t x, uint16_t y) {
    uint8_t byte = 0;
    uint8_t mask = 0b11000000;
    uint8_t offset = (x % 4) * 2;

    uint8_t a = (fb[((y * 4) * fbwidth / 4) + (x / 4)] & (mask >> offset)) << offset;
    uint8_t b = (fb[((y * 4 + 1) * fbwidth / 4) + (x / 4)] & (mask >> offset)) << offset;
    uint8_t c = (fb[((y * 4 + 2) * fbwidth / 4) + (x / 4)] & (mask >> offset)) << offset;
    uint8_t d = (fb[((y * 4 + 3) * fbwidth / 4) + (x / 4)] & (mask >> offset)) << offset;

    byte = a | (b >> 2) | (c >> 4) | (d >> 6);

    return byte;
}

std::string Framebuffer::BitmapToBraille() {
    // get all the bits and append them to a buffer
    buffer = "";
    int charx = fbwidth / 2;
    int chary = fbheight / 4;
    for (int i = 0; i < chary; i++) {
        for (int j = 0; j < charx; j++) {
           buffer.append(UnicodeToUTF8(0x2800 + ByteToBraille(GetByte(j, i)))); 
        }
        if (i != chary - 1 )
            buffer.append("\n");
    }
//    for (int i = 0; i < fbwidth; i++)
//        buffer.append("-");

//    buffer.append("\n");
    return buffer;
}

void Framebuffer::BufferText(uint16_t x, uint16_t y, std::string msg) {
    uint32_t offset = (y * fbwidth * sizeof(0x2800)) + x * sizeof(0x2800);
    
    for (int i = 0; i < msg.size(); i++) {
        if (buffer[offset + i] != '\n') {
            buffer[offset + i] = msg[i];
        } else {
            buffer[offset + i + 1] = msg[i];
        }
    }
}
	
void Framebuffer::Refresh() {
    std::cout << buffer << "\033[H";
}

// we add 3 to prevent garbage data at the end with getbyte()
Framebuffer::Framebuffer(uint16_t x, uint16_t y) : fb(x * (y)), fbwidth(x), fbheight(y), buffer("") {
}
