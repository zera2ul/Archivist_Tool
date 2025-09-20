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

using namespace std;

#include "file_folder_manager.h"
#include "path_manager.h"
#include "archiver.h"
#include "app.h"

int main(int argc, char* argv[])
{
	if (argc == 1)
	{
		App app;
		return app.run();
	}
	else if (argc == 2)
	{
		FileFolderManager file_folder_manager;
		file_folder_manager.writePath(argv[1]);
		return 0;
	}
}
