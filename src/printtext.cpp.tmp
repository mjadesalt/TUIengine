#include <iostream>
#include "framebuffer.h"
#include "draw.h"

int main(int argc, char *argv[]) {
    
    Framebuffer fb(200, 100);
    

    Drawer d(fb);
    std::string s;

//    d.DrawBox(20, 20, 60, 60);
//    d.DrawLine(20, 60, 60, 20);
//    d.DrawLine(20, 20, 60, 60);
    for (int i = 0; std::getline(std::cin, s); i++) {
        d.PrintStr(s.c_str(), 0, i * 16); 
        fb.BitmapToBraille();
        fb.Refresh();
    }
}
