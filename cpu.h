#define OPCODE_ADC_IMM		0x69
#define OPCODE_ADC_ZERO		0x65
#define OPCODE_ADC_ZERO_X	0x75
#define OPCODE_ADC_ABS		0x6D
#define OPCODE_ADC_ABS_X	0x7D
#define OPCODE_ADC_ABS_Y	0x79
#define OPCODE_ADC_IND_X	0x61
#define OPCODE_ADC_IND_Y	0x71

#define OPCODE_AND_IMM		0x29
#define OPCODE_AND_ZERO		0x25
#define OPCODE_AND_ZERO_X   	0x35
#define OPCODE_AND_ABS		0x2D
#define OPCODE_AND_ABS_X    	0x3D
#define OPCODE_AND_ABS_Y    	0x39
#define OPCODE_AND_IND_X	0x21
#define OPCODE_AND_IND_Y	0x31

#define OPCODE_ASL_A		0x0A
#define OPCODE_ASL_ZERO		0x44
#define OPCODE_ASL_ZERO_X	0x16
#define OPCODE_ASL_ABS		0x0E
#define OPCODE_ASL_ABS_X	0x1E


#define OPCODE_BIT_ZERO		0x24
#define OPCODE_BIT_ABS		0x2C

#define OPCODE_BPL		0x10
#define OPCODE_BMI		0x30
#define OPCODE_BVC		0x50
#define OPCODE_BVS		0x70
#define OPCODE_BCC		0x90
#define OPCODE_BCS		0xB0
#define OPCODE_BNE		0xD0
#define OPCODE_BEQ		0xF0

#define OPCODE_BRK		0x00

#define OPCODE_CMP_IMM		0xC9
#define OPCODE_CMP_ZERO		0xC5
#define OPCODE_CMP_ZERO_X	0xD5
#define OPCODE_CMP_ABS		0xCD
#define OPCODE_CMP_ABS_X	0xDD
#define OPCODE_CMP_ABS_Y	0xD9
#define OPCODE_CMP_IND_X	0xC1
#define OPCODE_CMP_IND_Y	0xD1

#define OPCODE_CPX_IMM		0xE0
#define OPCODE_CPX_ZERO		0xE4
#define OPCODE_CPX_ABS		0xEC

#define OPCODE_CPY_IMM		0xC0
#define OPCODE_CPY_ZERO		0xC4
#define OPCODE_CPY_ABS		0xCC

#define OPCODE_DEC_ZERO		0xC6
#define OPCODE_DEC_ZERO_X	0xD6
#define OPCODE_DEC_ABS		0xCE
#define OPCODE_DEC_ABS_X	0xDE


#define OPCODE_EOR_IMM		0x49
#define OPCODE_EOR_ZERO		0x45
#define OPCODE_EOR_ZERO_X	0x55
#define OPCODE_EOR_ABS		0x4D
#define OPCODE_EOR_ABS_X	0x5D
#define OPCODE_EOR_ABS_Y	0x59
#define OPCODE_EOR_IND_X	0x41
#define OPCODE_EOR_IND_Y	0x51

#define OPCODE_CLC		0x18
#define OPCODE_SEC		0x38
#define OPCODE_CLI		0x58
#define OPCODE_SEI		0x78
#define OPCODE_CLV		0xB8
#define OPCODE_CLD		0xD8
#define OPCODE_SED		0xF8

#define OPCODE_INC_ZERO		0xE6
#define OPCODE_INC_ZERO_X	0xF6
#define OPCODE_INC_ABS		0xEE  
#define OPCODE_INC_ABS_X	0xFE

#define OPCODE_JMP_ABS		0x4C
#define OPCODE_JMP_IND		0x6C


#define OPCODE_JSR_ABS		0x20

#define OPCODE_LDA_IMM		0xA9
#define OPCODE_LDA_ZERO		0xA5
#define OPCODE_LDA_ZERO_X		0xB5  
#define OPCODE_LDA_ABS		0xAD  
#define OPCODE_LDA_ABS_X		0xBD  
#define OPCODE_LDA_ABS_Y		0xB9  
#define OPCODE_LDA_IND_X		0xA1  
#define OPCODE_LDA_IND_Y		0xB1  

#define OPCODE_LDX_IMM		0xA2  
#define OPCODE_LDX_ZERO		0xA6  
#define OPCODE_LDX_ZERO_Y		0xB6  
#define OPCODE_LDX_ABS		0xAE  
#define OPCODE_LDX_ABS_Y		0xBE  

#define OPCODE_LDY_IMM		0xA0
#define OPCODE_LDY_ZERO		0xA4
#define OPCODE_LDY_ZERO_X		0xB4
#define OPCODE_LDY_ABS		0xAC
#define OPCODE_LDY_ABS_X		0xBC

#define OPCODE_LSR_A		0x4A
#define OPCODE_LSR_ZERO		0x46
#define OPCODE_LSR_ZERO_X		0x56
#define OPCODE_LSR_ABS		0x4E
#define OPCODE_LSR_ABS_X		0x5E

#define OPCODE_NOP		0xEA

#define OPCODE_ORA_IMM		0x09
#define OPCODE_ORA_ZERO		0x05
#define OPCODE_ORA_ZERO_X		0x15
#define OPCODE_ORA_ABS		0x0D
#define OPCODE_ORA_ABS_X		0x1D
#define OPCODE_ORA_ABS_Y		0x19
#define OPCODE_ORA_IND_X		0x01
#define OPCODE_ORA_IND_Y		0x11

#define OPCODE_TAX	0xAA
#define OPCODE_TXA	0x8A
#define OPCODE_DEX	0xCA
#define OPCODE_INX	0xE8
#define OPCODE_TAY	0xA8
#define OPCODE_TYA	0x98
#define OPCODE_DEY	0x88
#define OPCODE_INY	0xC8
 
#define OPCODE_ROL_A		0x2A
#define OPCODE_ROL_ZERO		0x26
#define OPCODE_ROL_ZERO_X		0x36
#define OPCODE_ROL_ABS		0x2E
#define OPCODE_ROL_ABS_X		0x3E


#define OPCODE_ROR_A		0x6A
#define OPCODE_ROR_ZERO		0x66
#define OPCODE_ROR_ZERO_X		0x76
#define OPCODE_ROR_ABS		0x6E
#define OPCODE_ROR_ABS_X		0x7E

#define OPCODE_RTI		0x40

#define OPCODE_RTS		0x60

#define OPCODE_SBC_IMM		0xE9
#define OPCODE_SBC_ZERO		0xE5
#define OPCODE_SBC_ZERO_X		0xF5
#define OPCODE_SBC_ABS		0xED
#define OPCODE_SBC_ABS_X		0xFD
#define OPCODE_SBC_ABS_Y		0xF9
#define OPCODE_SBC_IND_X		0xE1
#define OPCODE_SBC_IND_Y		0xF1

#define OPCODE_STA_ZERO		0x85
#define OPCODE_STA_ZERO_X		0x95
#define OPCODE_STA_ABS		0x8D
#define OPCODE_STA_ABS_X		0x9D
#define OPCODE_STA_ABS_Y		0x99
#define OPCODE_STA_IND_X		0x81
#define OPCODE_STA_IND_Y		0x91
             

// Stack Instructions
#define OPCODE_TXS		0x9A
#define OPCODE_TSX		0xBA
#define OPCODE_PHA		0x48
#define OPCODE_PLA		0x68
#define OPCODE_PHP		0x08
#define OPCODE_PLP		0x28

#define OPCODE_STX_ZERO		0x86
#define OPCODE_STX_ZERO_Y		0x96
#define OPCODE_STX_ABS		0x8E  

#define OPCODE_STY_ZERO		0x84
#define OPCODE_STY_ZERO_X		0x94
#define OPCODE_STY_ABS		0x8C

#define SIGN_MASK (1<<7)

#define FLAG_CARRY_MASK (1 << 0)
#define FLAG_CARRY(a) (FLAG_CARRY_MASK & a)
#define FLAG_CARRY_CLEAR(a) (a &= ~FLAG_CARRY_MASK)
#define FLAG_CARRY_SET(a) (a |= FLAG_CARRY_MASK)
#define SETCARRY(a) ((a>255)?					\
		     FLAG_CARRY_SET(processor->regs.flags):	\
		     FLAG_CARRY_CLEAR(processor->regs.flags))

#define FLAG_ZERO_MASK (1 << 1)
#define FLAG_ZERO(a) (FLAG_ZERO_MASK & a)
#define FLAG_ZERO_CLEAR(a) (a &= ~FLAG_ZERO_MASK)
#define FLAG_ZERO_SET(a) (a |= FLAG_ZERO_MASK)
#define SETZERO(a) (processor->regs.flags = (processor->regs.flags & \
		    (~FLAG_ZERO_MASK)) | ((!a) << 1))

#define FLAG_INTERRUPT_MASK (1 << 2)
#define FLAG_INTERRUPT(a) (FLAG_INTERRUPT_MASK & a)
#define FLAG_INTERRUPT_CLEAR(a) (a &= ~FLAG_INTERRUPT_MASK)
#define FLAG_INTERRUPT_SET(a) (a |= FLAG_INTERRUPT_MASK)

#define FLAG_DECIMAL_MASK (1 << 3)
#define FLAG_DECIMAL(a) (FLAG_DECIMAL_MASK & a)
#define FLAG_DECIMAL_CLEAR(a) (a &= ~FLAG_DECIMAL_MASK)
#define FLAG_DECIMAL_SET(a) (a |= FLAG_DECIMAL_MASK)

#define FLAG_BREAK_MASK (1 << 4)
#define FLAG_BREAK(a) (FLAG_BREAK_MASK & a)
#define FLAG_BREAK_CLEAR(a) (a &= ~FLAG_BREAK_MASK)
#define FLAG_BREAK_SET(a) (a |= FLAG_BREAK_MASK)

#define FLAG_OVER_MASK (1 << 6)
#define FLAG_OVER(a) (FLAG_OVER_MASK & a)
#define FLAG_OVER_CLEAR(a) (a &= ~FLAG_OVER_MASK)
#define FLAG_OVER_SET(a) (a |= FLAG_OVER_MASK)
#define SETOVER(a,b,c) (processor->regs.flags = (processor->regs.flags & \
						 (~FLAG_OVER_MASK)) |	\
			((~(a ^ b))&(a ^ c)&SIGN_MASK)>>1)

#define FLAG_NEG_MASK (1 << 7)
#define FLAG_NEG(a) (FLAG_NEG_MASK & a)
#define FLAG_NEG_CLEAR(a) (a &= ~FLAG_NEG_MASK)
#define FLAG_NEG_SET(a) (a |= FLAG_NEG_MASK)

#define SETSIGN(a) (processor->regs.flags = (processor->regs.flags & \
		    (~FLAG_NEG_MASK)) | ((a) & SIGN_MASK))

#define NMI_VECTOR_ADDR 0xFFFA
#define RESET_VECTOR_ADDR 0xFFFC
#define BREAK_VECTOR_ADDR 0xFFFE

#define MEMORY_MASK 0x1FFF
#define STACK_MASK 0x100


#define REG_A processor->regs.accumulator
#define REG_X processor->regs.x
#define REG_Y processor->regs.y
#define REG_SP processor->regs.sp
#define REG_PC processor->regs.pc
#define REG_ST processor->regs.flags


typedef struct _Cpu{
  struct{
    unsigned char accumulator;
    unsigned char x;
    unsigned char y;
    unsigned char sp;
    ushort pc;
    unsigned char flags;
  }regs;
  ushort resetvect;
  ushort nmi;
  ushort irq;
  ushort brk;
  int cycles;
}Cpu;

int
CpuCycle();

int
CpuCreate();

