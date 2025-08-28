#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <zip.h>

#include "path_manager.h"

using namespace std;

class Archiver
{
private:
	zip_t* archive;
	vector <filesystem::path> base_entries;
	filesystem::path base_path;

	void createParentDirectories(filesystem::path absolute_path)
	{
		absolute_path = absolute_path.parent_path();
		if (!filesystem::exists(absolute_path))
		{
			filesystem::create_directories(absolute_path);
		}
	}

	string getPathU8(filesystem::path path)
	{
		u8string path_u8 = path.u8string();
		return string(path_u8.begin(), path_u8.end());
	}

	void unpackFile(filesystem::path absolute_path, zip_int64_t index)
	{
		createParentDirectories(absolute_path);
		ofstream output_file(absolute_path, ios::binary);

		zip_file_t* archive_file = zip_fopen_index(archive, index, 0);
		char buffer[4096];
		zip_int64_t bytes_count;
		while (bytes_count = zip_fread(archive_file, buffer, sizeof(buffer)))
		{
			output_file.write(buffer, bytes_count);
		}

		output_file.close();
		zip_fclose(archive_file);
	}

	void unpackArchive()
	{
		zip_int64_t entries_count = zip_get_num_entries(archive, 0);
		for (zip_int64_t index = 0; index < entries_count; index++)
		{
			zip_stat_t stat;
			zip_stat_index(archive, index, ZIP_FL_ENC_UTF_8, &stat);

			PathManager path_manager(base_path / filesystem::path(stat.name));
			filesystem::path repaired_path = path_manager.getRepairedPath();
			
			if (repaired_path != "")
			{
				unpackFile(repaired_path, index);
			}
		}
	}

	void packFile(filesystem::path absolute_path, filesystem::path relative_path)
	{
		zip_source_t* source = zip_source_file(archive, getPathU8(absolute_path).c_str(), 0, 0);
		zip_file_add(archive, getPathU8(relative_path).c_str(), source, ZIP_FL_ENC_UTF_8);
	}

	bool isFile(filesystem::path entry)
	{
		return entry.string().find('.') != -1;
	}

	vector <pair <filesystem::path, filesystem::path>> collectSubEntries(filesystem::path absolute_path)
	{
		vector < pair <filesystem::path, filesystem::path>> entries;
		for (auto& entry : filesystem::recursive_directory_iterator(absolute_path))
		{
			filesystem::path absolute_path = entry.path();
			filesystem::path relative_path = filesystem::relative(absolute_path, base_path);

			entries.emplace_back(absolute_path, relative_path);
		}

		return entries;
	}

	void packDirectory(filesystem::path absolute_path)
	{
		vector <pair <filesystem::path, filesystem::path>> sub_entries = collectSubEntries(absolute_path);
		for (auto& [absolute_path, relative_path]: sub_entries)
		{
			if (isFile(relative_path))
			{
				packFile(absolute_path, relative_path);
			}
			else
			{
				zip_dir_add(archive, relative_path.string().c_str(), ZIP_FL_ENC_UTF_8);
			}
		}
	}

	void packArchive()
	{
		for (filesystem::path& base_entry : base_entries)
		{
			if (isFile(base_entry))
			{
				packFile(base_entry, base_entry.filename());
			}
			else
			{
				packDirectory(base_entry);
			}
		}
	}

	vector <filesystem::path> strings_to_paths(vector <string> entries)
	{
		vector <filesystem::path> entries_paths;
		for (auto& entry : entries)
		{
			entries_paths.emplace_back(filesystem::path(entry));
		}

		return entries_paths;
	}
public:
	Archiver(string archive_path) : base_path(filesystem::path(archive_path).parent_path())
	{
		archive = zip_open(archive_path.c_str(), ZIP_RDONLY, nullptr);
		unpackArchive();
		zip_close(archive);
	}

	Archiver(string archive_path, vector <string> base_entries) :
		base_path(filesystem::path(archive_path).parent_path()),
		base_entries(strings_to_paths(base_entries))
	{
		archive = zip_open(archive_path.c_str(), ZIP_CREATE | ZIP_TRUNCATE, nullptr);
		packArchive();
		zip_close(archive);
	}
};