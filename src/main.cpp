#include <iostream>
#include <thread>
#include <chrono>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstdlib>
#include <csignal>
#include "framebuffer.h"
#include "input.h"
#include "draw.h"

void cleanup() {
    std::cout << "\033[?25h" << std::flush;
}

unsigned int ms = 33;

void waitinput() {
    while (true) {
        if (getch() == 'q')
            std::exit(0);
        else if (getch() == '>' && ms > 5)
            ms = ms - 5;
        else if (getch() == '<')
            ms = ms + 5;
    }
}

int main(int argc, char *argv[]) {

    std::atexit(cleanup);

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int cols = w.ws_col * 2;
    int rows = w.ws_row * 4;
    
    Framebuffer fb(cols, rows);
    std::string s = "hello world!";
    Drawer d(fb);

    //fb.BufferText(0, 2, "hi");

    std::cout << "\033[?25l";

    std::thread input(waitinput);

    for (int i = 0;;i++) {
    fb.Refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    fb.Clear();
    d.DrawBox(20, 20, 60, 60);
    if (i > 40)
        i = 0;
    if (i <= 40) {
        d.DrawLine(20, 60-i, 60, 20+i);
        d.DrawLine(20+i, 20, 60-i, 60);
    }

    //d.PrintStr(s.c_str(), 0, 0);
    fb.BitmapToBraille();
    }
}
