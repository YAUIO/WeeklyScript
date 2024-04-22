#include <ranges>
#include "setget.h"
#include "step.h"
//#include "cmake-build-debug/_deps/mediainfolib-src/Source/MediaInfo/MediaInfo.h"
//Before running the code, don't forget to move inputTable.txt to cmake-build-debug, add STOP in the end of csv and properly set paths in main() and openProgram()
//Also, adjust locale for IpWindowName in OpenFocusChrome()

auto main() -> int {
    //Namespaces
    namespace fs = std::filesystem;
    using namespace std::chrono_literals;

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
    //int startMode = std::stoi(conf[9]); //stage to start with - 0 downloading, 1 renaming, 2 render

    auto window = sf::RenderWindow(sf::VideoMode(800, 600), "WeeklyScript");
    auto font = sf::Font();
    font.loadFromFile("reqfiles/font.otf");
    auto event = sf::Event();

    auto button1Text = sf::Text{"Download", font};
    button1Text.setPosition(20, 20);
    auto button2Text = sf::Text{"Rename", font};
    button2Text.setPosition(20, 80);
    auto button3Text = sf::Text{"OpenDanser", font};
    button3Text.setPosition(20, 140);
    auto button4Text = sf::Text{"Render", font};
    button4Text.setPosition(20, 200);
    auto button5Text = sf::Text{"RenderOpt", font};
    button5Text.setPosition(20, 260);

    auto textOutputOnTheRight = sf::Text{"", font};

    auto button1 = sf::RectangleShape(sf::Vector2f(205, 30));
    button1.setPosition(20, 20);
    button1.setFillColor(sf::Color::Cyan);
    auto button2 = sf::RectangleShape(sf::Vector2f(142, 30));
    button2.setPosition(20, 80);
    button2.setFillColor(sf::Color::Cyan);
    auto button3 = sf::RectangleShape(sf::Vector2f(220, 30));
    button3.setPosition(20, 140);
    button3.setFillColor(sf::Color::Cyan);
    auto button4 = sf::RectangleShape(sf::Vector2f(205, 30));
    button4.setPosition(20, 200);
    button4.setFillColor(sf::Color::Cyan);
    auto button5 = sf::RectangleShape(sf::Vector2f(220, 30));
    button5.setPosition(20, 260);
    button5.setFillColor(sf::Color::Cyan);

    window.setFramerateLimit(240);

    bool download = false;
    bool rename = false;
    bool openDanserB = false;
    bool render = false;
    bool renderOpt = false;

    while (window.isOpen()) {
        window.clear(sf::Color::Black);
        window.draw(button1Text);
        window.draw(button2Text);
        window.draw(button3Text);
        window.draw(button4Text);
        window.draw(button5Text);

        if (download || render || openDanserB || rename || renderOpt) {
            textOutputOnTheRight = sf::Text{"Please wait, working on it", font};
            textOutputOnTheRight.setPosition(140, 400);
        }else{
            textOutputOnTheRight = sf::Text{"", font};
        }
        window.draw(textOutputOnTheRight);

        if (download) {
            button1Text.setFillColor(sf::Color::Green);
            window.draw(button1Text);
            window.display();
            std::this_thread::sleep_for(2s);
            if (downloadData(weeklyScores, InputTableV, cycle, downloadMode, pathD)) {
                fmt::println("\n\nDownloaded all data successfully!");
                download = false;
                button1Text.setFillColor(sf::Color::White);
            } else {
                button1Text.setFillColor(sf::Color::Red);
            }
        }

        if (rename) {
            button2Text.setFillColor(sf::Color::Green);
            window.draw(button2Text);
            window.display();
            std::this_thread::sleep_for(2s);
            if (renameFiles(weeklyScores, cycle, pathD)) { //renames all replays to their pp value
                fmt::println("\n\nRenamed all data successfully!");
                moveMaps(pathD, pathOsuSongs, doRemove);
                fmt::println("\n\nMoved all data successfully!");
                rename = false;
                button2Text.setFillColor(sf::Color::White);
            } else {
                button2Text.setFillColor(sf::Color::Red);
            }
        }

        if (openDanserB) {
            button3Text.setFillColor(sf::Color::Green);
            window.draw(button3Text);
            window.display();
            std::this_thread::sleep_for(2s);
            setProperDancerState(openDanser);
            fmt::println("\n\nSet dancer state successfully!");
            openDanserB = false;
            button3Text.setFillColor(sf::Color::White);

        }

        if (render) {
            button4Text.setFillColor(sf::Color::Green);
            window.draw(button4Text);
            window.display();
            std::this_thread::sleep_for(2s);
            if (renderReplays(weeklyScores, cycle, InputTableV, pathD, pathConf, pathV, doRemove)) {
                fmt::println("\n\nAll replays rendered successfully!");
                render = false;
                button4Text.setFillColor(sf::Color::White);
            } else {
                button4Text.setFillColor(sf::Color::Red);
            }
        }

        if (renderOpt) {
            button5Text.setFillColor(sf::Color::Green);
            window.draw(button5Text);
            window.display();
            std::this_thread::sleep_for(2s);
            if (renderReplaysOpt(InputTableV, pathD, pathConf, pathV, doRemove)) {
                fmt::println("\n\nAll replays rendered successfully!");
                renderOpt = false;
                button5Text.setFillColor(sf::Color::White);
            } else {
                button5Text.setFillColor(sf::Color::Red);
            }
        }

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (isCursorOnButton(window, button1)) {
                    download = true;
                } else if (isCursorOnButton(window, button2)) {
                    rename = true;
                } else if (isCursorOnButton(window, button3)) {
                    openDanserB = true;
                } else if (isCursorOnButton(window, button4)) {
                    render = true;
                } else if (isCursorOnButton(window,button5)){
                    renderOpt = true;
                }
            }
        }
        window.display();
    }

    /*if (doRemove == 1) {
        removeAllFilesInFolder(pathV); //delete all recorded videos
        removeAllFilesInFolder(pathD); //delete all files in replay folder
    }

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

    //makePremiereProject(cycle,InputTableV,pathProjects,pathV,weeklyScores);*/

    return 0;
}
