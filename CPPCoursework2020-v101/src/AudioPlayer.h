#pragma once
#include <map>

class AudioPlayer
{
public:

	AudioPlayer();
	~AudioPlayer();

	int audioInit();
	void playAudio(const char* audioPath, int audioChannel, int loops);
	void playMusic(const char* musicPath, int loops);
	bool isSongPlaying(const char* musicPath);
	void stopMusic();
	void closeAudio();

private:
	//Store a cache of pointers to existing sound and music
	//When playing a sound/music, first check if it is already cached
	//When the AudioPlayer is deleted, free all Mix_Chunk and Mix_Music pointers stored
	std::map<std::string,Mix_Chunk*> sfx;
	std::map<std::string,Mix_Music*> music;

	std::string currentSong;
};

