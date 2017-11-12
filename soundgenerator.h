typedef struct soundgenerator_s {
  int tone;
  int frequency;
  int volume;
  int pending;
}SoundGenerator;



void SDLInitAudio(int32_t SamplesPerSecond, int32_t BufferSize);
SoundGenerator* SoundGeneratorCreate();
void soundtest(SoundGenerator* generator);
