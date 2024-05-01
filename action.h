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

void openLink(std::string const &link);

void openProgram(std::string app);

void openExplorer(std::string const &pathToFoler);

std::string getKeyCode(std::string const &keyI, std::vector<InputTable> const &table);

void SetCursorPosExplorer(int x, int y);

void SetClickExplorer(int x, int y);

void pressTwoKeys(std::string s, std::string s1, std::vector<InputTable> const &InputTableV);

void pressKey(std::string s, std::vector<InputTable> const &InputTableV);

void typeInStringPP(std::string s, std::vector<InputTable> const &InputTableV);

void ExplorerFocusTextField();

std::vector<std::string> sortOBC(std::vector<std::string> v);

void moveMaps(std::filesystem::path const &pathD, std::filesystem::path const &pathOsuSongs, int const &removeMode);

void removeAllFilesInFolder(std::filesystem::path const &folder);

std::vector<std::string> loadCfg();

bool isCursorOnButton(sf::Window &window, sf::RectangleShape &button1);

std::vector<InputTable> ParseInputTable();