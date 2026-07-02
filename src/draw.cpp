#include <iostream>
#include <vector>
#include <string>
#include "framebuffer.h"
#include "draw.h"
#include "font.h"

void Drawer::DrawLineLow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    int16_t dx = x2 - x1;
    int16_t dy = y2 - y1;
    uint16_t yi = 1;

    if (dy < 0) {
        yi = -1;
        dy = -dy;
    }

    int16_t d = (2 * dy) - dx;
    int16_t y = y1;

    for (int x = x1; x <= x2; x++) {
        fb.PutPixel(x, y, true);
        if (d > 0) {
            y += yi;
            d += (2 * (dy - dx));
        } else {
            d += 2*dy;
        }
    }
}

void Drawer::DrawLineHigh(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    int16_t dx = x2 - x1;
    int16_t dy = y2 - y1;
    uint16_t xi = 1;

    if (dx < 0) {
        xi = -1;
        dx = -dx;
    }

    int16_t d = 2 * dx - dy;
    int16_t x = x1;

    for (int y = y1; y <= y2; y++) {
        fb.PutPixel(x, y, true);
        if (d > 0) {
            x += xi;
            d += (2 * (dx - dy));
        } else {
            d += 2*dx;
        }
    }
}


void Drawer::DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    if (abs(y2 - y1) < abs(x2 - x1)) {
        if (x1 > x2)
            DrawLineLow(x2, y2, x1, y1);
        else
            DrawLineLow(x1, y1, x2, y2);
    } else {
        if (y1 > y2)
            DrawLineHigh(x2, y2, x1, y1);
        else
            DrawLineHigh(x1, y1, x2, y2);
    }
}

void Drawer::DrawBox(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    DrawLine(x1, y1, x2, y1);
    DrawLine(x1, y1, x1, y2);
    DrawLine(x2, y1, x2, y2);
    DrawLine(x1, y2, x2, y2);
}

void Drawer::DrawChar(char c, uint32_t dx, uint32_t dy)
{
        int cx,cy = 0;
        uint8_t mask[8]={128,64,32,16,8,4,2,1};
        uint8_t *character = (uint8_t*) __font_bitmap__[c];
        for(cy=0;cy<16;cy++) {
                for(cx=0;cx<8;cx++){
                        fb.PutPixel((dx) + cx, (dy) + cy, character[cy] &mask[cx] ?true : false);
                }

        }
}



void Drawer::PrintStr(const char *str, uint32_t offx, uint32_t offy) {
static uint16_t xres = fb.GetWidth() * 2;
static uint16_t yres = fb.GetHeight() * 4;


	for (uint32_t i; *str != '\0'; i++, offx+=8, *str++) {
		//if (offx >= xres/8) {
		//	offx = 0;
		//	offy++;
		//}
		//if (offy >= yres/16) offy = 0;

		switch(*str) {
			case '\n':
				offx = -1;
				offy++;
				break;
			case '\b':
				if (offx > 0) offx = offx - 1;
				DrawChar(' ', offx, offy);
				offx--;
				break;
			default:
				DrawChar(*str, offx, offy);
		}
	}
}

Drawer::Drawer(Framebuffer& myfb) : fb(myfb) {
}
