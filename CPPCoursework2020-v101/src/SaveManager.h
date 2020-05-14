#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

class SaveManager
{
public:
	SaveManager();

	std::string getText() { return text; }

	//Loading
	void loadFileContents(std::string filename);
	std::string getSaveData(std::string tag);
	std::string getTagContents(std::string text, std::string tag);
	std::vector<std::string> splitContentBetween(std::string text, char start, char end);
	std::vector<std::string> splitContentBy(std::string text, char split);

	//Saving
	std::ofstream* openFile(const char* path);
	
	//Takes template type T as content, wraps it in the given tag and writes it to a new line in the file
	//Definition is in header because it broke when I put it in the cpp. Using templates so I can pass whatever type I want without needing excessive amounts of overloaded functions
	//Use with caution - only pass types that an fstream can take
	template <class T>
	void writeLine(std::string tag, T content)
	{
		//Make the input string into a tag
		tag.insert(0, "<");
		tag.push_back('>');

		//Copy tag but add a / for the end tag
		std::string endTag = tag;
		endTag.insert(1, "/");

		file << tag << content << endTag << std::endl;
	}

	void closeFile();

private:
	std::string text;
	std::ofstream file;

};

