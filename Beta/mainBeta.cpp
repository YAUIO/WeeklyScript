#include <Windows.h>
#include <fmt/core.h>
#include <ranges>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <utility>
#include <vector>
#include <thread>
#include <regex>
#include <string>
#include <iostream>
#include <sstream>

//BETA BRANCH BETA BRANCH BETA BRANCH BETA BRANCH BETA BRANCH BETA BRANCH BETA BRANCH BETA BRANCH BETA BRANCH BETA BRANCH BETA BRANCH

//Before running the code, don't forget to move inputTable.txt to cmake-build-debug, add STOP in the end of csv and properly set paths in main() and openProgram()
//Also, adjust locale for IpWindowName in OpenFocusChrome()
//Look at 937

namespace ReplayParser {
// https://github.com/qtkite/Osu-Replay-Parser
// https://osu.ppy.sh/help/wiki/osu!_File_Formats/Osr_(file_format)

    struct ReplayFormat {
        /*unsigned char m_chGameMode = 0;
        int m_iGameVersion = 0;*/
        std::string m_strMapMD5Hash;
        std::string m_strPlayerName;
        std::string m_strReplayHash;
        /*short m_sh300{};
        short m_sh100{};
        short m_sh50{};
        short m_shGekis{};
        short m_shKatus{};
        short m_shMisses{};
        int m_iTotalScore{};
        short m_shHighestCombo{};
        unsigned char m_chFC{};
        int m_iMods{};*/
        std::string m_strLifeBar;
        //long long m_lTimeStamp{};
        int m_iLenCompressedReplay{};
        std::vector<unsigned char> compressed_data;
        //long long unknown{};
    };

/// Utility function to decode a ULEB128 value.
    /*static inline uint64_t decodeULEB128(const uint8_t *p, unsigned *n = nullptr) {
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
    }*/

    /*std::wstring s2ws(const std::string &str) {
        using convert_typeX = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_typeX, wchar_t> converterX;

        return converterX.from_bytes(str);
    }*/

    /*std::string ws2s(const std::wstring &wstr) {
        using convert_typeX = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_typeX, wchar_t> converterX;

        return converterX.to_bytes(wstr);
    }*/

    /*std::string parse_osu_string(std::ifstream &infile) {
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
    }*/

    /*enum Mods {
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
    };*/ //Mods enums

    /* void print_mods(int mods) {

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

         if (mods & Mods::None)
             std::cout << "No mods" << std::endl;
         if (mods & Mods::HalfTime)
             std::cout << "HalfTime on" << std::endl;
         if (mods & Mods::Random)
             std::cout << "Random on" << std::endl;
         if (mods & Mods::Cinema)
             std::cout << "Cinema on" << std::endl;
         if (mods & Mods::Target)
             std::cout << "Target on" << std::endl;
         if (mods & Mods::ScoreV2)
             std::cout << "SV2 on" << std::endl;
         if (mods & Mods::LastMod)
             std::cout << "LastMod on" << std::endl;
         if (mods & Mods::FreeModAllowed)
             std::cout << "FreeModAllowed on" << std::endl;
         if (mods & Mods::ScoreIncreaseMods)
             std::cout << "ScoreIncreaseMods on" << std::endl;

     }*/ //Print mods from int

    auto parseReplay(const std::filesystem::path &pathD, double pp) {
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

            /*infile.read((char *) &replay_data.m_chGameMode, sizeof(replay_data.m_chGameMode));
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
            std::cout << "m_lTimeStamp:" << replay_data.m_lTimeStamp << std::endl;*/

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
        double pp{};
        std::string dateTime;
        bool replay{};
        char *scoreLinkC{};
        std::string scoreLink;
        std::string map;
        std::string diff;
        std::string mods;
        int country_rank{};
        int global_rank{};
        double total_pp{};
    };

    class InputTable {
    public:
        std::string keyCode;
        std::string key;
    };

    std::string getKeyCode(const std::string &keyI, std::vector<InputTable> table) {
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
            vector.emplace_back();
            vector[i].keyCode = buf;
            vector[i].key = bufN;
            std::getline(file, buf, '\n');
            i++;
        }
        return vector;
    }

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

    void MouseScroll(unsigned long RY) {
        using namespace std::chrono_literals;
        INPUT Input = {0};
        Input.type = INPUT_MOUSE;
        Input.mi.dwFlags = MOUSEEVENTF_WHEEL;
        Input.mi.mouseData = RY * 120;
        Input.mi.time = 0;
        SendInput(1, &Input, sizeof(Input));
        ::SendInput(1, &Input, sizeof(INPUT));
    }

    void pressTwoKeys(std::string s, std::string s1, const std::vector<InputTable> &InputTableV) {
        s = getKeyCode(s, InputTableV);
        s1 = getKeyCode(s1, InputTableV);
        INPUT ip;
        ip.type = INPUT_KEYBOARD;
        ip.ki.wScan = 0;
        ip.ki.time = 0;
        ip.ki.dwExtraInfo = 0;

        // Press the "A" key
        ip.ki.wVk = static_cast< unsigned short >( std::strtoul(s.c_str(), nullptr,
                                                                0)); // virtual-key code for the "a" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));

        ip.ki.wVk = static_cast< unsigned short >( std::strtoul(s1.c_str(), nullptr,
                                                                0)); // virtual-key code for the "a" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));

        // Release the "A" key
        ip.ki.wVk = static_cast< unsigned short >( std::strtoul(s.c_str(), nullptr, 0));
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));

        ip.ki.wVk = static_cast< unsigned short >( std::strtoul(s1.c_str(), nullptr, 0));
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    }

    void pressKey(std::string s, std::vector<InputTable> InputTableV) {
        s = getKeyCode(s, std::move(InputTableV));
        INPUT ip;
        ip.type = INPUT_KEYBOARD;
        ip.ki.wScan = 0;
        ip.ki.time = 0;
        ip.ki.dwExtraInfo = 0;

        // Press the "A" key
        ip.ki.wVk = static_cast< unsigned short >( std::strtoul(s.c_str(), nullptr,
                                                                0)); // virtual-key code for the "a" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));

        // Release the "A" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    }

    void typeInStringPP(std::string s, const std::vector<InputTable> &InputTableV) {
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
            if ((arg[i] >= 'a' && arg[i] <= 'z') || (arg[i] >= '0' && arg[i] <= '9'))
                return false;
            else
                i++;

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
        int ii;
        int check;
        int scoreN = 0;
        std::getline(file, argument);
        while (i != 12) {
            scores.emplace_back();
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
                    ii = 0;
                    scores[scoreN].scoreLink = argument;
                    scores[scoreN].scoreLinkC = new char[argument.length() + 1];
                    while(ii<argument.size()) {
                        scores[scoreN].scoreLinkC[ii] = *reinterpret_cast<char *>(argument[ii]);
                        ii++;
                    }
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

    void openLink(char *linkChar) {
        //std::copy(link.begin(), link.end(), linkChar);
        fmt::println("{}", linkChar);
        ShellExecute(nullptr, nullptr, linkChar, nullptr, nullptr, SW_SHOWNORMAL);
    }

    void openProgram(std::string app) {
        if (app == "danser") {
            app = R"(D:\Users\User\Desktop\Files\danser-0.9.0-win\danser.exe)"; // #fix maybe put in variables?
        } else if (app == "chrome") {
            app = R"(C:\Program Files\Google\Chrome\Application\chrome.exe)";
        } else if (app == "premiere") {
            app = R"(F:\Users\User\Videos\trying to yt\OBC!Weekly\auto\premiere.prproj)";
        } else {
            app = "G:\\osu!\\osu!.exe";
        }
        char *pathChar = new char[app.length() + 1];
        std::copy(app.begin(), app.end(), pathChar);
        ShellExecute(nullptr, "open", pathChar, nullptr, nullptr, SW_SHOWDEFAULT);
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
        ShellExecute(nullptr, "explore", pathFChar, nullptr, nullptr, SW_MAXIMIZE);

        std::this_thread::sleep_for(10ms);
        HWND window = FindWindow(nullptr, "videos");
        if (window) {
            RECT rect = {0};
            GetWindowRect(window, &rect);

            SetForegroundWindow(window);
            SetActiveWindow(window);
            SetFocus(window);
        }
    }

    void SetCursorPosDanser(int x, int y) {
        HWND window = FindWindow(nullptr, "danser-go 0.9.1 launcher");
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
        HWND window = FindWindow(nullptr, "Select replay file");
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

    void renameFiles(std::vector<Score> vector, int qScores, const std::filesystem::path& path) {
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
        std::ranges::sort(weeklyScores, [](const Score& a, const Score& b) {
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
            if (int(static_cast<unsigned char>(a[58])) < int(static_cast<unsigned char>(b[58])) // #fix this char hell

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

    void downloadData(std::vector<Score> weeklyScores, const std::vector<InputTable>& InputTableV, int cycle, int mode) {
        int i = 0;
        using namespace std::chrono_literals;

        openProgram("chrome");
        HWND window = FindWindow(nullptr, "Новая Вкладка - Google Chrome");
        if (window) {
            SetForegroundWindow(window);
            SetActiveWindow(window);
            SetFocus(window);
        }

        std::this_thread::sleep_for(2s);
        SetCursorPos(900, 500);
        openLink(weeklyScores[i].scoreLinkC);
        std::this_thread::sleep_for(10ms);
        pressKey("F11", InputTableV);
        std::this_thread::sleep_for(10ms);
        pressTwoKeys("CTRL", "W", InputTableV);
        fmt::println("Download begin");
        while (i < cycle) {
            if (mode == 0 || mode == 2) {
                openLink(weeklyScores[i].scoreLinkC);
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

    std::vector<std::string> getOBCConfNames(const std::filesystem::path& path, const std::string& config_prefix) {
        auto pathM = std::vector<std::string>();
        namespace fs = std::filesystem;
        for (const auto &entry: fs::directory_iterator(path)) {
            if (std::regex_match(entry.path().generic_string(), std::regex(".*" + config_prefix + ".*"))) {
                pathM.push_back(entry.path().generic_string());
            }
        }
        return pathM;
    }

    void setProperDancerState(int mode) {
        using namespace std::chrono_literals;
        if (mode == 1) { //mode 0 is just for rendering, 1 - for opening
            openProgram("danser");
            std::this_thread::sleep_for(120s);
        }

        //Set proper danser state
        SetClickDanser(694, 497); //mode
        SetClickDanser(682, 409); //watch a replay
        SetClickDanser(760, 38); //watch
        SetClickDanser(753, 95); //record
    }

    void renderReplays(std::vector<Score> weeklyScores, const std::string& configPrefix, int qScores,
                       const std::vector<InputTable>& InputTableV,
                       const std::filesystem::path& pathD, const std::filesystem::path& pathConf, const std::filesystem::path& pathV,
                       int removeMode) {
        using namespace std::chrono_literals;
        bool isBegChecked;
        bool rendered;
        double lastPart;
        double multiplier;
        int i = 0;
        auto length = std::vector<int>();
        while (i < qScores) { //Parse lengths
            length.push_back(ReplayParser::parseReplay(pathD, weeklyScores[i].pp));
            i++;
        }
        i = 0;
        //fmt::println("{}",pathD.generic_string().append("/").append(std::to_string(score.pp).append(".osr")));

        while (i < qScores) {
            //Starting to configure recording
            SetClickDanser(745, 423); //select replay
            std::this_thread::sleep_for(40ms);
            ExplorerFocusTextField();
            typeInStringPP(std::to_string(weeklyScores[i].pp).append(".OSR"), InputTableV);
            SetClickExplorer(136, 36);

            SetClickDanser(542, 40); //configure
            std::this_thread::sleep_for(40ms);
            SetClickDanser(388, 262); //focus text field
            std::this_thread::sleep_for(40ms);
            pressTwoKeys("CTRL", "A", InputTableV);
            pressKey("DEL", InputTableV);
            typeInStringPP(std::to_string(weeklyScores[i].pp), InputTableV);
            SetClickDanser(410, 194); //escape configure

            rendered = false;

            length[i] = length[i] / 440; //get approximate length in seconds
            std::this_thread::sleep_for(40ms);
            if (length[i] <= 40) {
                if (!isBegChecked) {
                    SetClickDanser(690, 166); //time/offset menu
                    SetClickDanser(582, 388); //check skip map beginning
                    SetClickDanser(422, 440); //exit menu
                }
            } else {
                if (i != 0) { multiplier = double(i) / double(qScores); } else { multiplier = 1 / double(qScores); }
                lastPart = (40.0 + ((double(length[i]) / 4) * multiplier)) / length[i];
                SetClickDanser(690, 166); //time/offset menu
                if (lastPart > 1) { lastPart = 1; }
                SetClickDanser(214 + (378 * lastPart), 328); //part select
                SetClickDanser(422, 440); //exit menu
            }
            //fmt::println("length {}\nmultiplier {}\nqScores {}\ni {}\nlastPart {}",length,multiplier,qScores,i,lastPart);

            auto configs = getOBCConfNames(pathConf, configPrefix); //configs
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
            if (configNumber <= 11) { // #fix rewrite this for any number of cfgs, like configNumber <= configNumber-9
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
                SetClickDanser(246, 196 + (24.286 * (configNumber - 11)));
                std::this_thread::sleep_for(80ms);
            }
            SetClickDanser(422, 440); //exit menu
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

    void moveMaps(const std::filesystem::path& pathD, const std::filesystem::path& pathOsuSongs, int removeMode) {
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

    void removeAllFilesInFolder(const std::filesystem::path& folder) {
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

    void makePremiereProject(const std::vector<InputTable>& InputTableV, const std::filesystem::path& pathProject,
                             const std::filesystem::path& pathV) {
        namespace fs = std::filesystem;
        using namespace std::chrono_literals;
        fmt::println("{}", pathProject.generic_string().append(
                "/template/premiere.prproj")); //remake all the directories here for yourself
        fs::remove_all(pathProject.generic_string().append("/auto"));
        fs::create_directory(pathProject.generic_string().append("/auto"));
        fs::copy(pathProject.generic_string().append("/template/premiere.prproj"),
                 pathProject.generic_string().append("/auto/premiere.prproj"));

        openExplorer(pathV.generic_string());
        std::this_thread::sleep_for(2s);
        SetCursorPos(220, 180);
        LeftClick();
        pressTwoKeys("CTRL", "A", InputTableV);
        SetCursorPos(284, 248);
        LeftDown();
        SetCursorPos(400, 800);
        openProgram("premiere");
        std::this_thread::sleep_for(10s);
        SetCursorPos(294, 834);
        std::this_thread::sleep_for(10s);
        LeftUp();
    }

}

auto main() -> int {
    //Namespaces
    namespace fs = std::filesystem;
    using namespace std::chrono_literals;
    using namespace YAUIO;

    //Paths
    auto const path = fs::path(R"(D:\Users\User\Desktop\Files\OsuScores\output.csv)"); //csv
    auto const pathD = fs::path(R"(F:\Users\User\Downloads\Scores)"); //replays
    auto const pathConf = fs::path(R"(D:\Users\User\Desktop\Files\danser-0.9.0-win\settings)"); //configs
    auto const pathV = fs::path(R"(D:\Users\User\Desktop\Files\danser-0.9.0-win\videos)"); //rendered videos
    auto const pathOsuSongs = fs::path("G:\\osu!\\Songs"); // osu!/songs path
    auto const pathProjects = fs::path(R"(F:\Users\User\Videos\trying to yt\OBC!Weekly)"); // osu!/songs path

    //Parsing data
    auto scores = parseScores(path);
    fmt::println("Begin parsing");
    auto InputTableV = ParseInputTable();
    auto weeklyScores = sortScores(scores);

    auto const searchConfigPrefix = "obc"; //danser configs name, see line 775
    int const cycle = 4; //How many scores out of 400pp+ ones you want to iterate over
    int const doRemove = 0; //0 - not remove replay&map file after rendering, 1 - remove
    int const openDanser = 1; //0 - just config danser, 1 - open and configure
    int const downloadMode = 0; //mode 0 - only replays ,1 - only maps, 2 - replays + maps

    if (doRemove == 1) {
        removeAllFilesInFolder(pathV); //delete all recorded videos
        removeAllFilesInFolder(pathD); //delete all files in replay folder
    }

    printScoresVector(weeklyScores);
    downloadData(weeklyScores, InputTableV, cycle, downloadMode);
    fmt::println("\n\nDownloaded all data successfully!");
    renameFiles(weeklyScores, cycle, pathD); //renames all replays to their pp value
    fmt::println("\n\nRenamed all data successfully!");
    moveMaps(pathD, pathOsuSongs, doRemove);
    fmt::println("\n\nMoved all data successfully!");
    setProperDancerState(openDanser);
    fmt::println("\n\nSet dancer state successfully!");
    renderReplays(weeklyScores, searchConfigPrefix, cycle, InputTableV, pathD, pathConf, pathV, doRemove);
    fmt::println("\n\nAll replays rendered successfully!");

    makePremiereProject(InputTableV, pathProjects, pathV);
    return 0;
}