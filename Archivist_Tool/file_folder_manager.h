#pragma once

const int PATH_MAX_LENGTH = 260;

class FileFolderManager
{
private:
	filesystem::path app_data_path, settings_path, paths_path;

	filesystem::path getAppDataPath()
	{
		char path[PATH_MAX_LENGTH];

		SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, path);

		return filesystem::path(path) / "Archivist_Tool";
	}
public:
	void clearPaths()
	{
		ofstream paths_file(paths_path);
		paths_file << "";
	}

	vector <string> readPaths()
	{
		streambuf* console_input = cin.rdbuf();
		ifstream paths_file(paths_path);
		cin.rdbuf(paths_file.rdbuf());

		vector <string> paths;
		string paths_line;
		while (getline(cin, paths_line))
			paths.push_back(paths_line);

		cin.rdbuf(console_input);
		paths_file.close();

		return paths;
	}

	void writePath(string path)
	{
		string paths;
		for (auto path_line : readPaths())
			paths += path_line + '\n';
		paths += path + '\n';

		FileFolderManager file_folder_manager;
		file_folder_manager.createParentDirectories(paths_path);

		ofstream paths_file(paths_path);
		paths_file << paths;
	}	

	vector <string> readSettings()
	{
		streambuf* console_input = cin.rdbuf();
		ifstream settings_file(settings_path);
		cin.rdbuf(settings_file.rdbuf());

		vector <string> settings;
		string setting;
		while (getline(cin, setting))
			settings.push_back(setting);

		cin.rdbuf(console_input);
		settings_file.close();

		return settings;
	}

	string readSetting(string name)
	{
		vector <string> settings = readSettings();
		for (auto &setting : settings)
			if (setting.find(name) != -1)
				return setting.substr(setting.find(':') + 1);

		return "";
	}

	void writeSetting(string name, string value)
	{
		vector <string> settings = readSettings();

		string rewrited_settings;
		for (auto &setting : settings)
		{
			string name_line = setting.substr(0, setting.find(':')), value_line = setting.substr(setting.find(':') + 1);

			if (name_line != name)
				rewrited_settings += name_line + ':' + value_line + '\n';
			else
				rewrited_settings += name + ':' + value + '\n';
		}
		
		if (rewrited_settings.find(name) == -1)
			rewrited_settings += name + ':' + value + '\n';

		FileFolderManager file_folder_manager;
		file_folder_manager.createParentDirectories(settings_path);

		ofstream settings_file(settings_path);
		settings_file << rewrited_settings;
	}

	void createParentDirectories(filesystem::path path)
	{
		filesystem::path parent_path = path.parent_path();
		if (!filesystem::exists(parent_path))
			filesystem::create_directories(parent_path);
	}

	FileFolderManager()
	{
		app_data_path = getAppDataPath();
		settings_path = app_data_path / "settings.txt";
		paths_path = app_data_path / "paths.txt";
	}
};