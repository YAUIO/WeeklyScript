#include <Windows.h>
#include <iostream>
#include <thread>
#include <fmt/core.h>

auto main() -> int {
    using namespace std::chrono_literals;
    POINT p;
    /*while (GetCursorPos(&p)) {
        std::this_thread::sleep_for(1s);
        std::cout << p.x << " " << p.y << "\n";
    }*/
}