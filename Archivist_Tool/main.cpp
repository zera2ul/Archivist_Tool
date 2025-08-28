#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <Windows.h>
#include <ShlObj.h>
#include <zip.h>

using namespace std;

#include "file_folder_manager.h"
#include "settings_manager.h"
#include "path_manager.h"
#include "archiver.h"

int main()
{
	setlocale(LC_ALL, "Russian");

	SettingsManager settings_manager;
	settings_manager.writeSetting("EnableContextMenuCommands", "True");

	return 0;
}
