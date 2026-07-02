#include <iostream>
#include <thread>
#include <chrono>
#include <sys/ioctl.h>
#include <unistd.h>
#include "framebuffer.h"
#include "draw.h"

int main(int argc, char *argv[]) {

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int cols = w.ws_col * 2;
    int rows = w.ws_row * 4;
    
    Framebuffer fb(rows, cols);
    std::string s = "hello world!";
    Drawer d(fb);

    d.DrawBox(20, 20, 60, 60);
    d.DrawLine(20, 60, 60, 20);
    d.DrawLine(20, 20, 60, 60);
    d.PrintStr(s.c_str(), 0, 0);
    fb.BitmapToBraille();


    fb.BufferText(0, 2, "hi");

//    for (;;) {
    fb.Refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(33));
//    }
}
