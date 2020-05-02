#pragma once
#include <string>
#include <iostream>
#include <fstream>

class SaveManager
{
public:
	SaveManager();

	//Loading
	void readFileContents(std::string filename);
	std::string getData(std::string tag);

	//Saving
	void openFile(const char* path);
	void closeFile();
	void writeFile(std::string text);

private:
	std::string text;
	std::ofstream file;

};

