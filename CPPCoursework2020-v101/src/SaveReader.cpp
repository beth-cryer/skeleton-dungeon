#include "header.h"
#include "SaveReader.h"

#include <iostream>
#include <fstream>

SaveReader::SaveReader()
{

}

void SaveReader::getFile(std::string filename)
{
    std::string buffer;
    char c;

    std::ifstream in(filename);
    if (!in) exit(1);

    while (in.get(c)) buffer += c;
    in.close();

    text = buffer;
}

//A very basic file reader that looks for a set of matching XML tags and returns the string between them
//Using this method every tag has to be unique, but that's fine for our purposes
std::string SaveReader::getData(std::string tag)
{
    //Make the input string into a tag
    tag.insert(0, "<");
    tag.push_back('>');

    //Copy tag but add a / for the end tag
    std::string endTag = tag;
    endTag.insert(1, "/");

    //Get the substring between the start and end tag
    size_t start = text.find(tag) + tag.length();
    size_t end = text.find(endTag);
    std::string out = text.substr(start, end-start);

    std::cout << out;
    return out;
}