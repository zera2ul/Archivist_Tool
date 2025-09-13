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
#include "settings_manager.h"
#include "path_manager.h"
#include "archiver.h"
#include "app.h"

int main()
{
	setlocale(LC_ALL, "Russian");

	App app;
	app.run();

	return 0;
}
