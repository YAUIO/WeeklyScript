#include <Windows.h>
#include <fmt/core.h>
#include <ranges>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <vector>
#include <thread>
#include <regex>
#include <cmath>

//Before running the code, don't forget to move inputTable.txt to cmake-build-debug, add STOP in the end of csv and properly set paths in main() and openProgram()
//Also, adjust locale for IpWindowName in OpenFocusChrome()

class Score {
public:
    std::string username;
    double pp;
    std::string dateTime;
    bool replay;
    std::string scoreLink;
    std::string map;
    std::string diff;
    std::string mods;
    int country_rank;
    int global_rank;
    double total_pp;
};

class InputTable {
public:
    std::string keyCode;
    std::string key;
};

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

std::vector<InputTable> ParseInputTable() {
    namespace fs = std::filesystem;
    auto pathToTable = fs::path("inputTable.txt");
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

void pressTwoKeys(std::string s, std::string s1, std::vector<InputTable> InputTableV) {
    s = getKeyCode(s, InputTableV);
    s1 = getKeyCode(s1, InputTableV);
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0;
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;

    // Press the "A" key
    ip.ki.wVk = static_cast< unsigned short >( std::strtoul(s.c_str(), NULL, 0)); // virtual-key code for the "a" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    ip.ki.wVk = static_cast< unsigned short >( std::strtoul(s1.c_str(), NULL, 0)); // virtual-key code for the "a" key
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
    ip.ki.wVk = static_cast< unsigned short >( std::strtoul(s.c_str(), NULL, 0)); // virtual-key code for the "a" key
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
        c = s[i];
        pressKey(c, InputTableV);
        std::this_thread::sleep_for(5ms);
        i++;
    }
}

bool isMods(std::string arg) {
    int i = 0;
    if (arg.size() > 6) {
        return false;
    }
    while (i < arg.size()) {
        if (arg[i] >= 'a' && arg[i] <= 'z') {
            return false;
        } else if (arg[i] >= '0' && arg[i] <= '9') {
            return false;
        } else {
            i++;
        }
    }
    return true;
}

std::vector<Score> parseScores(auto path) {
    /*auto fileInp = std::fstream(path, std::ios::out | std::ios::app);
    fmt::print(fileInp, "{}", "\nSTOP");*/
    namespace fs = std::filesystem;
    auto file = std::fstream(path);
    auto line = std::string();
    std::string debugStr[10];
    auto argument = std::string();
    auto scores = std::vector<Score>();
    int i = 11;
    int check = 0;
    int scoreN = 0;
    std::getline(file, argument);
    while (i != 12) {
        scores.push_back(Score());
        i = 11;
        while (i != 0) {
            if (i == 1) {
                std::getline(file, argument, '\n');
            } else if (i == 6) {
                std::getline(file, debugStr[0], ',');
                check = 0;
                while (!isMods(debugStr[check])) {
                    check++;
                    std::getline(file, debugStr[check], ',');
                }
                i = 3;
                /*fmt::print("6 ----- {}, {} check", debugStr[check - 2],check);
                fmt::print("\n5 ----- {}", debugStr[check - 1]);
                fmt::print("\n4 ----- {}\n", debugStr[check]);*/
                scores[scoreN].map = debugStr[check - 2];
                scores[scoreN].diff = debugStr[check - 1];
                scores[scoreN].mods = debugStr[check];
            } else {
                std::getline(file, argument, ',');
            }
            if (i == 11) {
                if (argument == "STOP") {
                    i = 12;
                    break;
                }
                scores[scoreN].username = argument;
            } else if (i == 10) {
                scores[scoreN].pp = std::stod(argument);
            } else if (i == 9) {
                scores[scoreN].dateTime = argument;
            } else if (i == 8) {
                if (argument == "true") {
                    scores[scoreN].replay = true;
                } else {
                    scores[scoreN].replay = false;
                }
            } else if (i == 7) {
                scores[scoreN].scoreLink = argument;
            } else if (i == 3) {
                if (argument.size() > 3) {
                    std::getline(file, argument, ',');
                }
                scores[scoreN].country_rank = std::stoi(argument);
            } else if (i == 2) {
                scores[scoreN].global_rank = std::stoi(argument);
            } else if (i == 1) {
                scores[scoreN].total_pp = std::stod(argument);
            }
            //fmt::println("{} ---- {}", i, argument);
            i--;
        }
        scoreN++;
        //fmt::println("\n\n");
    }
    return scores;
}

void printScoresVector(std::vector<Score> v) {
    int i = 0;
    while (i < v.size()) {
        fmt::println("Score {}:\n{}\n{}\n{}\n{}\n{}\n{}\n{}\n{}\n{}\n{}\n{}\n", i, v[i].username, v[i].pp,
                     v[i].dateTime, v[i].replay, v[i].scoreLink, v[i].map, v[i].diff, v[i].mods, v[i].country_rank,
                     v[i].global_rank, v[i].total_pp);
        i++;
    }
}

void openLink(std::string s) {
    char *linkChar = new char[s.length() + 1];
    std::copy(s.begin(), s.end(), linkChar);
    ShellExecute(NULL, NULL, linkChar, NULL, NULL, SW_SHOWNORMAL);
}

void openProgram(std::string s) {
    if (s == "danser") {
        s = "D:\\Users\\User\\Desktop\\Files\\danser-0.9.0-win\\danser.exe";
    } else if (s == "premiere") {
        s = "D:\\Soft\\Programs\\Adobe\\Adobe Premiere Pro 2020\\Adobe\\Adobe Premiere Pro 2020\\Adobe Premiere Pro.exe";
    } else if (s == "chrome") {
        s = "C:\\Program Files\\Google\\Chrome\\Application\\chrome.exe";
    } else {
        s = "G:\\osu!\\osu!.exe";
    }
    char *pathChar = new char[s.length() + 1];
    std::copy(s.begin(), s.end(), pathChar);
    ShellExecute(NULL, "open", pathChar, NULL, NULL, SW_SHOWDEFAULT);
}

void SetCursorPosDanser(int x, int y) {
    HWND window = FindWindow(NULL, "danser-go 0.9.1 launcher");
    if (window) {
        RECT rect = {0};
        GetWindowRect(window, &rect);

        SetForegroundWindow(window);
        SetActiveWindow(window);
        SetFocus(window);
        SetCursorPos(rect.right - x, rect.bottom - y);
    }
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

void SetClickDanser(int x, int y) {
    using namespace std::chrono_literals;
    SetCursorPosDanser(x, y);
    std::this_thread::sleep_for(10ms);
    LeftClick();
    std::this_thread::sleep_for(80ms);
}

void ExplorerFocusTextField() {
    SetClickExplorer(248, 58);
}

void renameFiles(std::vector<Score> vector, std::filesystem::path path) {
    int i = 0;
    namespace fs = std::filesystem;
    std::string pathM[vector.size() * 2];
    for (const auto &entry: fs::directory_iterator(path)) {
        pathM[i] = entry.path().generic_string();
        i++;
    }

    auto ids = std::vector<std::string>();
    i = 0;
    int ii = 0;
    while (i < vector.size()) {
        while (ii < vector[i].scoreLink.size()) {
            if (vector[i].scoreLink[ii] >= '0' && vector[i].scoreLink[ii] <= '9') {
                break;
            }
            ii++;
        }
        ids.push_back(vector[i].scoreLink.substr(ii, vector[i].scoreLink.size()));
        i++;
    }
    i = 0;
    ii = 0;
    auto found = std::vector<int>();
    auto matches = std::vector<std::string>();
    while (i < ids.size()) {
        for (const auto &str: pathM) {
            if (std::regex_match(str, std::regex(".*" + ids[i] + ".*"))) {
                matches.push_back(str);
                found.push_back(i);
            }
        }
        i++;
    }
    //fmt::println("{}",matches.size());
    i = 0;
    auto matchesConv = std::vector<std::string>();
    while (i < matches.size()) {
        matchesConv.push_back(matches[i]);
        i++;
    }
    i = 0;
    while (i < vector.size()) {
        fs::rename(matchesConv[i],
                   path.generic_string().append("/").append(std::to_string(vector[found[i]].pp)).append(".osr"));
        i++;
    }

}

std::vector<Score> sortScores(std::vector<Score> v) {
    auto weeklyScores = std::vector<Score>();
    int i = 0;
    while (i < v.size()) {
        if (v[i].pp > 400 && v[i].replay) {
            weeklyScores.push_back(v[i]);
        }
        i++;
    }
    std::ranges::sort(weeklyScores, [](Score a, Score b) {
        if (a.pp > b.pp) {
            return true;
        } else {
            return false;
        }
    });
    return weeklyScores;
}

void downloadData(std::vector<Score> weeklyScores, std::vector<InputTable> InputTableV, int cycle, int mode) {
    int i = 0;
    using namespace std::chrono_literals;

    openProgram("chrome");
    HWND window = FindWindow(NULL, "Новая Вкладка - Google Chrome");
    if (window) {
        SetForegroundWindow(window);
        SetActiveWindow(window);
        SetFocus(window);
    }

    while (i < cycle) {
        if (mode == 0 || mode == 2) {
            openLink(weeklyScores[i].scoreLink);
            SetCursorPos(1284, 528); //Replay download
            std::this_thread::sleep_for(2s);
            LeftClick();
            std::this_thread::sleep_for(150ms);
        }
        if (mode == 1 || mode == 2) { //Map download
            SetCursorPos(513, 278);
            std::this_thread::sleep_for(10ms);
            LeftClick();
            std::this_thread::sleep_for(2s);
            SetCursorPos(606, 595);
            std::this_thread::sleep_for(10ms);
            LeftClick();
            std::this_thread::sleep_for(2s);
        }
        pressTwoKeys("CTRL", "W", InputTableV);
        i++;
    }
}

auto main() -> int {
    //Namespaces
    namespace fs = std::filesystem;
    using namespace std::chrono_literals;

    //Path to input Scores file and Downloads folder
    auto path = fs::path("D:\\Users\\User\\Desktop\\Files\\OsuScores\\output.csv");
    auto pathD = fs::path("F:\\Users\\User\\Downloads\\Scores");
    auto pathConf = fs::path("D:\\Users\\User\\Desktop\\Files\\danser-0.9.0-win\\settings");

    //Parsing data
    auto scores = parseScores(path);
    auto InputTableV = ParseInputTable();
    auto weeklyScores = sortScores(scores);

    int i = 0;
    //How many scores out of 400pp+ ones you want to iterate over
    int cycle = weeklyScores.size();

    //printScoresVector(weeklyScores);
    //downloadData(weeklyScores, InputTableV, cycle, 0); //mode 0 - only replays ,1 - only maps, 2 - replays + maps
    //renameFiles(weeklyScores, pathD); //renames all replays to their pp value
    //openProgram("danser");
    //std::this_thread::sleep_for(300s);

    //Set proper danser state
    SetClickDanser(694, 497); //mode
    SetClickDanser(682, 409); //watch a replay
    SetClickDanser(760, 38); //watch
    SetClickDanser(753, 95); //record
    SetClickDanser(745, 423); //select replay
    std::this_thread::sleep_for(40ms);
    ExplorerFocusTextField();
    typeInStringPP(std::to_string(weeklyScores[i].pp).append(".OSR"),InputTableV);
    SetClickExplorer(136,36);
    SetClickDanser(542, 40); //configure
    std::this_thread::sleep_for(40ms);
    SetClickDanser(388, 262); //focus text field
    typeInStringPP(std::to_string(weeklyScores[i].pp),InputTableV);
    SetClickDanser(410, 194); //escape configure
    //config choice write pls!!

    SetClickDanser(182, 58);
    return 0;
}
