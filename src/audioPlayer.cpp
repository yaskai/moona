#include "raylib.h"
#include <cstdlib>
#include "audioPlayer.hpp"

void AudioPlayerInit(AudioPlayer *ap) {
InitAudioDevice();
ap->sound = (Sound *)malloc(sizeof(Sound) * 12);

ap->sound[SOUND_PICKUP] = LoadSoundFromWave(LoadWave("sfx/sound/Flower_Collect.wav"));
ap->sound[SOUND_JUMP] = LoadSoundFromWave(LoadWave("sfx/sound/Jump.wav"));
ap->sound[SOUND_MOONA_DAMAGE] = LoadSoundFromWave(LoadWave("sfx/sound/Moona_getting_Hit.wav"));
ap->sound[SOUND_ENEMY_DAMAGE] = LoadSoundFromWave(LoadWave("sfx/sound/Alien_Hit.wav"));
ap->sound[SOUND_MOONA_DEATH] = LoadSoundFromWave(LoadWave("sfx/sound/Moona_Death.wav"));
ap->sound[SOUND_ENEMY_DEATH] = LoadSoundFromWave(LoadWave("sfx/sound/Alien_Death.wav"));

ap->music[0] = LoadMusicStream("sfx/sound/CyberZeus_Fin (Game Music).mp3");
PlayMusicStream(ap->music[0]);
SetMusicVolume(ap->music[0], 0.3f);

ap->music[1] = LoadMusicStream("sfx/sound/Milk_Attack.wav");
}

void AudioPlayerClose(AudioPlayer *ap) {
CloseAudioDevice();
UnloadMusicStream(ap->music[0]);
UnloadMusicStream(ap->music[1]);
}

void PlaySoundEffect(AudioPlayer *ap, int soundEffect) {
PlaySound(ap->sound[soundEffect]);
}
