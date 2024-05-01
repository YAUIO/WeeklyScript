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

bool
downloadData(std::vector<Score> &weeklyScores, std::vector<InputTable> const &InputTableV, int cycle, int mode,
             std::filesystem::path const &pathD);

bool renderReplays(std::vector<Score> &weeklyScores, int qScores, std::vector<InputTable> const &InputTableV,
                   std::filesystem::path const &pathD, std::filesystem::path const &pathConf,
                   std::filesystem::path const &pathV,
                   int removeMode);

bool renderReplaysOpt(std::vector<InputTable> const &InputTableV,
                      std::filesystem::path &pathD, std::filesystem::path const &pathConf,
                      std::filesystem::path const &pathV,
                      int removeMode);

std::vector<Score> parseScores(std::filesystem::path const &path);

void printScoresVector(std::vector<Score> &v);

bool renameFiles(std::vector<Score> &vector, int qScores, std::filesystem::path const &path);

std::vector<Score> sortScores(std::vector<Score> &v);

void
makePremiereProject(int qReplays, std::vector<InputTable> const &InputTableV, std::filesystem::path const &pathProject,
                    std::filesystem::path const &pathV, std::vector<Score> &inpVec);