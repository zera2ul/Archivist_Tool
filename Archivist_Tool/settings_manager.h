#pragma once

const int PATH_MAX_LENGTH = 260;

class SettingsManager
{
private:
	const string APP_NAME = "Archivist Tool", SETTINGS_FILE_NAME = "settings.txt";

	filesystem::path settings_path;

	filesystem::path getAppDataPath()
	{
		char path[PATH_MAX_LENGTH];

		SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, path);

		return filesystem::path(path) / APP_NAME;
	}
public:
	void writeSetting(string name, string value)
	{
		FileFolderManager file_folder_manager;
		file_folder_manager.createParentDirectories(settings_path);

		ofstream settings_file(settings_path);
		string data = name + ':' + value;
		settings_file.write(data.c_str(), data.size());
	}

	SettingsManager()
	{
		settings_path = getAppDataPath() / SETTINGS_FILE_NAME;
	}
};
