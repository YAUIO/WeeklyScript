
#include <string>
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdint.h>
#include <codecvt>
#include <vector>
#include <locale>

// https://osu.ppy.sh/help/wiki/osu!_File_Formats/Osr_(file_format)
struct ReplayFormat
{
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
		*n = (unsigned)(p - orig_p);
	return Value;
}


std::wstring s2ws(const std::string& str)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.from_bytes(str);
}

std::string ws2s(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

std::string parse_osu_string(std::ifstream& infile)
{
	unsigned char header_info;

	infile.read((char*)&header_info, sizeof(header_info));

	if (header_info == 0x0)
		return "no values were identified";
	else if (header_info == 0x0b) {

		uint8_t length = 0;

		infile.read((char*)&length, sizeof(length));

		unsigned ret = 0;

		auto decLen = decodeULEB128(&length, &ret);

		std::vector<wchar_t> utf8str;

		for (auto i = 0; i < decLen; i++) {

			unsigned char utfbyte;
			infile.read((char*)&utfbyte, sizeof(utfbyte));
			utf8str.push_back(utfbyte);
		}

		std::wstring wutfstr;

		for (auto utfpart : utf8str) {
			wutfstr += utfpart;
		}
		auto stringfromwide = ws2s(wutfstr);
		return stringfromwide;
	}
	return "";
}

enum Mods
{
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
	Relax2 = 8192,	// Autopilot
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
	FreeModAllowed = NoFail | Easy | Hidden | HardRock | SuddenDeath | Flashlight | FadeIn | Relax | Relax2 | SpunOut | KeyMod,
	ScoreIncreaseMods = Hidden | HardRock | DoubleTime | Flashlight | FadeIn
};

void print_mods(int mods)
{

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
int main(int argc, char *argv[])
{
	char* path = argv[argc - 1];

	ReplayFormat replay_data;

	std::stringstream configfile;
	configfile << path;

	std::ifstream infile;

	std::cout << "Opening: " << configfile.str().c_str() << std::endl;

	infile.open(configfile.str().c_str(), std::ios::binary);

	if (infile.is_open()) {
		std::cout << "Opened file" << std::endl;

		infile.read((char*)&replay_data.m_chGameMode, sizeof(replay_data.m_chGameMode));
		std::cout << "m_chGameMode:" << replay_data.m_chGameMode << std::endl;

		infile.read((char*)&replay_data.m_iGameVersion, sizeof(replay_data.m_iGameVersion));
		std::cout << "m_iGameVersion:" << replay_data.m_iGameVersion << std::endl;

		replay_data.m_strMapMD5Hash = parse_osu_string(infile);
		std::cout << "m_strMapMD5Hash:" << replay_data.m_strMapMD5Hash << std::endl;

		replay_data.m_strPlayerName = parse_osu_string(infile);
		std::cout << "m_strPlayerName:" << replay_data.m_strPlayerName << std::endl;

		replay_data.m_strReplayHash = parse_osu_string(infile);
		std::cout << "m_strReplayHash:" << replay_data.m_strReplayHash << std::endl;

		infile.read((char*)&replay_data.m_sh300, sizeof(replay_data.m_sh300));
		std::cout << "m_sh300:" << replay_data.m_sh300 << std::endl;

		infile.read((char*)&replay_data.m_sh100, sizeof(replay_data.m_sh100));
		std::cout << "m_sh100:" << replay_data.m_sh100 << std::endl;

		infile.read((char*)&replay_data.m_sh50, sizeof(replay_data.m_sh50));
		std::cout << "m_sh50:" << replay_data.m_sh50 << std::endl;

		infile.read((char*)&replay_data.m_shGekis, sizeof(replay_data.m_shGekis));
		std::cout << "m_shGekis:" << replay_data.m_shGekis << std::endl;

		infile.read((char*)&replay_data.m_shKatus, sizeof(replay_data.m_shKatus));
		std::cout << "m_shKatus:" << replay_data.m_shKatus << std::endl;

		infile.read((char*)&replay_data.m_shMisses, sizeof(replay_data.m_shMisses));
		std::cout << "m_shMisses:" << replay_data.m_shMisses << std::endl;

		infile.read((char*)&replay_data.m_iTotalScore, sizeof(replay_data.m_iTotalScore));
		std::cout << "m_iTotalScore:" << replay_data.m_iTotalScore << std::endl;

		infile.read((char*)&replay_data.m_shHighestCombo, sizeof(replay_data.m_shHighestCombo));
		std::cout << "m_shHighestCombo:" << replay_data.m_shHighestCombo << std::endl;

		infile.read((char*)&replay_data.m_chFC, sizeof(replay_data.m_chFC));
		std::cout << "m_chFC:" << replay_data.m_chFC << std::endl;

		infile.read((char*)&replay_data.m_iMods, sizeof(replay_data.m_iMods));
		std::cout << "m_iMods:" << replay_data.m_iMods << std::endl;

		replay_data.m_strLifeBar = parse_osu_string(infile);
		std::cout << "m_strLifeBar:" << replay_data.m_strLifeBar << std::endl;

		infile.read((char*)&replay_data.m_lTimeStamp, sizeof(replay_data.m_lTimeStamp));
		std::cout << "m_lTimeStamp:" << replay_data.m_lTimeStamp << std::endl;

		infile.read((char*)&replay_data.m_iLenCompressedReplay, sizeof(replay_data.m_iLenCompressedReplay));
		std::cout << "m_iLenCompressedReplay:" << replay_data.m_iLenCompressedReplay << std::endl;

		for (auto i = 0; i < replay_data.m_iLenCompressedReplay; i++) {

			unsigned char byte_part = 0x0;
			infile.read((char*)&byte_part, sizeof(byte_part));
			replay_data.compressed_data.push_back(byte_part);
		}


	}
	infile.close();

	print_mods(replay_data.m_iMods);

	std::cout << "Finished parsing" << std::endl;

	system("pause");

	return 0;
}

