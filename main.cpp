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
    INPUT Input = {0};
    // left down
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    ::SendInput(1, &Input, sizeof(INPUT));

    // left up
    ::ZeroMemory(&Input, sizeof(INPUT));
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    ::SendInput(1, &Input, sizeof(INPUT));
}

void pressTwoKeys(std::string s, std::string s1) {
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

void pressKey(std::string s) {
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

auto main() -> int {
    namespace fs = std::filesystem;
    auto path = fs::path("D:\\Users\\User\\Desktop\\Files\\OsuScores\\output.csv");
    auto pathD = fs::path("F:\\Users\\User\\Downloads\\Scores");
    int i = 0;
    /*auto scores = parseScores(path);
    i = 0;
    auto weeklyScores = sortScores(scores);
    printScoresVector(weeklyScores);
    using namespace std::chrono_literals;
    int cycle = weeklyScores.size();
    while(i<cycle){
        openLink(weeklyScores[i].scoreLink);
        SetCursorPos(1284, 528);
        std::this_thread::sleep_for(2s);
        LeftClick();
        std::this_thread::sleep_for(2s);
        SetCursorPos(513, 278);
        std::this_thread::sleep_for(10ms);
        LeftClick();
        std::this_thread::sleep_for(2s);
        SetCursorPos(606, 595);
        std::this_thread::sleep_for(10ms);
        LeftClick();
        std::this_thread::sleep_for(2s);
        i++;
    }
    renameFiles(weeklyScores, pathD);
    */
    auto InputTableV = ParseInputTable();
    pressKey(getKeyCode("W", InputTableV));
    pressTwoKeys(getKeyCode("W", InputTableV),getKeyCode("A", InputTableV));
    return 0;
}
