#include <Windows.h>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <ranges>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <vector>
#include <thread>
#include <regex>
#include <string>
#include <iostream>
#include <sstream>
#include <stdint.h>
#include <codecvt>
#include <locale>
//#include "cmake-build-debug/_deps/mediainfolib-src/Source/MediaInfo/MediaInfo.h"
//Before running the code, don't forget to move inputTable.txt to cmake-build-debug, add STOP in the end of csv and properly set paths in main() and openProgram()
//Also, adjust locale for IpWindowName in OpenFocusChrome()

namespace ReplayParser {
// https://github.com/qtkite/Osu-Replay-Parser
// https://osu.ppy.sh/help/wiki/osu!_File_Formats/Osr_(file_format)

    struct ReplayFormat {
        unsigned char m_chGameMode = 0;
        int m_iGameVersion = 0;
        std::string m_strMapMD5Hash;
        std::string m_strPlayerName;
        std::string m_strReplayHash;
        short m_sh300;
        short m_sh100;
        short m_sh50;
        short m_shGekis;
        short m_shKatus;
        short m_shMisses;
        int m_iTotalScore;
        short m_shHighestCombo;
        unsigned char m_chFC;
        int m_iMods;
        std::string m_strLifeBar;
        long long m_lTimeStamp;
        int m_iLenCompressedReplay;
        std::vector<unsigned char> compressed_data;
        long long unknown;
    };

/// Utility function to decode a ULEB128 value.
    static inline uint64_t decodeULEB128(const uint8_t *p, unsigned *n = 0) {
        const uint8_t *orig_p = p;
        uint64_t Value = 0;
        unsigned Shift = 0;
        do {
            Value += (*p & 0x7f) << Shift;
            Shift += 7;
        } while (*p++ >= 128);
        if (n)
            *n = (unsigned) (p - orig_p);
        return Value;
    }

    std::wstring s2ws(const std::string &str) {
        using convert_typeX = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_typeX, wchar_t> converterX;

        return converterX.from_bytes(str);
    }

    std::string ws2s(const std::wstring &wstr) {
        using convert_typeX = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_typeX, wchar_t> converterX;

        return converterX.to_bytes(wstr);
    }

    std::string parse_osu_string(std::ifstream &infile) {
        unsigned char header_info;

        infile.read((char *) &header_info, sizeof(header_info));

        if (header_info == 0x0)
            return "no values were identified";
        else if (header_info == 0x0b) {

            uint8_t length = 0;

            infile.read((char *) &length, sizeof(length));

            unsigned ret = 0;

            auto decLen = decodeULEB128(&length, &ret);

            std::vector<wchar_t> utf8str;

            for (auto i = 0; i < decLen; i++) {

                unsigned char utfbyte;
                infile.read((char *) &utfbyte, sizeof(utfbyte));
                utf8str.push_back(utfbyte);
            }

            std::wstring wutfstr;

            for (auto utfpart: utf8str) {
                wutfstr += utfpart;
            }
            auto stringfromwide = ws2s(wutfstr);
            return stringfromwide;
        }
        return "";
    }

    enum Mods {
        None = 0,
        NoFail = 1,
        Easy = 2,
        TouchDevice = 4,
        Hidden = 8,
        HardRock = 16,
        SuddenDeath = 32,
        DoubleTime = 64,
        Relax = 128,
        HalfTime = 256,
        Nightcore = 512, // Only set along with DoubleTime. i.e: NC only gives 576
        Flashlight = 1024,
        Autoplay = 2048,
        SpunOut = 4096,
        Relax2 = 8192,    // Autopilot
        Perfect = 16384, // Only set along with SuddenDeath. i.e: PF only gives 16416
        Key4 = 32768,
        Key5 = 65536,
        Key6 = 131072,
        Key7 = 262144,
        Key8 = 524288,
        FadeIn = 1048576,
        Random = 2097152,
        Cinema = 4194304,
        Target = 8388608,
        Key9 = 16777216,
        KeyCoop = 33554432,
        Key1 = 67108864,
        Key3 = 134217728,
        Key2 = 268435456,
        ScoreV2 = 536870912,
        LastMod = 1073741824,
        KeyMod = Key1 | Key2 | Key3 | Key4 | Key5 | Key6 | Key7 | Key8 | Key9 | KeyCoop,
        FreeModAllowed =
        NoFail | Easy | Hidden | HardRock | SuddenDeath | Flashlight | FadeIn | Relax | Relax2 | SpunOut | KeyMod,
        ScoreIncreaseMods = Hidden | HardRock | DoubleTime | Flashlight | FadeIn
    };

    void print_mods(int mods) {

        if (mods & Mods::NoFail)
            std::cout << "No fail on" << std::endl;
        if (mods & Mods::Easy)
            std::cout << "Easy on" << std::endl;
        if (mods & Mods::TouchDevice)
            std::cout << "TouchDevice" << std::endl;
        if (mods & Mods::Hidden)
            std::cout << "Hidden on" << std::endl;
        if (mods & Mods::HardRock)
            std::cout << "HardRock on" << std::endl;
        if (mods & Mods::SuddenDeath)
            std::cout << "SuddenDeath on" << std::endl;
        if (mods & Mods::DoubleTime)
            std::cout << "DoubleTime on" << std::endl;
        if (mods & Mods::Relax)
            std::cout << "Relax on" << std::endl;
        if (mods & Mods::Nightcore)
            std::cout << "Nightcore on" << std::endl;
        if (mods & Mods::Flashlight)
            std::cout << "Flashlight on" << std::endl;
        if (mods & Mods::Autoplay)
            std::cout << "Autoplay on" << std::endl;
        if (mods & Mods::SpunOut)
            std::cout << "SpunOut on" << std::endl;
        if (mods & Mods::Relax2)
            std::cout << "Relax2 on" << std::endl;
        if (mods & Mods::Perfect)
            std::cout << "Perfect on" << std::endl;

    }

    auto parseReplay(std::filesystem::path pathD, double pp) {
        auto s = pathD.generic_string();
        s.append("/");
        s.append(std::to_string(pp));
        s.append(".osr");

        ReplayFormat replay_data;

        std::stringstream configfile;
        configfile << s;

        std::ifstream infile;

        fmt::println("Opening: {}", configfile.str().c_str());
        //std::cout << "Opening: " << configfile.str().c_str() << std::endl;

        infile.open(configfile.str().c_str(), std::ios::binary);

        if (infile.is_open()) {
            std::cout << "Successfully opened file" << std::endl;

            infile.read((char *) &replay_data.m_chGameMode, sizeof(replay_data.m_chGameMode));
            std::cout << "m_chGameMode:" << replay_data.m_chGameMode << std::endl;

            infile.read((char *) &replay_data.m_iGameVersion, sizeof(replay_data.m_iGameVersion));
            std::cout << "m_iGameVersion:" << replay_data.m_iGameVersion << std::endl;

            replay_data.m_strMapMD5Hash = parse_osu_string(infile);
            std::cout << "m_strMapMD5Hash:" << replay_data.m_strMapMD5Hash << std::endl;

            replay_data.m_strPlayerName = parse_osu_string(infile);
            std::cout << "m_strPlayerName:" << replay_data.m_strPlayerName << std::endl;

            replay_data.m_strReplayHash = parse_osu_string(infile);
            std::cout << "m_strReplayHash:" << replay_data.m_strReplayHash << std::endl;

            infile.read((char *) &replay_data.m_sh300, sizeof(replay_data.m_sh300));
            std::cout << "m_sh300:" << replay_data.m_sh300 << std::endl;

            infile.read((char *) &replay_data.m_sh100, sizeof(replay_data.m_sh100));
            std::cout << "m_sh100:" << replay_data.m_sh100 << std::endl;

            infile.read((char *) &replay_data.m_sh50, sizeof(replay_data.m_sh50));
            std::cout << "m_sh50:" << replay_data.m_sh50 << std::endl;

            infile.read((char *) &replay_data.m_shGekis, sizeof(replay_data.m_shGekis));
            std::cout << "m_shGekis:" << replay_data.m_shGekis << std::endl;

            infile.read((char *) &replay_data.m_shKatus, sizeof(replay_data.m_shKatus));
            std::cout << "m_shKatus:" << replay_data.m_shKatus << std::endl;

            infile.read((char *) &replay_data.m_shMisses, sizeof(replay_data.m_shMisses));
            std::cout << "m_shMisses:" << replay_data.m_shMisses << std::endl;

            infile.read((char *) &replay_data.m_iTotalScore, sizeof(replay_data.m_iTotalScore));
            std::cout << "m_iTotalScore:" << replay_data.m_iTotalScore << std::endl;

            infile.read((char *) &replay_data.m_shHighestCombo, sizeof(replay_data.m_shHighestCombo));
            std::cout << "m_shHighestCombo:" << replay_data.m_shHighestCombo << std::endl;

            infile.read((char *) &replay_data.m_chFC, sizeof(replay_data.m_chFC));
            std::cout << "m_chFC:" << replay_data.m_chFC << std::endl;

            infile.read((char *) &replay_data.m_iMods, sizeof(replay_data.m_iMods));
            std::cout << "m_iMods:" << replay_data.m_iMods << std::endl;

            replay_data.m_strLifeBar = parse_osu_string(infile);
            std::cout << "m_strLifeBar:" << replay_data.m_strLifeBar << std::endl;

            infile.read((char *) &replay_data.m_lTimeStamp, sizeof(replay_data.m_lTimeStamp));
            std::cout << "m_lTimeStamp:" << replay_data.m_lTimeStamp << std::endl;

            infile.read((char *) &replay_data.m_iLenCompressedReplay, sizeof(replay_data.m_iLenCompressedReplay));
            std::cout << "m_iLenCompressedReplay:" << replay_data.m_iLenCompressedReplay << std::endl;

            for (auto i = 0; i < replay_data.m_iLenCompressedReplay; i++) {

                unsigned char byte_part = 0x0;
                infile.read((char *) &byte_part, sizeof(byte_part));
                replay_data.compressed_data.push_back(byte_part);
            }

        }
        infile.close();

        //print_mods(replay_data.m_iMods);

        std::cout << "Finished parsing" << std::endl;

        return replay_data.m_iLenCompressedReplay;
    }
}

namespace YAUIO {

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

    /*int getVideoDuration(std::string fileName){
        using namespace MediaInfoLib;
        MediaInfo MI;
        MI.Open(fileName);
        //int duration = std::stoi(MI.Get(Stream_General, 0, "Duration"));
        MI.Close();
        return 0; //duration;
    }*/

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

    void LeftUp(){
        using namespace std::chrono_literals;
        INPUT Input = {0};
        Input.type = INPUT_MOUSE;
        Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
        ::SendInput(1, &Input, sizeof(INPUT));
        std::this_thread::sleep_for(50ms);
    }

    void LeftDown(){
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
            if(s[i]==' '){
                c = "SPACEBAR";
            }else{
                c = s[i];
            }
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
            } else if (arg[i] == 'A' || arg[i] == 'I' || arg[i] == '"') {
                return false;
            } else {
                i++;
            }
        }
        return true;
    }

    std::vector<Score> parseScores(auto path) {
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
                    fmt::print("6 ----- {}, {} check", debugStr[check - 2],check);
                    fmt::print("\n5 ----- {}", debugStr[check - 1]);
                    fmt::print("\n4 ----- {}\n", debugStr[check]);
                    scores[scoreN].map = debugStr[check - 2];
                    scores[scoreN].diff = debugStr[check - 1];
                    scores[scoreN].mods = debugStr[check];
                } else {
                    std::getline(file, argument, ',');
                }
                //fmt::println("{} ---- {}",i,argument);
                if (i == 11) {
                    if (argument == "STOP") {
                        //fmt::println("WTF");
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
                    fmt::println("{} total pp: /{}/",i,argument);
                    scores[scoreN].total_pp = std::stod(argument);
                }
                if(i==12) {
                    break;
                }
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

    void openExplorer(std::string pathToFoler){
        using namespace std::chrono_literals;
        int h = 0;
        char *pathFChar = new char[pathToFoler.length() + 1];
        while (h < pathToFoler.length()) {
            pathFChar[h] = pathToFoler[h];
            h++;
        }
        //fmt::println("{}",pathFChar);
        ShellExecute(NULL,"explore", pathFChar, 0,0,SW_MAXIMIZE);

        fmt::println("Setting focus to explorer");
        std::this_thread::sleep_for(10ms);
        HWND window = FindWindow(NULL, "videos");
        if (window) {
            SetForegroundWindow(window);
            SetActiveWindow(window);
            SetFocus(window);
        }
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

    void renameFiles(std::vector<Score> vector, int qScores, std::filesystem::path path) {
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
        while (i < qScores) {
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
        while (i < qScores) {
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
            if (a.pp < b.pp) {
                return true;
            } else {
                return false;
            }
        });
        return weeklyScores;
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

        std::this_thread::sleep_for(2s);
        SetCursorPos(900, 500);
        openLink("https://twitter.com");
        std::this_thread::sleep_for(2s);
        pressKey("F11", InputTableV);
        pressTwoKeys("CTRL", "W", InputTableV);
        while (i < cycle) {
            if (mode == 0 || mode == 2) {
                openLink(weeklyScores[i].scoreLink);
                SetCursorPos(1284, 440); //Replay download
                std::this_thread::sleep_for(2s);
                LeftClick();
                std::this_thread::sleep_for(1s);
            }
            if (mode == 1 || mode == 2) { //Map download
                SetCursorPos(513, 168);
                std::this_thread::sleep_for(10ms);
                LeftClick();
                std::this_thread::sleep_for(2s);
                SetCursorPos(606, 522);
                std::this_thread::sleep_for(10ms);
                LeftClick();
                std::this_thread::sleep_for(2s);
            }
            pressTwoKeys("CTRL", "W", InputTableV);
            i++;
        }
        pressKey("F11", InputTableV);
        std::this_thread::sleep_for(4s);
        pressTwoKeys("ALT", "F4", InputTableV);
        std::this_thread::sleep_for(20ms);
    }

    std::vector<std::string> getOBCConfNames(std::filesystem::path path) {
        auto pathM = std::vector<std::string>();
        namespace fs = std::filesystem;
        for (const auto &entry: fs::directory_iterator(path)) {
            if (std::regex_match(entry.path().generic_string(), std::regex(".*obc.*"))) {
                pathM.push_back(entry.path().generic_string());
            }
        }
        return pathM;
    }

    void setProperDancerState(int mode) {
        using namespace std::chrono_literals;
        if (mode == 1) { //mode 0 is just for rendering, 1 - for opening
            openProgram("danser");
            fmt::println("Waiting 2 mins for danser to load properly...");
            std::this_thread::sleep_for(120s);
        }

        //Set proper danser state
        SetClickDanser(694, 497); //mode
        SetClickDanser(682, 409); //watch a replay
        SetClickDanser(760, 38); //watch
        SetClickDanser(753, 95); //record
    }

    void renderReplays(std::vector<Score> weeklyScores, int qScores, std::vector<InputTable> InputTableV,
                       std::filesystem::path pathD, std::filesystem::path pathConf, std::filesystem::path pathV,
                       int removeMode) {
        using namespace std::chrono_literals;
        bool isBegChecked;
        bool rendered;
        double lastPart;
        double multiplier;
        int i = 0;
        //fmt::println("{}",pathD.generic_string().append("/").append(std::to_string(score.pp).append(".osr")));

        while (i < qScores) {
            //Starting to configure recording
            SetClickDanser(745, 423); //select replay
            std::this_thread::sleep_for(40ms);
            ExplorerFocusTextField();
            typeInStringPP(std::to_string(weeklyScores[i].pp).append(".OSR"), InputTableV);
            SetClickExplorer(136, 36);

            std::this_thread::sleep_for(80ms);
            SetClickDanser(542, 40); //configure
            std::this_thread::sleep_for(40ms);
            SetClickDanser(388, 262); //focus text field
            std::this_thread::sleep_for(40ms);
            pressTwoKeys("CTRL", "A", InputTableV);
            pressKey("DEL", InputTableV);
            typeInStringPP(std::to_string(weeklyScores[i].pp), InputTableV);
            SetClickDanser(410, 194); //escape configure

            rendered = false;
            auto length = ReplayParser::parseReplay(pathD, weeklyScores[i].pp);
            length = length / 440; //get approximate length in seconds
            std::this_thread::sleep_for(40ms);
            if (length <= 40) {
                if (!isBegChecked) {
                    SetClickDanser(690, 166); //time/offset menu
                    SetClickDanser(582, 388); //check skip map beginning
                    SetClickDanser(422, 440); //exit menu
                }
            } else {
                if (i != 0) { multiplier = double(i) / (double(qScores)*double(qScores)); } else { multiplier = 1 / double(qScores); }
                lastPart = (24.0 + ((double(length) / 4) * multiplier)) / length;
                SetClickDanser(690, 166); //time/offset menu
                if (lastPart > 1) { lastPart = 1; }
                SetClickDanser(214 + (378 * lastPart), 328); //part select
                SetClickDanser(422, 440); //exit menu
            }
            //fmt::println("length {}\nmultiplier {}\nqScores {}\ni {}\nlastPart {}",length,multiplier,qScores,i,lastPart);

            //Config selection
            auto configs = getOBCConfNames(pathConf); //configs
            configs = sortOBC(configs);
            auto configPath = std::string();
            namespace fs = std::filesystem;
            int configNumber = 0;
            for (const auto &entry: configs) {
                if (std::regex_match(configs[configNumber], std::regex(".*obc!" + weeklyScores[i].username + ".*"))) {
                    configPath = entry;
                    break;
                }
                configPath = "NoConf";
                configNumber++;
            }
            //fmt::println("{} {}", configPath, configNumber);
            SetClickDanser(238, 466); //enter config menu
            SetCursorPosDanser(246, 366); //hover over first cfg
            configNumber++;
            if (configPath == "NoConf") {
                configNumber = 1;
            }
            if (configNumber <= configs.size()-7) {
                std::this_thread::sleep_for(40ms);
                MouseScroll(20);
                std::this_thread::sleep_for(80ms);
                MouseScroll(-0.18 * configNumber);
                std::this_thread::sleep_for(80ms);
                LeftClick();
                std::this_thread::sleep_for(80ms);
            } else {
                std::this_thread::sleep_for(40ms);
                MouseScroll(20);
                std::this_thread::sleep_for(80ms);
                MouseScroll(-0.18 * configNumber);
                std::this_thread::sleep_for(80ms);
                fmt::println("config number: {}",configNumber);
                SetClickDanser(246, 168 + (25 * (configs.size()-configNumber+1)));
                std::this_thread::sleep_for(80ms);
            }
            SetClickDanser(422, 440); //exit menu
            std::this_thread::sleep_for(80ms);

            //Render
            SetClickDanser(182, 58); //render
            while (!rendered) {
                std::this_thread::sleep_for(4s);
                for (const auto &entry: fs::directory_iterator(pathV)) {
                    if (std::regex_match(entry.path().generic_string(),
                                         std::regex(".*" + std::to_string(weeklyScores[i].pp) + "\\.mp4"))) {
                        rendered = true;
                        break;
                    }
                }
            }
            if (removeMode == 1) {
                std::filesystem::remove(
                        pathD.generic_string().append("/").append(std::to_string(weeklyScores[i].pp).append(".osr")));
            }
            i++;
        }
    }

    void moveMaps(std::filesystem::path pathD, std::filesystem::path pathOsuSongs, int removeMode) {
        namespace fs = std::filesystem;
        auto maps = std::vector<std::string>();
        int i = 0;
        for (const auto &entry: fs::directory_iterator(pathD)) {
            if (std::regex_match(entry.path().generic_string(),
                                 std::regex(".*\\.osz"))) {
                maps.push_back(entry.path().generic_string());
            }
        }
        while (i < maps.size()) {
            fs::copy_file(maps[i], pathOsuSongs.generic_string().append(maps[i].substr(30)));
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

    std::vector<std::string> loadCfg(){
        namespace fs = std::filesystem;
        auto cfg = std::fstream("weeklyConfig.txt");
        auto bufCfg = std::string();
        auto parsedCfg = std::vector<std::string>();
        int var = 0;

        while(var<6) {
            std::getline(cfg, bufCfg, '"');
            std::getline(cfg, bufCfg, '"');
            parsedCfg.push_back(bufCfg);
            fmt::println("{} ---- {}", var, bufCfg);
            var++;
        }

        while(var<10){
            std::getline(cfg, bufCfg, '=');
            std::getline(cfg, bufCfg, '\n');
            parsedCfg.push_back(bufCfg);
            //fmt::println("{} ---- {}", var, bufCfg);
            var++;
        }
        return parsedCfg;
    }

    void makePremiereProject(int qReplays, std::vector<InputTable> InputTableV, std::filesystem::path pathProject, std::filesystem::path pathV, std::vector<Score> inpVec) {
        namespace fs = std::filesystem;
        using namespace std::chrono_literals;
        int premR = 0;
        int textScale;
        auto genericPauseSize = 100ms;
        std::string bufS;
        fmt::println("{}",pathProject.generic_string().append("/template/premiere.prproj"));
        fs::remove(pathProject.generic_string().append("/auto/premiere.prproj"));
        fs::copy(pathProject.generic_string().append("/template/premiere.prproj"), pathProject.generic_string().append("/auto/premiere.prproj"));

        openExplorer(pathV.generic_string());
        std::this_thread::sleep_for(2s);
        SetCursorPos(220,180);
        LeftClick();
        pressTwoKeys("CTRL","A",InputTableV);
        SetCursorPos(284,248);
        LeftDown();
        SetCursorPos(400,800);
        openProgram("premiere");
        std::this_thread::sleep_for(10s);
        SetCursorPos(294,834);
        std::this_thread::sleep_for(10s);
        LeftUp();
        SetCursorPos(1594,760);
        std::this_thread::sleep_for(genericPauseSize);
        LeftClick();
        std::this_thread::sleep_for(genericPauseSize);

        SetCursorPos(900,524);
        std::this_thread::sleep_for(genericPauseSize);
        pressKey("T",InputTableV);
        std::this_thread::sleep_for(1s);
        LeftClick();
        SetCursorPos(1446,202);
        std::this_thread::sleep_for(genericPauseSize);
        LeftClick();
        std::this_thread::sleep_for(genericPauseSize);
        SetCursorPos(1524,272); //FONT
        std::this_thread::sleep_for(genericPauseSize);
        LeftClick();
        std::this_thread::sleep_for(genericPauseSize);
        typeInStringPP("LEMON MILK BOLD",InputTableV);
        std::this_thread::sleep_for(genericPauseSize);
        pressKey("ENTER",InputTableV);
        std::this_thread::sleep_for(genericPauseSize);
        SetCursorPos(900,524);
        std::this_thread::sleep_for(genericPauseSize);
        LeftClick();
        std::this_thread::sleep_for(genericPauseSize);
        bufS = inpVec[premR].username.append(" BY");
        bufS.append(std::to_string(inpVec[premR].country_rank));
        fmt::println("Output text: {}",bufS);
        typeInStringPP(bufS,InputTableV);
        std::this_thread::sleep_for(genericPauseSize);
        SetCursorPos(940,520);
        std::this_thread::sleep_for(genericPauseSize);
        LeftClick();
        std::this_thread::sleep_for(genericPauseSize);
        pressTwoKeys("CTRL","A",InputTableV);
        std::this_thread::sleep_for(genericPauseSize);
        SetCursorPos(1640,300); //TEXT SIZE
        std::this_thread::sleep_for(genericPauseSize);
        LeftClick();
        std::this_thread::sleep_for(genericPauseSize);
        if (bufS.size()>16){
            textScale=34;
        }else{
            textScale=40;
        }
        typeInStringPP(std::to_string(textScale),InputTableV);
        pressKey("ENTER",InputTableV);
        std::this_thread::sleep_for(genericPauseSize);

        SetCursorPos(1524,330); //TEXT ALIGN
        std::this_thread::sleep_for(genericPauseSize);
        LeftClick();
        std::this_thread::sleep_for(genericPauseSize);

        SetCursorPos(1630,634); //X text
        std::this_thread::sleep_for(genericPauseSize);
        LeftClick();
        std::this_thread::sleep_for(genericPauseSize);
        typeInStringPP("1664",InputTableV);
        std::this_thread::sleep_for(genericPauseSize);
        pressKey("ENTER",InputTableV);
        std::this_thread::sleep_for(genericPauseSize);

        SetCursorPos(1674,634); //Y text
        std::this_thread::sleep_for(genericPauseSize);
        LeftClick();
        std::this_thread::sleep_for(genericPauseSize);
        typeInStringPP("1044",InputTableV);
        std::this_thread::sleep_for(genericPauseSize);
        pressKey("ENTER",InputTableV);
        std::this_thread::sleep_for(genericPauseSize);
    }

}

auto main() -> int {
    //Namespaces
    namespace fs = std::filesystem;
    using namespace std::chrono_literals;
    using namespace YAUIO;

    //Load config
    auto conf = loadCfg();

    //Paths
    auto path = fs::path(conf[0]); //csv
    auto pathD = fs::path(conf[1]); //replays
    auto pathConf = fs::path(conf[2]); //configs
    auto pathV = fs::path(conf[3]); //rendered videos
    auto pathOsuSongs = fs::path(conf[4]); // osu!/songs path
    auto pathProjects = fs::path(conf[5]); // premiereProjects path

    //Parsing data
    auto scores = parseScores(path);
    auto InputTableV = ParseInputTable();
    auto weeklyScores = sortScores(scores);

    int cycle = weeklyScores.size(); //How many scores out of 400pp+ ones you want to iterate over
    int doRemove = std::stoi(conf[6]); //0 - not remove replay&map file after rendering, 1 - remove
    int openDanser = std::stoi(conf[7]); //0 - just config danser, 1 - open and configure
    int downloadMode = std::stoi(conf[8]); //mode 0 - only replays ,1 - only maps, 2 - replays + maps
    int startMode = std::stoi(conf[9]); //stage to start with - 0 downloading, 1 renaming, 2 render

    if (doRemove == 1) {
        removeAllFilesInFolder(pathV); //delete all recorded videos
        removeAllFilesInFolder(pathD); //delete all files in replay folder
    }

    std::this_thread::sleep_for(5s);
    printScoresVector(weeklyScores);
    if (startMode<1){
        downloadData(weeklyScores, InputTableV, cycle, downloadMode);
        fmt::println("\n\nDownloaded all data successfully!");
    }
    if (startMode<2){
        renameFiles(weeklyScores, cycle, pathD); //renames all replays to their pp value
        fmt::println("\n\nRenamed all data successfully!");
        moveMaps(pathD, pathOsuSongs, doRemove);
        fmt::println("\n\nMoved all data successfully!");
    }
    if (startMode<3){
        setProperDancerState(openDanser);
        fmt::println("\n\nSet dancer state successfully!");
        renderReplays(weeklyScores, cycle, InputTableV, pathD, pathConf, pathV, doRemove);
        fmt::println("\n\nAll replays rendered successfully!");
    }

    //makePremiereProject(cycle,InputTableV,pathProjects,pathV,weeklyScores);

    return 0;
}
