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
		{{208, 144}, 'À'},
		{{208, 145}, 'Á'},
		{{208, 146}, 'Â'},
		{{208, 147}, 'Ã'},
		{{208, 148}, 'Ä'},
		{{208, 129}, 'Å'},
		{{208, 149}, '¨'},
		{{208, 150}, 'Æ'},
		{{208, 151}, 'Ç'},
		{{208, 152}, 'È'},
		{{208, 153}, 'É'},
		{{208, 154}, 'Ê'},
		{{208, 155}, 'Ë'},
		{{208, 156}, 'Ì'},
		{{208, 157}, 'Í'},
		{{208, 158}, 'Î'},
		{{208, 159}, 'Ï'},
		{{208, 160}, 'Ð'},
		{{208, 161}, 'Ñ'},
		{{208, 162}, 'Ò'},
		{{208, 163}, 'Ó'},
		{{208, 164}, 'Ô'},
		{{208, 165}, 'Õ'},
		{{208, 166}, 'Ö'},
		{{208, 167}, '×'},
		{{208, 168}, 'Ø'},
		{{208, 169}, 'Ù'},
		{{208, 170}, 'Ú'},
		{{208, 171}, 'Û'},
		{{208, 172}, 'Ü'},
		{{208, 173}, 'Ý'},
		{{208, 174}, 'Þ'},
		{{208, 175}, 'ß'},
		{{208, 176}, 'à'},
		{{208, 177}, 'á'},
		{{208, 178}, 'â'},
		{{208, 179}, 'ã'},
		{{208, 180}, 'ä'},
		{{208, 181}, 'å'},
		{{209, 145}, '¸'},
		{{208, 182}, 'æ'},
		{{208, 183}, 'ç'},
		{{208, 184}, 'è'},
		{{208, 185}, 'é'},
		{{208, 186}, 'ê'},
		{{208, 187}, 'ë'},
		{{208, 188}, 'ì'},
		{{208, 189}, 'í'},
		{{208, 190}, 'î'},
		{{208, 191}, 'ï'},
		{{209, 128}, 'ð'},
		{{209, 129}, 'ñ'},
		{{209, 130}, 'ò'},
		{{209, 131}, 'ó'},
		{{209, 132}, 'ô'},
		{{209, 133}, 'õ'},
		{{209, 134}, 'ö'},
		{{209, 135}, '÷'},
		{{209, 136}, 'ø'},
		{{209, 137}, 'ù'},
		{{209, 138}, 'ú'},
		{{209, 139}, 'û'},
		{{209, 140}, 'ü'},
		{{209, 141}, 'ý'},
		{{209, 142}, 'þ'},
		{{209, 143}, 'ÿ'},
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