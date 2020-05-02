#pragma once
#include <string>
#include <iostream>
#include <fstream>

class SaveManager
{
public:
	SaveManager();

	//Loading
	void loadFileContents(std::string filename);
	std::string getSaveData(std::string tag);

	//Saving
	std::ofstream* openFile(const char* path);
	void writeLine(std::string tag, std::string content);
	void closeFile();

private:
	std::string text;
	std::ofstream file;

};

