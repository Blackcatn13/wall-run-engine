#ifndef _TYPES_H_
#define _TYPES_H_

typedef signed char         int8;   // integer de 8 bits
typedef short               int16;  // integer de 16 bits
typedef int                 int32;  // integer de 32 bits

typedef __int64             int64;  // integer de 64 bits
typedef unsigned __int64    uint64; // integer positivo de 64 bits

typedef unsigned char       uint8;  // integer positivo de 8 bits
typedef unsigned short      uint16; // integer positivo de 16 bits
typedef unsigned int        uint32; // integer positivo de 32 bits


typedef enum ETypeModePaint { PAINT_SOLID, PAINT_WIREFRAME, PAINT_BOTH};
typedef enum EtypeSphere		{ COMPLETE, HALF_TOP, HALF_BOTTOM };

typedef enum ETypeFunction { FUNC_CONSTANT, FUNC_INCREMENT, FUNC_DECREMENT};

#endif
