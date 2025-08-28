#pragma once

class FileFolderManager
{
public:
	void createParentDirectories(filesystem::path path)
	{
		filesystem::path parent_path = path.parent_path();
		if (!filesystem::exists(parent_path))
			filesystem::create_directories(parent_path);
	}
};