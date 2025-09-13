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
#include <FL/Fl_Box.H>

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

	Fl_Window* window = new Fl_Window(340, 180);

	Fl_Box* box = new Fl_Box(20, 40, 300, 100, "Archivist Tool");
	box->box(FL_UP_BOX);

	window->end();

	window->show();

	return Fl::run();
}
