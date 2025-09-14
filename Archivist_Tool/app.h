#pragma once
class App
{
private:
	const int width = 512, height = 512;
	Fl_Window* window;
	Fl_Check_Button* enable_context_menu_commands_checkbox;

	void setupEnableContextMenuCommandsCheckboxCallback()
	{
		enable_context_menu_commands_checkbox->callback([](Fl_Widget* widget, void* data) {
			Fl_Check_Button* enable_context_menu_commands_checkbox = (Fl_Check_Button*)(widget);
			SettingsManager settings_manager;

			if (enable_context_menu_commands_checkbox->value())
				settings_manager.writeSetting("EnableContextMenuCommands", "True");
			else
				settings_manager.writeSetting("EnableContextMenuCommands", "False");
			});
	}

	void setupEnableContextMenuCommandsCheckbox()
	{
		enable_context_menu_commands_checkbox = new Fl_Check_Button(0, 0, width, height, "Enable context menu commands");
		
		SettingsManager settings_manager;
		string enable_context_menu_commands_value = settings_manager.readSetting("EnableContextMenuCommands");
		if (enable_context_menu_commands_value == "True")
			enable_context_menu_commands_checkbox->value(1);

		setupEnableContextMenuCommandsCheckboxCallback();

		enable_context_menu_commands_checkbox->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);
		enable_context_menu_commands_checkbox->labelsize(32);
		enable_context_menu_commands_checkbox->selection_color(FL_BLUE);
		enable_context_menu_commands_checkbox->labelcolor(FL_BLUE);
		
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

		window->end();
		window->show();
	}

	int run()
	{
		return Fl::run();
	}
};