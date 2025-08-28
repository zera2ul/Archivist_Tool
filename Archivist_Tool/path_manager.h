#pragma once
#include <string>
#include <vector>
#include <map>
#include <filesystem>

using namespace std;

class PathManager
{
private:
	map <pair <int, int>, char> utf8_cyrillic_characters = {
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

	string path;
	filesystem::path repaired_path;
	
	u8string getUtf8PathPart(string path_part)
	{
		u8string utf8_path;
		for (int i = 0; i < path_part.size(); i++)
			utf8_path.push_back(path_part[i]);

		return utf8_path;
	}

	vector <int> getUtf8CharactersCodes(string path_part)
	{
		vector <int> utf8_codes;
		for (char8_t character : getUtf8PathPart(path_part))
			utf8_codes.push_back(character);

		return utf8_codes;
	}

	string asCyrillic(string path_part)
	{
		vector <int> utf8_characters_codes = getUtf8CharactersCodes(path_part);		
		if (utf8_characters_codes.size() % 2 == 1)
			return "";

		string repaired_path_part;
		for (int index = 0; index < utf8_characters_codes.size(); index += 2)
		{
			pair <int, int> utf8_character_codes = { utf8_characters_codes[index], utf8_characters_codes[index + 1] };
			if (utf8_cyrillic_characters.find(utf8_character_codes) == utf8_cyrillic_characters.end())
				return "";
			
			repaired_path_part.push_back(utf8_cyrillic_characters[utf8_character_codes]);
		}

		return repaired_path_part;
	}

	string asAscii(string path_part)
	{
		for (auto chr : path_part)
			if (!isascii(chr))
				return "";

		return path_part;
	}

	string getRepairedPathPart(string path_part)
	{
		string repaired_path_part = asAscii(path_part);
		if (repaired_path_part != "")
			return path_part;

		repaired_path_part = asCyrillic(path_part);
		if (repaired_path_part != "")
			return repaired_path_part;

		return "";
	}

	void repairPathPart(int current_index, int separator_index)
	{
		string path_part = path.substr(current_index, separator_index - current_index);

		string repaired_path_part = getRepairedPathPart(path_part);
		if (repaired_path_part != "")
			if (repaired_path == "")
				repaired_path = repaired_path_part;
			else
				repaired_path = repaired_path.string() + "\\" + repaired_path_part;
		else
			repaired_path = "";
	}
public:
	filesystem::path getRepairedPath()
	{
		int current_index = 0;
		while (true)
		{
			int separator_index = path.find('\\', current_index);
			if (separator_index != -1)
			{
				repairPathPart(current_index, separator_index);
				if (repaired_path == "")
					return "";

				current_index = separator_index + 1;
			}
			else
			{
				separator_index = path.find('.', current_index);
				repairPathPart(current_index, separator_index);
				if (repaired_path == "")
					return "";

				repaired_path += path.substr(separator_index);

				return repaired_path;
			}
		}
	}

	string getUtf8Path()
	{
		u8string path_u8 = filesystem::path(path).u8string();

		return string(path_u8.begin(), path_u8.end());
	}
	
	void setPath(filesystem::path path_value)
	{
		path = path_value.string();
	}
};