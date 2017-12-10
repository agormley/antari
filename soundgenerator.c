#include "includes.h"
#include <math.h>

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

#define PIXELCLOCK 31200
#define CPUCLOCK 10400
#define Swap2Bytes(val) \
( (((val) >> 8) & 0x00FF) | (((val) << 8) & 0xFF00) )

int SamplesPerSecond = 44100;
int BytesPerSample = sizeof(int16) * 2;
static int samplecounter = 0;


double cotan(double i) { return(1 / tan(i)); }

int16 getSawSample(float period, int amplitude) {
  return (int16)((-2 * amplitude) / M_PI) * atan(cotan((samplecounter++ * M_PI) / (period * 8)));
}

int16 getSineSample(float period, int amplitude) {
  return (int16)(amplitude * sin(samplecounter++ * (M_PI * 2.0) / period));

}

int16 getSquareSample(float period, int amplitude){
  int16 sampleVal = ((samplecounter++ / ((int)period/2)) % 2)?
    amplitude : -amplitude;
  return sampleVal;
}

static int bits4[] = {1,1,1,1};
static int bits5[] = {1,1,1,1,1};
static int bits9[] = {1,1,1,1,1,1,1,1,1};

int16 getFourBitWave(float period, int amplitude) {
  int16 result = 0;
  static int count = 0;
  result = bits4[3]?amplitude:-amplitude;

  int tmpcount = (samplecounter++ / ((int)period*2));
  if (tmpcount != count) {
    count = tmpcount;
    int z = bits4[2]^bits4[3];
    bits4[3]=bits4[2];
    bits4[2]=bits4[1];
    bits4[1]=bits4[0];
    bits4[0]=z;
  }
  return result;

}

int16 getFour31BitWave(float period, int amplitude) {
  int16 result = 0;
  static int count = 0;
  static int thirtyone = 0;

  result = bits4[3]?amplitude:-amplitude;

  int offset[] = {18, 13};

  int tmpcount = (samplecounter++ / ((int)period*2*offset[thirtyone%2]));
  if (tmpcount != count) {
    count = tmpcount;
    thirtyone++;
    int z = bits4[2]^bits4[3];
    bits4[3]=bits4[2];
    bits4[2]=bits4[1];
    bits4[1]=bits4[0];
    bits4[0]=z;
  }
  return result;

}

int16 get31BitWave(float period, int amplitude) {
  int16 result = 0;
  static int count = 0;
  static int thirtyone;

  result = thirtyone%2?amplitude:-amplitude;

  int offset[] = {18, 13};
  int tmpcount = (samplecounter++ / ((int)period*2*offset[thirtyone%2]));
  if (tmpcount != count ) {
    
    thirtyone++;
    count = (samplecounter++ / ((int)period*2*offset[thirtyone%2]));
  }
  return result;

}

int16 getFiveBitWave(float period, int amplitude) {
  int16 result = 0;
  static int count = 0;

  result = bits5[4]?amplitude:-amplitude;
  int tmpcount = (samplecounter++ / ((int)period*2));
  if (tmpcount != count) {
    count = tmpcount;
    int z = bits5[2]^bits5[4];
    bits5[4]=bits5[3];
    bits5[3]=bits5[2];
    bits5[2]=bits5[1];
    bits5[1]=bits5[0];
    bits5[0]=z;
  }
  return result;

}

int16 getFiveFourBitWave(float period, int amplitude) {
  int16 result = 0;
  int shift4 = 0;
  static int count = 0;

  result = bits4[3]?amplitude:-amplitude;
  shift4 = bits5[4];

  int tmpcount = (samplecounter++ / ((int)period*2));
  if (tmpcount != count) {
    count = tmpcount;
    int z = bits5[2]^bits5[4];
    bits5[4]=bits5[3];
    bits5[3]=bits5[2];
    bits5[2]=bits5[1];
    bits5[1]=bits5[0];
    bits5[0]=z;
    if (shift4) {
      z = bits4[2]^bits4[3];
      bits4[3]=bits4[2];
      bits4[2]=bits4[1];
      bits4[1]=bits4[0];
      bits4[0]=z;
    }
  }
  return result;

}

int16 getNineBitWave(float period, int amplitude) {
  int16 result = 0;
  static int count = 0;

  result = bits9[8]?amplitude:-amplitude;
  int tmpcount = (samplecounter++ / ((int)period));
  if (tmpcount != count) {
    count = tmpcount;
    int z = bits9[4]^bits9[8];
    bits9[8]=bits9[7];
    bits9[7]=bits9[6];
    bits9[6]=bits9[5];
    bits9[5]=bits9[4];
    bits9[4]=bits9[3];
    bits9[3]=bits9[2];
    bits9[2]=bits9[1];
    bits9[1]=bits9[0];
    bits9[0]=z;
  }

  return result;
}

int16 getDist1(float period, int amplitude) {
  int length = 15;
  int bits[] = {0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1};
  int16 result = 0;

  static float oldperiod = 0;
  if (oldperiod != period) {
    oldperiod = period;
  }

  result = bits[(samplecounter++ / ((int)period*31))%length]?amplitude:-amplitude;

  return result;
}

int16 getWaveSample(int tone, float period, int amplitude, int channel) {
  static int oldtone1 = 0;
  if (oldtone1 != (tone&0xF) && channel == 1) {
    printf("tone1: %#x\n", tone&0xF);
    oldtone1 = (tone&0xF);
 
  }

  static int oldtone2 = 0;
  if (oldtone2 != (tone&0xF) && channel == 2) {
    printf("tone2: %#x\n", tone&0xF);
    oldtone2 = (tone&0xF);
 
  }

  switch(tone&0xF){
  case 0:
  case 0xB:
    return 0xffff;
  case 1:
    return Swap2Bytes(getFourBitWave(period, amplitude));
  case 2:
    return Swap2Bytes(getFour31BitWave(period, amplitude));
  case 3:
    return Swap2Bytes(getFiveFourBitWave(period, amplitude));
  case 4:
    return Swap2Bytes(getSquareSample(period*8, amplitude));
  case 5:
    return Swap2Bytes(getSquareSample(period*8, amplitude));
  case 6:
  case 0xA:
    return Swap2Bytes(get31BitWave(period, amplitude));  
  case 8:
    return Swap2Bytes(getNineBitWave(period*8, amplitude));
  case 7:
  case 9:
    return Swap2Bytes(getFiveBitWave(period, amplitude));
  case 0xC:
  case 0xD:
    return Swap2Bytes(getSquareSample(period*24, amplitude));
  case 0xE:
    return Swap2Bytes(get31BitWave(period*3, amplitude));
  case 0XF:
    return Swap2Bytes(getFiveBitWave(period*6, amplitude));
  default:
    printf("unknown tone: %d\n", tone&0xF);
    return Swap2Bytes(getSawSample(period, amplitude));
  }
}

int16 getSample1() {
  float toneHz = 30000;
  if (audio1->frequency == 0)
    audio1->frequency = 1;
  toneHz /= (audio1->frequency);
  int16 amplitude = 5 * audio1->volume;
  float period = SamplesPerSecond / toneHz;

  if(period < 2)
    period = 2;

  return getWaveSample(audio1->tone, period, amplitude,1);

}

int16 getSample2() {
  float toneHz = 30000;
  if (audio2->frequency == 0)
    audio2->frequency = 1;
  toneHz /= (audio2->frequency);
  int16 amplitude = 5 * audio2->volume;
  float period = SamplesPerSecond / toneHz;

  if(period < 2)
    period = 2;
  return getWaveSample(audio2->tone, period, amplitude,2);
}

void SDLAudioCallback(void *UserData, Uint8 *AudioData, int Length)
{
  if (!audio1 || !audio2)
    return;

  if (Length) {
    void *SoundBuffer = malloc(Length);
    int16 *SampleOut = (int16 *)SoundBuffer;

    for(int SampleIndex = 0; SampleIndex < (Length/2)/sizeof(int16); ++SampleIndex) {
      int16 sampleVal1 = getSample1();
      int16 sampleVal2 = getSample2();

      *SampleOut++ = (sampleVal1) + sampleVal2;
      *SampleOut++ = (sampleVal1) + sampleVal2;
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

  printf("Initialised an Audio device at frequency %d Hz, %d Channels, %d Buffer Size.\n",
	 AudioSettings.freq, AudioSettings.channels, AudioSettings.size);

  if (AudioSettings.format != AUDIO_S16LSB)
    {
      printf("Oops! We didn't get AUDIO_S16LSB as our sample format!\n");
      SDL_CloseAudio();
    }

  SDL_PauseAudio(0);
}

SoundGenerator* SoundGeneratorCreate()
{
  SoundGenerator* sg =  (SoundGenerator*)calloc(1, sizeof(SoundGenerator));
  return sg;
}


