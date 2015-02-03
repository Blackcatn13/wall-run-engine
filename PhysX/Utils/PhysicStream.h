//----------------------------------------------------------------------------------
// CPhysicUserStream, CPhysicMemoryWriteBuffer and CPhysicMemoryReadBuffer class
// Author: Enric Vergara
//
// Description:
// Clase con una implementacion de los streams que utilizara physX
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_PHYSIC_STREAM_H
#define INC_PHYSIC_STREAM_H



#include "NxStream.h"

class CPhysicUserStream : public NxStream
{

public:

	CPhysicUserStream(const char* filename, bool load);
	virtual	~CPhysicUserStream();

	virtual		NxU8				readByte()								const;
	virtual		NxU16				readWord()								const;
	virtual		NxU32				readDword()								const;
	virtual		float				readFloat()								const;
	virtual		double			readDouble()							const;
	virtual		void				readBuffer(void* buffer, NxU32 size)	const;

	virtual		NxStream&		storeByte(NxU8 b);
	virtual		NxStream&		storeWord(NxU16 w);
	virtual		NxStream&		storeDword(NxU32 d);
	virtual		NxStream&		storeFloat(NxReal f);
	virtual		NxStream&		storeDouble(NxF64 f);
	virtual		NxStream&		storeBuffer(const void* buffer, NxU32 size);

	FILE*			fp;
};



class CPhysicMemoryWriteBuffer : public NxStream
{

public:
	CPhysicMemoryWriteBuffer();
	virtual	~CPhysicMemoryWriteBuffer();

	void									clear();
	virtual		NxU8				readByte()								const	{ NX_ASSERT(0);	return 0;	}
	virtual		NxU16				readWord()								const	{ NX_ASSERT(0);	return 0;	}
	virtual		NxU32				readDword()								const	{ NX_ASSERT(0);	return 0;	}
	virtual		float				readFloat()								const	{ NX_ASSERT(0);	return 0.0f;}
	virtual		double			readDouble()							const	{ NX_ASSERT(0);	return 0.0;	}
	virtual		void				readBuffer(void* buffer, NxU32 size)	const	{ NX_ASSERT(0);	}

	virtual		NxStream&		storeByte(NxU8 b);
	virtual		NxStream&		storeWord(NxU16 w);
	virtual		NxStream&		storeDword(NxU32 d);
	virtual		NxStream&		storeFloat(NxReal f);
	virtual		NxStream&		storeDouble(NxF64 f);
	virtual		NxStream&		storeBuffer(const void* buffer, NxU32 size);

	NxU32			currentSize;
	NxU32			maxSize;
	NxU8*			data;
};

class CPhysicMemoryReadBuffer : public NxStream
{

public:
	CPhysicMemoryReadBuffer(const NxU8* data);
	virtual	~CPhysicMemoryReadBuffer();

	virtual		NxU8				readByte()								const;
	virtual		NxU16				readWord()								const;
	virtual		NxU32				readDword()								const;
	virtual		float				readFloat()								const;
	virtual		double			readDouble()							const;
	virtual		void				readBuffer(void* buffer, NxU32 size)	const;

	virtual		NxStream&		storeByte(NxU8 b)								{ NX_ASSERT(0);	return *this;	}
	virtual		NxStream&		storeWord(NxU16 w)							{ NX_ASSERT(0);	return *this;	}
	virtual		NxStream&		storeDword(NxU32 d)							{ NX_ASSERT(0);	return *this;	}
	virtual		NxStream&		storeFloat(NxReal f)						{ NX_ASSERT(0);	return *this;	}
	virtual		NxStream&		storeDouble(NxF64 f)						{ NX_ASSERT(0);	return *this;	}
	virtual		NxStream&		storeBuffer(const void* buffer, NxU32 size)	{ NX_ASSERT(0);	return *this;	}

	mutable		const NxU8*		buffer;
};

#endif //INC_PHYSIC_STREAM_H
