#include <iostream>
#include <string>
#include <fstream>

#include "archiver.h"

using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");

	streambuf* console_input = cin.rdbuf();
	ifstream file_input("pack.txt");
	cin.rdbuf(file_input.rdbuf());

	string operation, archive_path;
	cin >> operation >> archive_path;

	if (operation == "PACK")
	{
		vector <string> archive_files;
		string archive_file;
		while (cin >> archive_file)
			archive_files.push_back(archive_file);

		Archiver archiver(archive_path, archive_files);
	}
	else if (operation == "UNPACK")
		Archiver archiver(archive_path);
	
	cin.rdbuf(console_input);
	file_input.close();

	return 0;
}
