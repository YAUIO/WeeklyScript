#include "setget.h"

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

void SetClickDanser(int x, int y) {
    using namespace std::chrono_literals;
    SetCursorPosDanser(x, y);
    std::this_thread::sleep_for(10ms);
    LeftClick();
    std::this_thread::sleep_for(80ms);
}

std::vector<int> GetColorDanser(int x, int y) {

    using namespace std::chrono_literals;
    HWND window = FindWindow(NULL, "danser-go 0.9.1 launcher");
    POINT p;
    COLORREF color;
    HDC hDC;

    if (window) {
        RECT rect = {0};
        GetWindowRect(window, &rect);
        SetForegroundWindow(window);
        SetActiveWindow(window);
        SetFocus(window);

        p.x = rect.right - x;
        p.y = rect.bottom - y;
    }

    // Get the device context for the screen
    hDC = GetDC(NULL);
    if (hDC == NULL)
        fmt::println("hdc null");

    // Retrieve the color at that position
    color = GetPixel(hDC, p.x, p.y);
    if (color == CLR_INVALID)
        fmt::println("color null");

    // Release the device context again
    ReleaseDC(GetDesktopWindow(), hDC);

    //fmt::println("R {} G {} B {}; x {} y {}", GetRValue(color), GetGValue(color), GetBValue(color),x,y);
    return std::vector<int>{GetRValue(color), GetGValue(color), GetBValue(color)};
}

int GetStrainDanser(long long length) {
    using namespace std::chrono_literals;
    //xmin = 208 xmax = 591  ymin=190 ymax = 262
    int x = 208;
    int y = 264;
    int yf = 0;
    int xf = 0;
    bool found = false;

    SetClickDanser(690, 166); //time/offset menu
    std::this_thread::sleep_for(80ms);

    while (y > 190) {
        x = 208;
        while (x < 591) {
            if (GetColorDanser(x, y) == std::vector<int>{255, 255, 255}) {
                xf = x;
                yf = y;
                found = true;
                break;
            }
            x++;
        }
        if (found) {
            break;
        }
        y--;
    }
    fmt::print("Found strain {} {} ", xf, yf);

    if (length < 300) {
        x = 208;
        y = yf;
        int yf1 = 0;
        int xf1 = 0;
        bool foundE = false;

        //last x is 290
        while (y > (yf - 6)) {
            x = 208;
            while (x < 290) {
                if (GetColorDanser(x, y) == std::vector<int>{255, 255, 255}) {
                    xf1 = x;
                    yf1 = y;
                    foundE = true;
                    break;
                }
                x++;
            }
            if (foundE) {
                break;
            }
            y--;
        }

        std::this_thread::sleep_for(80ms);
        fmt::print("- Found end strain {} {}", xf1, yf1);
        fmt::println("\n");
        if (foundE) {
            return xf1;
        }
    }
    return xf;
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

    if (mode == 1) { //mode 0 is just for rendering, 1 - for opening (basically useless now)
        openProgram("danser");
        fmt::println("Waiting for danser to load properly...");
        auto color = std::vector<int>(3);
        while (mode == 1){
            color = GetColorDanser(178,408);
            std::this_thread::sleep_for(2s);
            if(color[0] == 255 && color[1] == 255 && color[2] == 255){
                break;
            }
        }
    }

    //Set proper danser state
    SetClickDanser(694, 497); //mode
    SetClickDanser(682, 409); //watch a replay
    SetClickDanser(760, 38); //watch
    SetClickDanser(753, 95); //record
}

long long getLength (long long length){
    length = length / 440; //get approximate length in seconds
    if (length<0){
        length = 120;
    }
    fmt::println("Length is {}s",length);
    return length;
}

long long getLengthOpt (long long length){
    length = getLength(length);

    if(length>4000){
        length = length / 33409;
    }

    if (length<0){
        length = 120;
    }

    fmt::println("Length is {}s",length);
    return length;
}

double getMultiplier (int i, int qScores){
    double multiplier;
    if (i != 0) { multiplier = double(i) / (double(qScores) * double(qScores)); }
    else {
        multiplier = 1 /
                     double(qScores);
    }
    return multiplier;
}

double getLastPart(int foundSpike, long long length, int mods, double multiplier){
    double lastPart;
    if (length < 100) {
        lastPart = (28.0 + ((double(length) / 4) * multiplier)) / length;
    } else if (length > 180) {
        lastPart = (24.0 + ((double(length) / 2) * multiplier)) / length;
    } else {
        lastPart = (24.0 + ((double(length) / 3) * multiplier)) / length;
    }

    if (mods & ReplayParser::Mods::DoubleTime) {
        lastPart = lastPart * 4;
        length = length / 1.5;
        fmt::println("\n\nIt is, indeed, a gigachad DoubleTime score.");
        if(foundSpike < 294 && length < 300){
            lastPart = lastPart/2;
        }
    }

    if(length*lastPart<40){
        lastPart = 40.00/length;
    }else if(length*lastPart>90){
        lastPart = 90.00/length;
    }

    if (lastPart > 1) { lastPart = 1; }
    fmt::println("\n lastPart is {}",lastPart);

    return lastPart;
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
        } else if (arg[i] == '!' || arg[i] == 'V' || arg[i] == 'A' || arg[i] == 'I' || arg[i] == '"') {
            return false;
        } else {
            i++;
        }
    }
    return true;
}

/*int getVideoDuration(std::string fileName){
        using namespace MediaInfoLib;
        MediaInfo MI;
        MI.Open(fileName);
        //int duration = std::stoi(MI.Get(Stream_General, 0, "Duration"));
        MI.Close();
        return 0; //duration;
    }*/