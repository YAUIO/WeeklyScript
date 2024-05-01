#pragma once

#include <string>
#include <Windows.h>
#include <thread>
#include <fmt/core.h>
#include "action.h"
#include "ReplayParser.h"

void SetCursorPosDanser(int x, int y);

void SetCursorPosExplorer(int x, int y);

void SetClickExplorer(int x, int y);

void SetClickDanser(int x, int y);

std::vector<int> GetColorDanser(int x, int y);

int GetStrainDanser(long long length);

std::vector<std::string> getOBCConfNames(std::filesystem::path const &path);

void setProperDancerState(int mode);

long long getLength(long long length);

long long getLengthOpt(long long length);

double getMultiplier(int i, int qScores);

double getLastPart(int foundSpike, long long length, int mods, double multiplier);

std::string getKeyCode(std::string keyI, std::vector<InputTable> table);

bool isMods(std::string arg);

/*int getVideoDuration(std::string fileName)*/

