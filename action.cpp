#include "action.h"

void LeftClick() {
    using namespace std::chrono_literals;
    INPUT Input = {0};
    // left down
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    ::SendInput(1, &Input, sizeof(INPUT));
    std::this_thread::sleep_for(50ms);
    // left up
    ::ZeroMemory(&Input, sizeof(INPUT));
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    ::SendInput(1, &Input, sizeof(INPUT));
}

void LeftUp() {
    using namespace std::chrono_literals;
    INPUT Input = {0};
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    ::SendInput(1, &Input, sizeof(INPUT));
    std::this_thread::sleep_for(50ms);
}

void LeftDown() {
    using namespace std::chrono_literals;
    INPUT Input = {0};
    // left down
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    ::SendInput(1, &Input, sizeof(INPUT));
    std::this_thread::sleep_for(50ms);
}

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

void openLink(std::string link) {
    int t = 0;
    char *linkChar = new char[link.length() + 1];
    //std::copy(link.begin(), link.end(), linkChar);
    while (t < link.length()) {
        linkChar[t] = link[t];
        //fmt::println("{} == {}",linkChar[t],link[t]);
        t++;
    }
    fmt::println("{} == {}", linkChar, link);
    ShellExecute(NULL, NULL, linkChar, NULL, NULL, SW_SHOWNORMAL);
}

void openProgram(std::string app) {
    if (app == "danser") {
        app = "D:\\Users\\User\\Desktop\\Files\\danser-0.9.0-win\\danser.exe";
    } else if (app == "chrome") {
        app = "C:\\Program Files\\Google\\Chrome\\Application\\chrome.exe";
    } else if (app == "premiere") {
        app = "F:\\Users\\User\\Videos\\trying to yt\\OBC!Weekly\\auto\\premiere.prproj";
    } else {
        app = "G:\\osu!\\osu!.exe";
    }
    char *pathChar = new char[app.length() + 1];
    std::copy(app.begin(), app.end(), pathChar);
    ShellExecute(NULL, "open", pathChar, NULL, NULL, SW_SHOWDEFAULT);
}

void openExplorer(std::string pathToFoler) {
    using namespace std::chrono_literals;
    int h = 0;
    char *pathFChar = new char[pathToFoler.length() + 1];
    while (h < pathToFoler.length()) {
        pathFChar[h] = pathToFoler[h];
        h++;
    }
    //fmt::println("{}",pathFChar);
    ShellExecute(NULL, "explore", pathFChar, 0, 0, SW_MAXIMIZE);

    fmt::println("Setting focus to explorer");
    std::this_thread::sleep_for(10ms);
    HWND window = FindWindow(NULL, "videos");
    if (window) {
        SetForegroundWindow(window);
        SetActiveWindow(window);
        SetFocus(window);
    }
}

std::string getKeyCode(std::string keyI, std::vector<InputTable> table) {
    int i = 0;
    while (i < table.size()) {
        if (table[i].key == keyI) {
            return table[i].keyCode;
        }
        i++;
    }
    return "0x30";
}

void SetCursorPosExplorer(int x, int y) {
    HWND window = FindWindow(NULL, "Select replay file");
    if (window) {
        RECT rect = {0};
        GetWindowRect(window, &rect);

        SetForegroundWindow(window);
        SetActiveWindow(window);
        SetFocus(window);
        SetCursorPos(rect.right - x, rect.bottom - y);
    }
}

void SetClickExplorer(int x, int y) {
    using namespace std::chrono_literals;
    SetCursorPosExplorer(x, y);
    std::this_thread::sleep_for(10ms);
    LeftClick();
    std::this_thread::sleep_for(50ms);
}

void pressTwoKeys(std::string s, std::string s1, std::vector<InputTable> InputTableV) {
    s = getKeyCode(s, InputTableV);
    s1 = getKeyCode(s1, InputTableV);
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0;
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;

    // Press the "A" key
    ip.ki.wVk = static_cast< unsigned short >( std::strtoul(s.c_str(), NULL,
                                                            0)); // virtual-key code for the "a" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    ip.ki.wVk = static_cast< unsigned short >( std::strtoul(s1.c_str(), NULL,
                                                            0)); // virtual-key code for the "a" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "A" key
    ip.ki.wVk = static_cast< unsigned short >( std::strtoul(s.c_str(), NULL, 0));
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    ip.ki.wVk = static_cast< unsigned short >( std::strtoul(s1.c_str(), NULL, 0));
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
}

void pressKey(std::string s, std::vector<InputTable> InputTableV) {
    s = getKeyCode(s, InputTableV);
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0;
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;

    // Press the "A" key
    ip.ki.wVk = static_cast< unsigned short >( std::strtoul(s.c_str(), NULL,
                                                            0)); // virtual-key code for the "a" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "A" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
}

void typeInStringPP(std::string s, std::vector<InputTable> InputTableV) {
    using namespace std::chrono_literals;
    int i = 0;
    auto c = std::string();
    while (i < s.size()) {
        if (s[i] == ' ') {
            c = "SPACEBAR";
        } else {
            c = s[i];
        }
        pressKey(c, InputTableV);
        std::this_thread::sleep_for(5ms);
        i++;
    }
}

void ExplorerFocusTextField() {
    SetClickExplorer(248, 58);
}

std::vector<std::string> sortOBC(std::vector<std::string> v) {
    std::ranges::sort(v, [](std::string a, std::string b) {
        //fmt::println("{} {} - {} {}",int(static_cast<unsigned char>(a[58])),a[58],int(static_cast<unsigned char>(b[58])),b[58]);
        if (int(static_cast<unsigned char>(a[58])) < int(static_cast<unsigned char>(b[58]))

            || (int(static_cast<unsigned char>(a[58])) == int(static_cast<unsigned char>(b[58]))
                && (int(static_cast<unsigned char>(a[59])) < int(static_cast<unsigned char>(b[59])))

                || ((int(static_cast<unsigned char>(a[59])) == int(static_cast<unsigned char>(b[59]))
                     && int(static_cast<unsigned char>(a[60])) < int(static_cast<unsigned char>(b[60])))
                )
            )
                ) {
            return true;
        } else {
            return false;
        }
    });
    return v;
}

void moveMaps(std::filesystem::path pathD, std::filesystem::path pathOsuSongs, int removeMode) {
    namespace fs = std::filesystem;
    auto maps = std::vector<std::string>();
    int i = 0;
    int ii;
    for (const auto &entry: fs::directory_iterator(pathD)) {
        if (std::regex_match(entry.path().generic_string(),
                             std::regex(".*\\.osz"))) {
            maps.push_back(entry.path().generic_string());
        }
    }

    while (i < maps.size()) {
        ii = maps[i].size();
        while (ii>0){
            if(maps[i][ii] == '/'){
                break;
            }
            ii--;
        }
        fmt::println("{}",pathOsuSongs.generic_string().append(maps[i].substr(ii)));
        fs::copy_file(maps[i], pathOsuSongs.generic_string().append(maps[i].substr(ii)));
        if (removeMode == 1) {
            fs::remove(maps[i]);
        }
        i++;
    }
}

void removeAllFilesInFolder(std::filesystem::path folder) {
    auto fileToRemove = std::vector<std::string>();
    namespace fs = std::filesystem;
    for (const auto &entry: fs::directory_iterator(folder)) {
        fileToRemove.push_back(entry.path().generic_string());
    }
    int p = 0;
    while (p < fileToRemove.size()) {
        fs::remove(fileToRemove[p]);
        p++;
    }
}

std::vector<std::string> loadCfg() {
    namespace fs = std::filesystem;
    auto cfg = std::fstream("reqfiles/weeklyConfig.txt");
    auto bufCfg = std::string();
    auto parsedCfg = std::vector<std::string>();
    int var = 0;

    while (var < 6) {
        std::getline(cfg, bufCfg, '"');
        std::getline(cfg, bufCfg, '"');
        parsedCfg.push_back(bufCfg);
        fmt::println("{} ---- {}", var, bufCfg);
        var++;
    }

    while (var < 10) {
        std::getline(cfg, bufCfg, '=');
        std::getline(cfg, bufCfg, '\n');
        parsedCfg.push_back(bufCfg);
        //fmt::println("{} ---- {}", var, bufCfg);
        var++;
    }
    return parsedCfg;
}

bool isCursorOnButton(sf::Window &window, sf::RectangleShape &button1) {
    return (sf::Mouse::getPosition(window).x - (button1.getPosition().x) <=
            button1.getSize().x) &&
           (sf::Mouse::getPosition(window).x - (button1.getPosition().x + button1.getSize().x) >=
            button1.getSize().x * -1) &&
           (sf::Mouse::getPosition(window).y - (button1.getPosition().y) <=
            button1.getSize().y) &&
           (sf::Mouse::getPosition(window).y - (button1.getPosition().y + button1.getSize().y) >=
            button1.getSize().y * -1);
}

std::vector<InputTable> ParseInputTable() {
    namespace fs = std::filesystem;
    auto pathToTable = fs::path("reqfiles/inputTable.txt");
    auto file = std::fstream(pathToTable);
    auto buf = std::string();
    auto bufN = std::string();
    int i = 0;
    auto vector = std::vector<InputTable>();
    while (buf != "STOP") {
        std::getline(file, buf, ' ');
        //fmt::print("{}",buf);
        if (buf == "STOP") {
            break;
        }
        std::getline(file, bufN, ' ');
        //fmt::println("-{}",bufN);
        vector.push_back(InputTable());
        vector[i].keyCode = buf;
        vector[i].key = bufN;
        std::getline(file, buf, '\n');
        i++;
    }
    return vector;
};
