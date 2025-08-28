#pragma once
#include <string>
#include <vector>
#include <map>
#include <filesystem>

using namespace std;

class PathManager
{
private:
	map <pair <int, int>, char> utf8_characters = {
		{{208, 144}, '�'},
		{{208, 145}, '�'},
		{{208, 146}, '�'},
		{{208, 147}, '�'},
		{{208, 148}, '�'},
		{{208, 129}, '�'},
		{{208, 149}, '�'},
		{{208, 150}, '�'},
		{{208, 151}, '�'},
		{{208, 152}, '�'},
		{{208, 153}, '�'},
		{{208, 154}, '�'},
		{{208, 155}, '�'},
		{{208, 156}, '�'},
		{{208, 157}, '�'},
		{{208, 158}, '�'},
		{{208, 159}, '�'},
		{{208, 160}, '�'},
		{{208, 161}, '�'},
		{{208, 162}, '�'},
		{{208, 163}, '�'},
		{{208, 164}, '�'},
		{{208, 165}, '�'},
		{{208, 166}, '�'},
		{{208, 167}, '�'},
		{{208, 168}, '�'},
		{{208, 169}, '�'},
		{{208, 170}, '�'},
		{{208, 171}, '�'},
		{{208, 172}, '�'},
		{{208, 173}, '�'},
		{{208, 174}, '�'},
		{{208, 175}, '�'},
		{{208, 176}, '�'},
		{{208, 177}, '�'},
		{{208, 178}, '�'},
		{{208, 179}, '�'},
		{{208, 180}, '�'},
		{{208, 181}, '�'},
		{{209, 145}, '�'},
		{{208, 182}, '�'},
		{{208, 183}, '�'},
		{{208, 184}, '�'},
		{{208, 185}, '�'},
		{{208, 186}, '�'},
		{{208, 187}, '�'},
		{{208, 188}, '�'},
		{{208, 189}, '�'},
		{{208, 190}, '�'},
		{{208, 191}, '�'},
		{{209, 128}, '�'},
		{{209, 129}, '�'},
		{{209, 130}, '�'},
		{{209, 131}, '�'},
		{{209, 132}, '�'},
		{{209, 133}, '�'},
		{{209, 134}, '�'},
		{{209, 135}, '�'},
		{{209, 136}, '�'},
		{{209, 137}, '�'},
		{{209, 138}, '�'},
		{{209, 139}, '�'},
		{{209, 140}, '�'},
		{{209, 141}, '�'},
		{{209, 142}, '�'},
		{{209, 143}, '�'},
	};

	string path, path_part, repaired_path_part;
	filesystem::path repaired_path;
	int current_index, separator_index;
	
	u8string getUtf8PathPart()
	{
		u8string utf8_path;
		for (int i = 0; i < path_part.size(); i++)
		{
			utf8_path.push_back(path_part[i]);
		}

		return utf8_path;
	}

	vector <int> getUtf8CharactersCodes()
	{
		vector <int> utf8_codes;
		for (char8_t character : getUtf8PathPart())
		{
			utf8_codes.push_back(character);
		}

		return utf8_codes;
	}

	bool isPathPartCyrillic()
	{
		vector <int> utf8_characters_codes = getUtf8CharactersCodes();

		if (utf8_characters_codes.size() % 2 == 1)
		{
			return false;
		}

		for (int index = 0; index < utf8_characters_codes.size(); index += 2)
		{
			pair <int, int> utf8_character_codes = { utf8_characters_codes[index], utf8_characters_codes[index + 1] };
			if (utf8_characters.find(utf8_character_codes) == utf8_characters.end())
			{
				return false;
			}
			else
			{
				repaired_path_part.push_back(utf8_characters[utf8_character_codes]);
			}
		}

	}

	bool isPathPartAscii()
	{
		for (auto chr : path_part)
		{
			if (isascii(chr))
			{
				continue;
			}

			return false;
		}

		return true;
	}

	string getRepairedPathPart()
	{
		if (isPathPartAscii())
			return path_part;
		else if (isPathPartCyrillic())
			return repaired_path_part;
		else
			return "";
	}

	void repairPathPart()
	{
		path_part = path.substr(current_index, separator_index - current_index);

		repaired_path_part = getRepairedPathPart();
		if (repaired_path_part != "")
		{
			if (repaired_path == "")
				repaired_path = repaired_path_part;
			else
				repaired_path = repaired_path.string() + "\\" + repaired_path_part;

			repaired_path_part = "";
		}
		else
			repaired_path = "";
	}
public:
	filesystem::path getRepairedPath()
	{
		while (true)
		{
			separator_index = path.find('\\', current_index);
			if (separator_index != -1)
			{
				repairPathPart();
				if (repaired_path == "")
					return "";

				current_index = separator_index + 1;
				continue;
			}
			
			separator_index = path.find('.', current_index);
			repairPathPart();
			repaired_path += path.substr(separator_index);

			return repaired_path;
		}
	}

	PathManager(filesystem::path path) : path(path.string()), path_part(""), repaired_path_part(""),
		repaired_path(""), current_index(0), separator_index(-1) {}
};