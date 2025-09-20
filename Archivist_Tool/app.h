#pragma once
const string archive_path = "C:\\Utils\\archive.zip";

class App
{
private:
	const int width = 512, height = 512;
	
	Fl_Window* window;
	Fl_Check_Button* enable_context_menu_commands_checkbox;
	Fl_Button* pack_archive_button;

	void setupPackArchiveButtonCallback()
	{
		pack_archive_button->callback([](Fl_Widget* widget) {
			FileFolderManager file_folder_manager;
			vector <string> paths = file_folder_manager.readPaths();

			Archiver archiver(archive_path, paths);

			file_folder_manager.clearPaths();
			});
	}

	void setupPackArchiveButton()
	{
		pack_archive_button = new Fl_Button(0, height / 2, width, height / 2, "Pack archive");

		setupPackArchiveButtonCallback();

		pack_archive_button->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);
		pack_archive_button->labelsize(32);
		pack_archive_button->labelcolor(FL_GREEN);
		pack_archive_button->color(FL_BLACK);
		pack_archive_button->selection_color(FL_GREEN);

		pack_archive_button->show();
	}

	void setupEnableContextMenuCommandsCheckboxCallback()
	{
		enable_context_menu_commands_checkbox->callback([](Fl_Widget* widget) {
			Fl_Check_Button* enable_context_menu_commands_checkbox = (Fl_Check_Button*)(widget);
			FileFolderManager file_folder_manager;

			if (enable_context_menu_commands_checkbox->value())
				file_folder_manager.writeSetting("EnableContextMenuCommands", "True");
			else
				file_folder_manager.writeSetting("EnableContextMenuCommands", "False");
			});
	}

	void setupEnableContextMenuCommandsCheckbox()
	{
		enable_context_menu_commands_checkbox = new Fl_Check_Button(0, 0, width, height / 2, "Enable context menu commands");
		
		FileFolderManager file_folder_manager;
		string enable_context_menu_commands_value = file_folder_manager.readSetting("EnableContextMenuCommands");
		if (enable_context_menu_commands_value == "True")
			enable_context_menu_commands_checkbox->value(1);

		setupEnableContextMenuCommandsCheckboxCallback();

		enable_context_menu_commands_checkbox->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);
		enable_context_menu_commands_checkbox->labelsize(32);
		enable_context_menu_commands_checkbox->selection_color(FL_GREEN);
		enable_context_menu_commands_checkbox->labelcolor(FL_GREEN);
		
		enable_context_menu_commands_checkbox->show();
	}
public:
	App()
	{
		int x_position = (Fl::w() - width) / 2, y_position = (Fl::h() - height) / 2;
		window = new Fl_Window(x_position, y_position, width, height, "Archivist Tool");
		window->icon(LoadImage(fl_display, L"icon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE));
		window->color(FL_BLACK);

		setupEnableContextMenuCommandsCheckbox();
		setupPackArchiveButton();

		window->end();
		window->show();
	}

	int run()
	{
		return Fl::run();
	}
};