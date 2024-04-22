#pragma once
#include "setget.h"

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

bool downloadData(std::vector<Score> weeklyScores, std::vector<InputTable> InputTableV, int cycle, int mode,
                  std::filesystem::path pathD);

bool renderReplays(std::vector<Score> weeklyScores, int qScores, std::vector<InputTable> InputTableV,
                   std::filesystem::path pathD, std::filesystem::path pathConf, std::filesystem::path pathV,
                   int removeMode);

bool renderReplaysOpt(std::vector<InputTable> InputTableV,
                      std::filesystem::path pathD, std::filesystem::path pathConf, std::filesystem::path pathV,
                      int removeMode);

std::vector<Score> parseScores(std::filesystem::path path);

void printScoresVector(std::vector<Score> v);

bool renameFiles(std::vector<Score> vector, int qScores, std::filesystem::path path);

std::vector<Score> sortScores(std::vector<Score> v);

void makePremiereProject(int qReplays, std::vector<InputTable> InputTableV, std::filesystem::path pathProject,
                         std::filesystem::path pathV, std::vector<Score> inpVec);