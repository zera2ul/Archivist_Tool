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
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/x.H>
#include <FL/Fl_Check_Button.H>
#include <shellapi.h>

using namespace std;

#include "file_folder_manager.h"
#include "path_manager.h"
#include "archiver.h"
#include "app.h"

int main(int argc, char* argv[])
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	if (argc == 1)
	{
		App app;
		return app.run();
	}
	else if (argc == 3)
	{
		if (string(argv[1]) == "pack")
		{
			FileFolderManager file_folder_manager;
			file_folder_manager.writePath(argv[2]);
		}
		else if (string(argv[1]) == "unpack")
			Archiver archiver(argv[2]);

		return 0;
	}
}
