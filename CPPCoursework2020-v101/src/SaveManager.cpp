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
    std::string out = text.substr(start, end-start);

    std::cout << out;
    return out;
}

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

//Helper function that automatically wraps up text content inside the tag format and writes it to the save file on a new line
void SaveManager::writeLine(std::string tag, std::string content)
{
    //Make the input string into a tag
    tag.insert(0, "<");
    tag.push_back('>');

    //Copy tag but add a / for the end tag
    std::string endTag = tag;
    endTag.insert(1, "/");

    file << tag << content << endTag << std::endl;
}

void SaveManager::closeFile()
{
    file.close();
}