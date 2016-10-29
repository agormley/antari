BYTE
getImmediate(ushort addr);

BYTE
getZero(ushort addr, ushort* mid);

BYTE
getZeroX(ushort pc, ushort* mid, ushort* addr);

BYTE
getZeroY(ushort pc, ushort* mid, ushort* addr);

BYTE
getAbsolute(ushort addr, ushort* mid);

BYTE
getAbsoluteX(ushort addr, ushort* mid, ushort *write_addr);

BYTE
getAbsoluteY(ushort addr, ushort* mid, ushort *write_addr);

BYTE
getIndirectX(ushort addr, ushort* mid, ushort *write_addr);

BYTE
getIndirectY(ushort addr, ushort* mid, ushort *write_addr);

BYTE
getIndirect(ushort addr, ushort* mid);
