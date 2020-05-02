#pragma once
#include <string>

class SaveReader
{
public:
	SaveReader();

	void getFile(std::string filename);
	std::string getData(std::string tag);

private:
	std::string text;

};

