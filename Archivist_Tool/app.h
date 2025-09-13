#pragma once
class App
{
private:
	const int width = 512, height = 512;
	Fl_Window* window;

	void setupEnableContextMenuCommandsCheckbox()
	{
		Fl_Check_Button* context_menu_commands_checkbox = new Fl_Check_Button(0, 0, width, height, "Enable context menu commands");
		
		SettingsManager settings_manager;
		string enable_context_menu_commands_value = settings_manager.readSetting("EnableContextMenuCommands");
		if (enable_context_menu_commands_value == "True")
			context_menu_commands_checkbox->value(1);

		context_menu_commands_checkbox->callback([](Fl_Widget* widget, void* data) {
			Fl_Check_Button* context_menu_commands_checkbox = (Fl_Check_Button*)(widget);
			SettingsManager settings_manager;

			if (context_menu_commands_checkbox->value())
				settings_manager.writeSetting("EnableContextMenuCommands", "True");
			else
				settings_manager.writeSetting("EnableContextMenuCommands", "False");
			});

		context_menu_commands_checkbox->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);
		context_menu_commands_checkbox->labelsize(32);
		
		context_menu_commands_checkbox->show();
	}
public:
	App()
	{
		int x_position = Fl::w() / 2  - width / 2, y_position = Fl::h() / 2 - height / 2;
		window = new Fl_Window(x_position, y_position, width, height, "Archivist Tool");

		setupEnableContextMenuCommandsCheckbox();

		window->end();
		window->show();
	}

	int run()
	{
		return Fl::run();
	}
};