////////////////////////////////////////////////////////////////
//                                                            //
//      OsrParser.h                                           //
//      HaselLoyance 2017-2018, Unlicense                     //
//      https://github.com/HaselLoyance/osu-parser            //
//                                                            //
////////////////////////////////////////////////////////////////

#ifndef OSU_PARSER_OSR_H
#define OSU_PARSER_OSR_H

#include "OsTypes.h"
#include "OsUtils.h"

namespace osuParser
{
	// Object for parsing "*.osr" files from preloaded stream
	// For file format please visit: https://osu.ppy.sh/help/wiki/osu!_File_Formats/Osr_(file_format)
	class OsrParser
	{
	public:
		// Creates a parser from input data stream
		OsrParser(std::istream * stream);
		~OsrParser();

		// Goes through istream and reads all data
		void Parse();

		// Game mode of the replay
		GameMode mode;

		// Version of the game where the replay was created
		OsLong version;

		// osu! beatmap MD5 hash
		std::string beatmapHash;

		// Player name
		std::string player;

		// osu! replay MD5 hash
		std::string replayHash;

		// Number of 300s
		OsShort n300s;

		// Number of 100s in standard, 150s in Taiko, 100s in CTB, 200s in mania
		OsShort n100s;

		// Number of 50s in standard, small fruit in CTB, 50s in mania
		OsShort n50s;

		// Number of Gekis in standard, Max 300s in mania
		OsShort nGekis;

		// Number of Katus in standard, 100s in mania
		OsShort nKatus;

		// Number of misses
		OsShort nMisses;

		// Total score displayed on the score report
		OsInteger score;

		// Greatest combo displayed on the score report
		OsShort combo;

		// Perfect/full combo (true = no misses and no slider breaks and no early finished sliders)
		bool isFC;

		// Mods used in a mask form
		ModMask modsMask;

		// Mods used in a vector of ModType
		std::vector<ModType> modsVector;

		// Life bar graph. Represents the amount of life you have at the given time
		std::vector<LifeBarPoint> lifebar;

		// Time stamp (https://msdn.microsoft.com/en-us/library/system.datetime.ticks(v=vs.110).aspx)
		OsLong timestamp;

		// Vector of of replay actions
		std::vector<Action> actions;

		// Last 4 bytes of replay file - unknown, usually 00 00 00 00
		OsLong unknown;

	private:
		// Stream reading operations
		uint8_t _GetStreamByte();
		uint16_t _GetStreamShort();
		uint32_t _GetStreamInteger();
		uint64_t _GetStreamLong();
		uint64_t _GetStreamULEB128();
		std::string _GetStreamString();

		// Data extraction which requires more work than just
		// reading byte values
		void _CalcModsVector();
		void _CalcLifebar();
		void _CalcActions();

		// Pointer to the passed data stream in constructor
		std::istream * _s;
	};
}
#endif
