#include "ReplayParser.h"

namespace ReplayParser {
// https://github.com/qtkite/Osu-Replay-Parser
// https://osu.ppy.sh/help/wiki/osu!_File_Formats/Osr_(file_format)

/// Utility function to decode a ULEB128 value.
    static inline uint64_t decodeULEB128(const uint8_t *p, unsigned *n = 0) {
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
    }

    std::wstring s2ws(const std::string &str) {
        using convert_typeX = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_typeX, wchar_t> converterX;

        return converterX.from_bytes(str);
    }

    std::string ws2s(const std::wstring &wstr) {
        using convert_typeX = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_typeX, wchar_t> converterX;

        return converterX.to_bytes(wstr);
    }

    std::string parse_osu_string(std::ifstream &infile) {
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
    }

    void print_mods(int mods) {

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

    }

    ReplayFormat parseReplay(std::filesystem::path pathD, double pp) {
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

            infile.read((char *) &replay_data.m_chGameMode, sizeof(replay_data.m_chGameMode));
            //std::cout << "m_chGameMode:" << replay_data.m_chGameMode << std::endl;

            infile.read((char *) &replay_data.m_iGameVersion, sizeof(replay_data.m_iGameVersion));
            //std::cout << "m_iGameVersion:" << replay_data.m_iGameVersion << std::endl;

            replay_data.m_strMapMD5Hash = parse_osu_string(infile);
            //std::cout << "m_strMapMD5Hash:" << replay_data.m_strMapMD5Hash << std::endl;

            replay_data.m_strPlayerName = parse_osu_string(infile);
            std::cout << "m_strPlayerName:" << replay_data.m_strPlayerName << std::endl;

            replay_data.m_strReplayHash = parse_osu_string(infile);
            //std::cout << "m_strReplayHash:" << replay_data.m_strReplayHash << std::endl;

            infile.read((char *) &replay_data.m_sh300, sizeof(replay_data.m_sh300));
            //std::cout << "m_sh300:" << replay_data.m_sh300 << std::endl;

            infile.read((char *) &replay_data.m_sh100, sizeof(replay_data.m_sh100));
            //std::cout << "m_sh100:" << replay_data.m_sh100 << std::endl;

            infile.read((char *) &replay_data.m_sh50, sizeof(replay_data.m_sh50));
            //std::cout << "m_sh50:" << replay_data.m_sh50 << std::endl;

            infile.read((char *) &replay_data.m_shGekis, sizeof(replay_data.m_shGekis));
            //std::cout << "m_shGekis:" << replay_data.m_shGekis << std::endl;

            infile.read((char *) &replay_data.m_shKatus, sizeof(replay_data.m_shKatus));
            //std::cout << "m_shKatus:" << replay_data.m_shKatus << std::endl;

            infile.read((char *) &replay_data.m_shMisses, sizeof(replay_data.m_shMisses));
            //std::cout << "m_shMisses:" << replay_data.m_shMisses << std::endl;

            infile.read((char *) &replay_data.m_iTotalScore, sizeof(replay_data.m_iTotalScore));
            //std::cout << "m_iTotalScore:" << replay_data.m_iTotalScore << std::endl;

            infile.read((char *) &replay_data.m_shHighestCombo, sizeof(replay_data.m_shHighestCombo));
            std::cout << "m_shHighestCombo:" << replay_data.m_shHighestCombo << std::endl;

            infile.read((char *) &replay_data.m_chFC, sizeof(replay_data.m_chFC));
            //std::cout << "m_chFC:" << replay_data.m_chFC << std::endl;

            infile.read((char *) &replay_data.m_iMods, sizeof(replay_data.m_iMods));
            std::cout << "m_iMods:" << replay_data.m_iMods << std::endl;

            replay_data.m_strLifeBar = parse_osu_string(infile);
            //std::cout << "m_strLifeBar:" << replay_data.m_strLifeBar << std::endl;

            infile.read((char *) &replay_data.m_lTimeStamp, sizeof(replay_data.m_lTimeStamp));
            //std::cout << "m_lTimeStamp:" << replay_data.m_lTimeStamp << std::endl;

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

        return replay_data;
    }

    ReplayFormat parseReplayOpt(std::filesystem::path pathD, std::string name) {
        auto s = pathD.generic_string().append("/").append(name);

        ReplayFormat replay_data;

        std::stringstream configfile;
        configfile << s;

        std::ifstream infile;

        fmt::println("Opening: {}", configfile.str().c_str());
        //std::cout << "Opening: " << configfile.str().c_str() << std::endl;

        infile.open(configfile.str().c_str(), std::ios::binary);

        if (infile.is_open()) {
            std::cout << "Successfully opened file" << std::endl;

            infile.read((char *) &replay_data.m_chGameMode, sizeof(replay_data.m_chGameMode));
            //std::cout << "m_chGameMode:" << replay_data.m_chGameMode << std::endl;

            infile.read((char *) &replay_data.m_iGameVersion, sizeof(replay_data.m_iGameVersion));
            //std::cout << "m_iGameVersion:" << replay_data.m_iGameVersion << std::endl;

            replay_data.m_strMapMD5Hash = parse_osu_string(infile);
            //std::cout << "m_strMapMD5Hash:" << replay_data.m_strMapMD5Hash << std::endl;

            replay_data.m_strPlayerName = parse_osu_string(infile);
            std::cout << "m_strPlayerName:" << replay_data.m_strPlayerName << std::endl;

            replay_data.m_strReplayHash = parse_osu_string(infile);
            //std::cout << "m_strReplayHash:" << replay_data.m_strReplayHash << std::endl;

            infile.read((char *) &replay_data.m_sh300, sizeof(replay_data.m_sh300));
            //std::cout << "m_sh300:" << replay_data.m_sh300 << std::endl;

            infile.read((char *) &replay_data.m_sh100, sizeof(replay_data.m_sh100));
            //std::cout << "m_sh100:" << replay_data.m_sh100 << std::endl;

            infile.read((char *) &replay_data.m_sh50, sizeof(replay_data.m_sh50));
            //std::cout << "m_sh50:" << replay_data.m_sh50 << std::endl;

            infile.read((char *) &replay_data.m_shGekis, sizeof(replay_data.m_shGekis));
            //std::cout << "m_shGekis:" << replay_data.m_shGekis << std::endl;

            infile.read((char *) &replay_data.m_shKatus, sizeof(replay_data.m_shKatus));
            //std::cout << "m_shKatus:" << replay_data.m_shKatus << std::endl;

            infile.read((char *) &replay_data.m_shMisses, sizeof(replay_data.m_shMisses));
            //std::cout << "m_shMisses:" << replay_data.m_shMisses << std::endl;

            infile.read((char *) &replay_data.m_iTotalScore, sizeof(replay_data.m_iTotalScore));
            //std::cout << "m_iTotalScore:" << replay_data.m_iTotalScore << std::endl;

            infile.read((char *) &replay_data.m_shHighestCombo, sizeof(replay_data.m_shHighestCombo));
            std::cout << "m_shHighestCombo:" << replay_data.m_shHighestCombo << std::endl;

            infile.read((char *) &replay_data.m_chFC, sizeof(replay_data.m_chFC));
            //std::cout << "m_chFC:" << replay_data.m_chFC << std::endl;

            infile.read((char *) &replay_data.m_iMods, sizeof(replay_data.m_iMods));
            std::cout << "m_iMods:" << replay_data.m_iMods << std::endl;

            replay_data.m_strLifeBar = parse_osu_string(infile);
            //std::cout << "m_strLifeBar:" << replay_data.m_strLifeBar << std::endl;

            infile.read((char *) &replay_data.m_lTimeStamp, sizeof(replay_data.m_lTimeStamp));
            //std::cout << "m_lTimeStamp:" << replay_data.m_lTimeStamp << std::endl;

            infile.read((char *) &replay_data.m_iLenCompressedReplay, sizeof(replay_data.m_iLenCompressedReplay));
            std::cout << "m_iLenCompressedReplay:" << replay_data.m_iLenCompressedReplay << std::endl;

            /*for (auto i = 0; i < replay_data.m_iLenCompressedReplay; i++) {

                unsigned char byte_part = 0x0;
                infile.read((char *) &byte_part, sizeof(byte_part));
                replay_data.compressed_data.push_back(byte_part);
            }*/

        }
        infile.close();

        //print_mods(replay_data.m_iMods);

        std::cout << "Finished parsing" << std::endl;

        return replay_data;
    }
}