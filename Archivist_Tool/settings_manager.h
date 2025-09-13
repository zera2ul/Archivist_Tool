#pragma once

const int PATH_MAX_LENGTH = 260;

class SettingsManager
{
private:
	filesystem::path settings_path;

	filesystem::path getAppDataPath()
	{
		char path[PATH_MAX_LENGTH];

		SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, path);

		return filesystem::path(path) / "Archivist_Tool";
	}
public:
	string readSetting(string name)
	{
		streambuf* console_input = cin.rdbuf();
		ifstream settings_file(settings_path);
		cin.rdbuf(settings_file.rdbuf());

		string setting;
		while (getline(cin, setting))
		{

			if (setting.find(name) != -1)
				return setting.substr(setting.find(":") + 1);
		}

		cin.rdbuf(console_input);
		settings_file.close();
	}
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
		settings_path = getAppDataPath() / "settings.txt";
	}
};
