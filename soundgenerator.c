#include "includes.h"
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

int soundcounter = 0;

int SamplesPerSecond = 30000;
int BytesPerSample = sizeof(int16) * 2;
void SDLAudioCallback(void *UserData, Uint8 *AudioData, int Length)
{
  if (!audio1 || !audio2)
    return;

  int toneHz1 = 1000;
  int toneHz2 = toneHz1;
  if (audio1->frequency > 0)
    toneHz1 /= audio1->frequency;
  if (audio2->frequency > 0)
    toneHz2 /= audio2->frequency;
  int16 volume1 = 96 * audio1->volume;
  int16 volume2 = 96 * audio2->volume;
  uint32 RunningSampleIndex = 0;
  int period1 = SamplesPerSecond / toneHz1;
  int period2 = SamplesPerSecond / toneHz2;
  int halfPeriod1 = period1 / 2;
  if (halfPeriod1 == 0) 
    halfPeriod1 = 2;
  int halfPeriod2 = period2 / 2;
  if (halfPeriod2 == 0) 
    halfPeriod2 = 2;
  int BytesPerSample = sizeof(int16) * 2;

  int TargetQueueBytes = SamplesPerSecond * BytesPerSample;
  int BytesToWrite = TargetQueueBytes - SDL_GetQueuedAudioSize(1);
 
  if (Length) {
    void *SoundBuffer = malloc(BytesToWrite);
    int16 *SampleOut = (int16 *)SoundBuffer;
    int SampleCount = BytesToWrite/BytesPerSample;
    for(int SampleIndex = 0; SampleIndex < SampleCount; ++SampleIndex) {
      int16 sampleVal1 = ((RunningSampleIndex++ / halfPeriod1) % 2)? 
	volume1 : -volume1;
      int16 sampleVal2 = ((RunningSampleIndex++ / halfPeriod2) % 2)? 
	volume2 : -volume2;
      *SampleOut++ = sampleVal1 + sampleVal2;
      *SampleOut++ = sampleVal1 + sampleVal2;
    }

    memcpy(AudioData, SoundBuffer, Length);
    free(SoundBuffer);
  }
}

void
SDLInitAudio(int32_t SamplesPerSecond, int32_t BufferSize)
{
  SDL_AudioSpec AudioSettings = {0};

  AudioSettings.freq = SamplesPerSecond;
  AudioSettings.format = AUDIO_S16LSB;
  AudioSettings.channels = 2;
  AudioSettings.samples = BufferSize / 2;
  AudioSettings.callback = &SDLAudioCallback;


  int i, count = SDL_GetNumAudioDevices(0);

  for (i = 0; i < count; ++i) {
    SDL_Log("Audio device %d: %s", i, SDL_GetAudioDeviceName(i, 0));
  }

  SDL_OpenAudio(&AudioSettings, 0);

  printf("Initialised an Audio device at frequency %d Hz, %d Channels\n",
	 AudioSettings.freq, AudioSettings.channels);

  if (AudioSettings.format != AUDIO_S16LSB)
    {
      printf("Oops! We didn't get AUDIO_S16LSB as our sample format!\n");
      SDL_CloseAudio();
    }

  SDL_PauseAudio(0);
}

SoundGenerator* SoundGeneratorCreate()
{
  //  SDLInitAudio(48000, SamplesPerSecond * BytesPerSample / 60);
  SoundGenerator* sg =  (SoundGenerator*)calloc(1, sizeof(SoundGenerator));
  return sg;
}


