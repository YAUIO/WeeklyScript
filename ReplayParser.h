#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <cstdint>
#include <codecvt>
#include <locale>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <fmt/core.h>

namespace ReplayParser{
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
    ReplayFormat parseReplay(std::filesystem::path pathD, double pp);
    ReplayFormat parseReplayOpt(std::filesystem::path pathD, std::string name);
}
