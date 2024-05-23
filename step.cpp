#include "step.h"

bool
downloadData(std::vector<Score> &weeklyScores, std::vector<InputTable> const &InputTableV, int cycle, int mode,
             std::filesystem::path const &pathD) {
    int i = 0;
    int ii = 0;
    bool foundReplay = false;
    using namespace std::chrono_literals;
    namespace fs = std::filesystem;
    std::vector<std::string> pathM;
    std::string currentLink;

    while (i == 0) {
        HWND window = FindWindow(NULL, "Google Chrome");
        if (window) {
            SetForegroundWindow(window);
            SetActiveWindow(window);
            SetFocus(window);
            break;
        } else {
            openProgram("chrome");
        }
        std::this_thread::sleep_for(2s);
        i++;
    }

    i = 0;
    std::this_thread::sleep_for(2s);
    SetCursorPos(900, 500);
    openLink("https://twitter.com");
    std::this_thread::sleep_for(2s);
    pressKey("F11", InputTableV);
    pressTwoKeys("CTRL", "W", InputTableV);
    while (i < cycle) {

        pathM = std::vector<std::string>{};
        for (const auto &entry: fs::directory_iterator(pathD)) {
            pathM.push_back(entry.path().generic_string());
        }
        ii = 0;
        foundReplay = false;
        while (ii < weeklyScores[i].scoreLink.size()) {
            if (weeklyScores[i].scoreLink[ii] >= '0' && weeklyScores[i].scoreLink[ii] <= '9') {
                break;
            }
            ii++;
        }
        currentLink = weeklyScores[i].scoreLink.substr(ii, weeklyScores[i].scoreLink.size());
        for (const auto &str: pathM) {
            if (std::regex_match(str, std::regex(".*" + currentLink + ".*"))) {
                foundReplay = true;
            }
        }

        if (foundReplay) {
            i++;
        } else {
            if (mode == 0 || mode == 2) {
                openLink(weeklyScores[i].scoreLink);
                SetCursorPos(1284, 448); //Replay download
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

            pathM = std::vector<std::string>{};
            for (const auto &entry: fs::directory_iterator(pathD)) {
                pathM.push_back(entry.path().generic_string());
            }
            ii = 0;
            foundReplay = false;
            while (ii < weeklyScores[i].scoreLink.size()) {
                if (weeklyScores[i].scoreLink[ii] >= '0' && weeklyScores[i].scoreLink[ii] <= '9') {
                    break;
                }
                ii++;
            }
            currentLink = weeklyScores[i].scoreLink.substr(ii, weeklyScores[i].scoreLink.size());
            for (const auto &str: pathM) {
                if (std::regex_match(str, std::regex(".*" + currentLink + ".*"))) {
                    foundReplay = true;
                }
            }
            if (foundReplay) {
                i++;
            }
        }
    }
    pressKey("F11", InputTableV);
    std::this_thread::sleep_for(4s);
    pressTwoKeys("ALT", "F4", InputTableV);
    std::this_thread::sleep_for(20ms);
    return true;
}

bool renderReplays(std::vector<Score> &weeklyScores, int qScores, std::vector<InputTable> const &InputTableV,
                   std::filesystem::path const &pathD, std::filesystem::path const &pathConf,
                   std::filesystem::path const &pathV,
                   int removeMode) {
    using namespace std::chrono_literals;
    namespace fs = std::filesystem;
    bool isBegChecked;
    bool rendered;
    bool retry = false;
    double lastPart;
    double multiplier;
    long long length;
    int mods;
    int i = 0;
    //fmt::println("{}",pathD.generic_string().append("/").append(std::to_string(score.pp).append(".osr")));

    while (i < qScores) {

        fmt::println("i is {}",i);

        rendered = false;

        for (const auto &entry: fs::directory_iterator(pathV)) {
            if (std::regex_match(entry.path().generic_string(),
                                 std::regex(".*" + std::to_string(weeklyScores[i].pp) + "\\.mp4"))) {
                rendered = true;
                break;
            }
        }

        if (rendered) {
            i++;
        } else {

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
            std::this_thread::sleep_for(200ms);
            pressTwoKeys("CTRL", "A", InputTableV);
            pressKey("DEL", InputTableV);
            typeInStringPP(std::to_string(weeklyScores[i].pp), InputTableV);
            SetClickDanser(410, 194); //escape configure

            rendered = false;
            length = ReplayParser::parseReplay(pathD, weeklyScores[i].pp).m_iLenCompressedReplay;
            mods = ReplayParser::parseReplay(pathD, weeklyScores[i].pp).m_iMods;
            length = getLength(pathD.generic_string().append("/").append(std::to_string(weeklyScores[i].pp)).append(".osr"));
            fmt::println("{} is length of the replay",length);
            std::this_thread::sleep_for(40ms);

            if (length <= 40) {
                if (!isBegChecked) {
                    fmt::println("Rendering full replay (length<=40s), {}s", length);
                    SetClickDanser(690, 166); //time/offset menu
                    std::this_thread::sleep_for(80ms);
                    SetClickDanser(582, 388); //check skip map beginning
                    std::this_thread::sleep_for(80ms);
                    SetClickDanser(590, 330); //full start time
                    std::this_thread::sleep_for(80ms);
                    SetClickDanser(214, 276); //full end time
                    std::this_thread::sleep_for(80ms);
                    SetClickDanser(422, 440); //exit menu
                }
            } else {

                multiplier = getMultiplier(i, qScores);
                int fullX = 382;
                fmt::println("Getting strain graph");
                int foundSpike = GetStrainDanser(length);
                int firstPart;
                lastPart = getLastPart(foundSpike, length, mods, multiplier);

                std::this_thread::sleep_for(80ms);
                if (foundSpike < 294) {
                    firstPart = (lastPart - ((foundSpike - 212) / fullX)) * fullX;
                    SetClickDanser(foundSpike + firstPart, 328); //start time
                    fmt::println("Starting x: {}, Found Spike: {}", foundSpike + firstPart, foundSpike);
                    std::this_thread::sleep_for(100ms);
                    SetClickDanser(214, 276); //full end time
                } else {
                    int startx = foundSpike + ((foundSpike - 212) * (lastPart / 1.5));
                    int endx = foundSpike - ((fullX + 212 - foundSpike) * (lastPart / 1.5));
                    SetClickDanser(startx, 328);//start time
                    std::this_thread::sleep_for(80ms);
                    if (endx < 224 && length < 120) { endx = 214; }
                    SetClickDanser(endx, 278);//end time
                    fmt::println("Found Spike: {}, Starting x: {}, Ending x: {}", foundSpike, startx, endx);
                }
                std::this_thread::sleep_for(80ms);
                SetClickDanser(422, 440); //exit menu
            }
            //fmt::println("length {}\nmultiplier {}\nqScores {}\ni {}\nlastPart {}",length,multiplier,qScores,i,lastPart);

            //Config selection
            auto configs = getOBCConfNames(pathConf); //configs
            configs = sortOBC(configs);
            auto configPath = std::string();
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
            if (configNumber <= configs.size() - 7) {
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
                fmt::println("config number: {}", configNumber);
                int y = 0;
                switch (configs.size() - configNumber) {
                    case 0: {
                        y = 170;
                        break;
                    }
                    case 1: {
                        y = 200;
                        break;
                    }
                    case 2: {
                        y = 225;
                        break;
                    }
                    case 3: {
                        y = 250;
                        break;
                    }
                    case 4: {
                        y = 281;
                        break;
                    }
                    case 5: {
                        y = 310;
                        break;
                    }
                    case 6: {
                        y = 336;
                        break;
                    }
                    case 7: {
                        y = 368;
                        break;
                    }
                    default: {
                        MouseScroll(20);
                        y = 366;
                    }
                }
                SetClickDanser(246, y);
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
                HWND window = FindWindow(NULL, "danser-go 0.9.1 launcher");
                if (!window) {
                    retry = true;
                    break;
                }
            }

            if (!retry) {
                if (removeMode == 1) {
                    std::filesystem::remove(
                            pathD.generic_string().append("/").append(
                                    std::to_string(weeklyScores[i].pp).append(".osr")));
                }
                i++;
            } else if (retry) {
                setProperDancerState(1);
                retry = false;
            }
        }
    }
    return true;
}

bool renderReplaysOpt(std::vector<InputTable> const &InputTableV,
                      std::filesystem::path &pathD, std::filesystem::path const &pathConf,
                      std::filesystem::path const &pathV,
                      int removeMode) {
    using namespace std::chrono_literals;
    bool isBegChecked;
    bool rendered;
    bool retry = false;
    double lastPart;
    double multiplier;
    double pp;
    long long length;
    int mods;
    int i = 0;
    int ii;
    std::string osrName;
    pathD = pathD.generic_string().append("\\opt");
    //fmt::println("{}",pathD.generic_string().append("/").append(std::to_string(score.pp).append(".osr")));

    auto pathM = std::vector<std::string>();
    namespace fs = std::filesystem;
    for (const auto &entry: fs::directory_iterator(pathD)) {
        if (std::regex_match(entry.path().generic_string(), std::regex(".*osr"))) {
            pathM.push_back(entry.path().generic_string());
        }
    }

    while (i < pathM.size()) {
        ii = pathM[i].size() - 1;
        while (ii > 0) {
            if (pathM[i][ii] == '/') {
                break;
            }
            ii--;
        }
        fmt::println("{} {}", osrName, pathM[i].substr(ii + 1, pathM[i].size() - 1));
        osrName = pathM[i].substr(ii + 1, pathM[i].size() - 1);

        pp = std::stod(osrName.substr(0, osrName.size() - 4));
        fmt::println("Current replay's pp {}", pp);
        //Starting to configure recording
        SetClickDanser(745, 423); //select replay
        std::this_thread::sleep_for(100ms);
        SetCursorPosExplorer(740, 420);
        LeftClick();
        LeftClick();
        std::this_thread::sleep_for(100ms);
        ExplorerFocusTextField();
        typeInStringPP(osrName, InputTableV);
        SetClickExplorer(136, 36);

        std::this_thread::sleep_for(80ms);
        SetClickDanser(542, 40); //configure
        std::this_thread::sleep_for(40ms);
        SetClickDanser(388, 262); //focus text field
        std::this_thread::sleep_for(200ms);
        pressTwoKeys("CTRL", "A", InputTableV);
        pressKey("DEL", InputTableV);
        typeInStringPP(std::to_string(pp), InputTableV);
        SetClickDanser(410, 194); //escape configure

        rendered = false;
        length = ReplayParser::parseReplayOpt(pathD, osrName).m_iLenCompressedReplay;
        mods = ReplayParser::parseReplayOpt(pathD, osrName).m_iMods;

        length = getLength(pathD.generic_string().append("/").append(osrName));

        std::this_thread::sleep_for(40ms);
        if (length <= 40) {
            if (!isBegChecked) {
                fmt::println("Rendering full replay (length<=40s), {}s", length);
                SetClickDanser(690, 166); //time/offset menu
                std::this_thread::sleep_for(80ms);
                SetClickDanser(582, 388); //check skip map beginning
                std::this_thread::sleep_for(80ms);
                SetClickDanser(590, 330); //full start time
                std::this_thread::sleep_for(80ms);
                SetClickDanser(214, 276); //full end time
                std::this_thread::sleep_for(80ms);
                SetClickDanser(422, 440); //exit menu
            }
        } else {

            multiplier = getMultiplier(i, pathM.size());
            int fullX = 382;
            fmt::println("Getting strain graph");
            int foundSpike = GetStrainDanser(length);
            int firstPart;
            lastPart = getLastPart(foundSpike, length, mods, multiplier);

            std::this_thread::sleep_for(80ms);
            if (foundSpike < 294) {
                firstPart = (lastPart - ((foundSpike - 212) / fullX)) * fullX;
                SetClickDanser(foundSpike + firstPart, 328); //start time
                fmt::println("Starting x: {}, Found Spike: {}", foundSpike + firstPart, foundSpike);
                std::this_thread::sleep_for(100ms);
                SetClickDanser(214, 276); //full end time
            } else {
                int startx = foundSpike + ((foundSpike - 212) * (lastPart / 1.5));
                int endx = foundSpike - ((fullX + 212 - foundSpike) * (lastPart / 1.5));
                SetClickDanser(startx, 328);//start time
                std::this_thread::sleep_for(80ms);
                SetClickDanser(endx, 278);//end time
                fmt::println("Found Spike: {}, Starting x: {}, Ending x: {}", foundSpike, startx, endx);
            }
            std::this_thread::sleep_for(80ms);
            SetClickDanser(422, 440); //exit menu
        }
        //fmt::println("length {}\nmultiplier {}\nqScores {}\ni {}\nlastPart {}",length,multiplier,qScores,i,lastPart);

        //Config selection
        auto configs = getOBCConfNames(pathConf); //configs
        configs = sortOBC(configs);
        auto configPath = std::string();
        namespace fs = std::filesystem;
        int configNumber = 0;
        std::string username;
        username = ReplayParser::parseReplayOpt(pathD, osrName).m_strPlayerName;
        for (const auto &entry: configs) {
            if (std::regex_match(configs[configNumber], std::regex(".*obc!" + username + ".*"))) {
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
        if (configNumber <= configs.size() - 7) {
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
            fmt::println("config number: {}", configNumber);
            int y = 0;
            switch (configs.size() - configNumber) {
                case 0: {
                    y = 170;
                    break;
                }
                case 1: {
                    y = 200;
                    break;
                }
                case 2: {
                    y = 225;
                    break;
                }
                case 3: {
                    y = 250;
                    break;
                }
                case 4: {
                    y = 281;
                    break;
                }
                case 5: {
                    y = 310;
                    break;
                }
                case 6: {
                    y = 336;
                    break;
                }
                case 7: {
                    y = 368;
                    break;
                }
                default: {
                    MouseScroll(20);
                    y = 366;
                }
            }
            SetClickDanser(246, y);
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
                                     std::regex(".*" + std::to_string(pp) + "\\.mp4"))) {
                    rendered = true;
                    break;
                }
            }
            HWND window = FindWindow(NULL, "danser-go 0.9.1 launcher");
            if (!window) {
                retry = true;
                break;
            }
        }

        if (!retry) {
            if (removeMode == 1) {
                std::filesystem::remove(
                        pathD.generic_string().append("/").append(
                                std::to_string(pp).append(".osr")));
            }
            i++;
        } else if (retry) {
            setProperDancerState(1);
            retry = false;
        }
    }
    return true;
}

std::vector<Score> parseScores(std::filesystem::path const &path) {
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
                fmt::print("6 ----- {}, {} check", debugStr[check - 2], check);
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
                fmt::println("{} total pp: /{}/", i, argument);
                scores[scoreN].total_pp = std::stod(argument);
            }
            if (i == 12) {
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

bool renameFiles(std::vector<Score> &vector, int qScores, std::filesystem::path const &path) {
    int i = 0;
    namespace fs = std::filesystem;
    std::string pathM[vector.size() * 2 + 1];
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
    fmt::println("Starting renaming");
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
    return true;
}

std::vector<Score> sortScores(std::vector<Score> &v) {
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

void
makePremiereProject(int qReplays, std::vector<InputTable> const &InputTableV, std::filesystem::path const &pathProject,
                    std::filesystem::path const &pathV, std::vector<Score> &inpVec) {
    namespace fs = std::filesystem;
    using namespace std::chrono_literals;
    int premR = 0;
    int textScale;
    auto genericPauseSize = 100ms;
    std::string bufS;
    fmt::println("{}", pathProject.generic_string().append("/template/premiere.prproj"));
    fs::remove(pathProject.generic_string().append("/auto/premiere.prproj"));
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
    SetCursorPos(1594, 760);
    std::this_thread::sleep_for(genericPauseSize);
    LeftClick();
    std::this_thread::sleep_for(genericPauseSize);

    SetCursorPos(900, 524);
    std::this_thread::sleep_for(genericPauseSize);
    pressKey("T", InputTableV);
    std::this_thread::sleep_for(1s);
    LeftClick();
    SetCursorPos(1446, 202);
    std::this_thread::sleep_for(genericPauseSize);
    LeftClick();
    std::this_thread::sleep_for(genericPauseSize);
    SetCursorPos(1524, 272); //FONT
    std::this_thread::sleep_for(genericPauseSize);
    LeftClick();
    std::this_thread::sleep_for(genericPauseSize);
    typeInStringPP("LEMON MILK BOLD", InputTableV);
    std::this_thread::sleep_for(genericPauseSize);
    pressKey("ENTER", InputTableV);
    std::this_thread::sleep_for(genericPauseSize);
    SetCursorPos(900, 524);
    std::this_thread::sleep_for(genericPauseSize);
    LeftClick();
    std::this_thread::sleep_for(genericPauseSize);
    bufS = inpVec[premR].username.append(" BY");
    bufS.append(std::to_string(inpVec[premR].country_rank));
    fmt::println("Output text: {}", bufS);
    typeInStringPP(bufS, InputTableV);
    std::this_thread::sleep_for(genericPauseSize);
    SetCursorPos(940, 520);
    std::this_thread::sleep_for(genericPauseSize);
    LeftClick();
    std::this_thread::sleep_for(genericPauseSize);
    pressTwoKeys("CTRL", "A", InputTableV);
    std::this_thread::sleep_for(genericPauseSize);
    SetCursorPos(1640, 300); //TEXT SIZE
    std::this_thread::sleep_for(genericPauseSize);
    LeftClick();
    std::this_thread::sleep_for(genericPauseSize);
    if (bufS.size() > 16) {
        textScale = 34;
    } else {
        textScale = 40;
    }
    typeInStringPP(std::to_string(textScale), InputTableV);
    pressKey("ENTER", InputTableV);
    std::this_thread::sleep_for(genericPauseSize);

    SetCursorPos(1524, 330); //TEXT ALIGN
    std::this_thread::sleep_for(genericPauseSize);
    LeftClick();
    std::this_thread::sleep_for(genericPauseSize);

    SetCursorPos(1630, 634); //X text
    std::this_thread::sleep_for(genericPauseSize);
    LeftClick();
    std::this_thread::sleep_for(genericPauseSize);
    typeInStringPP("1664", InputTableV);
    std::this_thread::sleep_for(genericPauseSize);
    pressKey("ENTER", InputTableV);
    std::this_thread::sleep_for(genericPauseSize);

    SetCursorPos(1674, 634); //Y text
    std::this_thread::sleep_for(genericPauseSize);
    LeftClick();
    std::this_thread::sleep_for(genericPauseSize);
    typeInStringPP("1044", InputTableV);
    std::this_thread::sleep_for(genericPauseSize);
    pressKey("ENTER", InputTableV);
    std::this_thread::sleep_for(genericPauseSize);
}