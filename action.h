#pragma once

#include <Windows.h>
#include <thread>
#include <vector>
#include <regex>
#include <fstream>
#include <filesystem>
#include <fmt/core.h>
#include <SFML/Graphics.hpp>

class InputTable {
public:
    std::string keyCode;
    std::string key;
};

void LeftClick();

void LeftUp();

void LeftDown();

void MouseScroll(double RY);

void openLink(std::string link);

void openProgram(std::string app);

void openExplorer(std::string pathToFoler);

std::string getKeyCode(std::string keyI, std::vector<InputTable> table);

void SetCursorPosExplorer(int x, int y);

void SetClickExplorer(int x, int y);

void pressTwoKeys(std::string s, std::string s1, std::vector<InputTable> InputTableV);

void pressKey(std::string s, std::vector<InputTable> InputTableV);

void typeInStringPP(std::string s, std::vector<InputTable> InputTableV);

void ExplorerFocusTextField();

std::vector<std::string> sortOBC(std::vector<std::string> v);

void moveMaps(std::filesystem::path pathD, std::filesystem::path pathOsuSongs, int removeMode);

void removeAllFilesInFolder(std::filesystem::path folder);

std::vector<std::string> loadCfg();

bool isCursorOnButton(sf::Window &window, sf::RectangleShape &button1);

std::vector<InputTable> ParseInputTable();