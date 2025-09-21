#pragma once

class Archiver
{
private:
	zip_t* archive;
	vector <filesystem::path> base_entries;
	filesystem::path base_path;

	void unpackFile(filesystem::path absolute_path, zip_int64_t index)
	{
		FileFolderManager file_folder_manager;
		file_folder_manager.createParentDirectories(absolute_path);
		ofstream output_file(absolute_path, ios::binary);

		zip_file_t* archive_file = zip_fopen_index(archive, index, 0);
		char buffer[4096];
		zip_int64_t bytes_count;
		while (bytes_count = zip_fread(archive_file, buffer, sizeof(buffer)))
			output_file.write(buffer, bytes_count);

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

			PathManager path_manager;
			path_manager.setPath(base_path / filesystem::path(stat.name));
			filesystem::path repaired_path = path_manager.getRepairedPath();
			
			if (repaired_path != "")
				unpackFile(repaired_path, index);
		}
	}

	void packFile(filesystem::path absolute_path, filesystem::path relative_path)
	{
		PathManager path_manager;
		path_manager.setPath(absolute_path);
		zip_source_t* source = zip_source_file(archive, path_manager.getUtf8Path().c_str(), 0, 0);

		path_manager.setPath(relative_path);
		zip_file_add(archive, path_manager.getUtf8Path().c_str(), source, ZIP_FL_ENC_UTF_8);
	}

	bool isFile(filesystem::path entry)
	{
		return entry.string().find('.') != -1;
	}

	vector <pair <filesystem::path, filesystem::path>> collectSubEntries(filesystem::path directory_absolute_path)
	{
		vector < pair <filesystem::path, filesystem::path>> entries;
		for (auto& entry : filesystem::recursive_directory_iterator(directory_absolute_path))
		{
			filesystem::path entry_absolute_path = entry.path();
			filesystem::path relative_path = filesystem::relative(entry_absolute_path, directory_absolute_path.parent_path());

			entries.emplace_back(entry_absolute_path, relative_path);
		}

		return entries;
	}

	void packDirectory(filesystem::path absolute_path)
	{
		vector <pair <filesystem::path, filesystem::path>> sub_entries = collectSubEntries(absolute_path);
		for (auto& [absolute_path, relative_path]: sub_entries)
		{
			if (isFile(relative_path))
				packFile(absolute_path, relative_path);
			else
				zip_dir_add(archive, relative_path.string().c_str(), ZIP_FL_ENC_UTF_8);
		}
	}

	void packArchive()
	{
		for (filesystem::path& base_entry : base_entries)
		{
			if (isFile(base_entry))
				packFile(base_entry, base_entry.filename());
			else
				packDirectory(base_entry);
		}
	}

	vector <filesystem::path> strings_to_paths(vector <string> entries)
	{
		vector <filesystem::path> entries_paths;
		for (auto& entry : entries)
			entries_paths.emplace_back(filesystem::path(entry));

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
		base_entries(strings_to_paths(base_entries))
	{
		archive = zip_open(archive_path.c_str(), ZIP_CREATE | ZIP_TRUNCATE, nullptr);
		packArchive();
		zip_close(archive);
	}
};