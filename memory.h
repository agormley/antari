typedef struct _memory_map
{
  // atari only uses 8192!!
  union{
    unsigned char memory[65536];
    struct{ 
      unsigned char tia_write[45];
      unsigned char not_used0[3];
      unsigned char tia_read[14];
      unsigned char not_used1[66];
      unsigned char riot_ram[128];
      unsigned char stack[256];
      unsigned char not_used2[128];
      unsigned char riot_io [24];  // 664 bytes?
      unsigned char not_used3[3432];
      unsigned char rom[ROM_SIZE];
      unsigned char not_used4[57344];
    };
  };
}MemoryMap;
/* 0000-002C TIA (Write) */
/* 0030-003D TIA (Read) */
/* 0080-00FF RIOT RAM */
/* 0280-0297 RIOT I/O, TIMER */
/* F000-FFFF ROM */

unsigned short
MemoryGetTwoBytesAt(unsigned short addr);

void
MemoryCreate();

unsigned char
MemoryGetByteAt(unsigned short addr);

void
MemorySetByteAt(unsigned short addr, unsigned char byte);
