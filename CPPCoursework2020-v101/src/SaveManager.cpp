#include "header.h"
#include "SaveManager.h"

SaveManager::SaveManager()
{

}

void SaveManager::loadFileContents(std::string filename)
{
    std::string buffer;
    char c;

    std::ifstream in(filename);
    if (!in) exit(1);

    while (in.get(c)) buffer += c;
    in.close();

    //Save the file contents to this object, then return it too in case something else wants to use it
    text = buffer;
}

//A very basic file reader that looks for a set of matching XML tags and returns the string between them
//Using this method every tag has to be unique, but that's fine for our purposes
std::string SaveManager::getSaveData(std::string tag)
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
    std::string out = text.substr(start, end - start);

    std::cout << out;
    return out;
}

//Same as above, but text is passed as a parameter
std::string SaveManager::getTagContents(std::string text, std::string tag)
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
    std::string out = text.substr(start, end - start);

    return out;
}

//Returns a vector of each substring contained between the given start and end characters
std::vector<std::string> SaveManager::splitContentBetween(std::string text, char start, char end)
{
    std::vector<std::string> substrs;
    std::string buf;

    //Loop through the text:
    for (auto it = text.begin(); it != text.end(); it++) {

        //Ignore whitespace and newlines
        if (*it == ' ' || *it == '\n') continue;

        //Start of brackets - reset buffer
        if (*it == start)
            buf.clear();

        //End of brackets - add buffer to substrs
        else if (*it == end)
            substrs.push_back(buf);

        else {

            //If within the start and end bounds, add to buffer
            buf.push_back(*it);
        }
    }

    return substrs;
}

//Returns a vector of each substring in the text separated by a given char (eg. comma)
std::vector<std::string> SaveManager::splitContentBy(std::string text, char split)
{
    std::vector<std::string> substrs;
    std::string buf;

    //Loop through the text:
    for (auto it = text.begin(); it != text.end(); it++) {

        //Ignore whitespace and newlines
        if (*it == ' ' || *it == '\n') continue;

        //Start of brackets - reset buffer
        if (*it == split) {
            substrs.push_back(buf);
            buf.clear();
        } else {

            //Add to buffer
            buf.push_back(*it);
        }
    }
    return substrs;
}


//Writing

//Opens file (after checking if it exists, and creating it if not)
std::ofstream* SaveManager::openFile(const char* path)
{
    //Check if save file exists - create it if not
    std::ifstream check (path);
    if (!check.good()) std::ofstream create { path }; //creates file and closes the ofstream immediately

    check.close(); //manually close ifstream used to check filepath

    //Open the new/existing file
    file.open(path);

    return &file;
}

void SaveManager::closeFile()
{
    file.close();
}