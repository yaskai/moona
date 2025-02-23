#pragma once

#include <raylib.h>

enum SoundEffect{
    SOUND_JUMP,
    SOUND_PICKUP,
    SOUND_MOONA_DAMAGE,
    SOUND_ENEMY_DAMAGE,
    SOUND_MOONA_DEATH,
    SOUND_ENEMY_DEATH,
};

typedef struct {
    Sound *sound;
    Music music[2];
} AudioPlayer;

void AudioPlayerInit(AudioPlayer *ap);
void AudioPlayerClose(AudioPlayer *ap);
void PlaySoundEffect(AudioPlayer *ap, int soundEffect);
void PlayMusic(AudioPlayer *ap);
