#include "header.h"
#include "AudioPlayer.h"

//Call initialisation function automatically on creation
AudioPlayer::AudioPlayer()
{
	audioInit();
}

//Free all sound and music on deletion
AudioPlayer::~AudioPlayer() {

}

int AudioPlayer::audioInit()
{
	//Initialize SDL Audio
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
		return 1;

	//Initialise SDL Mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		return 1;
	}

	Mix_VolumeMusic(MIX_MAX_VOLUME / 4);

	return 0;
}

void AudioPlayer::playAudio(const char* audioPath, int audioChannel, int loops)
{
	//Check for an existing entry, pull pAudio from that if it exists
	if (sfx.count(audioPath) > 0) {
		Mix_PlayChannel(audioChannel, sfx.at(audioPath), loops); 

	//Entry not found:
	} else {
		Mix_Chunk* pAudio = Mix_LoadWAV(audioPath);
		Mix_PlayChannel(audioChannel, pAudio, loops);

		sfx.emplace(audioPath, pAudio); //cache pAudio
	}

}

void AudioPlayer::playMusic(const char* musicPath, int loops)
{
	//Check for an existing entry, pull pMusic from that if it exists
	if (music.count(musicPath) > 0) {
		Mix_PlayMusic(music.at(musicPath), loops);

		//Entry not found:
	}
	else {
		Mix_Music* pMusic = Mix_LoadMUS(musicPath);
		Mix_PlayMusic(pMusic, loops);

		music.emplace(musicPath, pMusic); //cache pMusic
	}

	//Track currently-playing song, so we can check if a song is already playing before we accidentally restart it
	currentSong = musicPath;
}

bool AudioPlayer::isSongPlaying(const char* musicPath)
{
	return (currentSong == musicPath);
}

void AudioPlayer::stopMusic() {
	if (Mix_PlayingMusic() == 1) {
		Mix_HaltMusic();
	}
}

//Free all pointers used, then quit the Mixer
void AudioPlayer::closeAudio() {

	//Loop through all audio and music cached and free them
	//(doesn't seem to have memory leaks either way, but I'd rather be safe and keep track of all the mix pointers I make)
	for (auto it = sfx.begin(); it != sfx.end(); ++it) {
		Mix_FreeChunk(it->second);
	}

	for (auto it = music.begin(); it != music.end(); ++it) {
		Mix_FreeMusic(it->second);
	}

	Mix_Quit();
}