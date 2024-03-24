#include <Windows.h>
#include <iostream>
#include <thread>
#include <fmt/core.h>
void MouseScroll(double RY) {
    using namespace std::chrono_literals;
    INPUT Input = {0};
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_WHEEL;
    Input.mi.mouseData = RY * 120;
    Input.mi.time = 0;
    SendInput(1, &Input, sizeof(Input));
    ::SendInput(1, &Input, sizeof(INPUT));
}
auto main() -> int {
    using namespace std::chrono_literals;
    POINT p;
    while (GetCursorPos(&p)) {
        std::this_thread::sleep_for(1s);
        std::cout << p.x << " " << p.y << "\n";
    }
}