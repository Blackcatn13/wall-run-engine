#define __DONT_INCLUDE_MEM_LEAKS__
#include <stdio.h>
#include "NxPhysics.h"
#include "Utils\PhysicStream.h"

#include "Utils\BaseUtils.h"

#include "Core_Utils\MemLeaks.h"

CPhysicUserStream::CPhysicUserStream(const char* filename, bool load) : fp(NULL)
{
	fopen_s(&fp, filename, load ? "rb" : "wb");
}

CPhysicUserStream::~CPhysicUserStream()
{
	if(fp)	fclose(fp);
}

// Loading API
NxU8 CPhysicUserStream::readByte( void ) const
{
	NxU8 b;
	size_t r = fread(&b, sizeof(NxU8), 1, fp);
	NX_ASSERT(r);
	return b;
}

NxU16 CPhysicUserStream::readWord( void ) const
{
	NxU16 w;
	size_t r = fread(&w, sizeof(NxU16), 1, fp);
	NX_ASSERT(r);
	return w;
}

NxU32 CPhysicUserStream::readDword( void ) const
{
	NxU32 d;
	size_t r = fread(&d, sizeof(NxU32), 1, fp);
	NX_ASSERT(r);
	return d;
}

float CPhysicUserStream::readFloat( void ) const
{
	NxReal f;
	size_t r = fread(&f, sizeof(NxReal), 1, fp);
	NX_ASSERT(r);
	return f;
}

double CPhysicUserStream::readDouble( void ) const
{
	NxF64 f;
	size_t r = fread(&f, sizeof(NxF64), 1, fp);
	NX_ASSERT(r);
	return f;
}

void CPhysicUserStream::readBuffer(void* buffer, NxU32 size)	const
{
	size_t w = fread(buffer, size, 1, fp);
	NX_ASSERT(w);
}

// Saving API
NxStream& CPhysicUserStream::storeByte(NxU8 b)
{
	size_t w = fwrite(&b, sizeof(NxU8), 1, fp);
	NX_ASSERT(w);
	return *this;
}

NxStream& CPhysicUserStream::storeWord(NxU16 w)
{
	size_t ww = fwrite(&w, sizeof(NxU16), 1, fp);
	NX_ASSERT(ww);
	return *this;
}

NxStream& CPhysicUserStream::storeDword(NxU32 d)
{
	size_t w = fwrite(&d, sizeof(NxU32), 1, fp);
	NX_ASSERT(w);
	return *this;
}

NxStream& CPhysicUserStream::storeFloat(NxReal f)
{
	size_t w = fwrite(&f, sizeof(NxReal), 1, fp);
	NX_ASSERT(w);
	return *this;
}

NxStream& CPhysicUserStream::storeDouble(NxF64 f)
{
	size_t w = fwrite(&f, sizeof(NxF64), 1, fp);
	NX_ASSERT(w);
	return *this;
}

NxStream& CPhysicUserStream::storeBuffer(const void* buffer, NxU32 size)
{
	size_t w = fwrite(buffer, size, 1, fp);
	NX_ASSERT(w);
	return *this;
}




CPhysicMemoryWriteBuffer::CPhysicMemoryWriteBuffer() : currentSize(0), maxSize(0), data(NULL)
{
}

CPhysicMemoryWriteBuffer::~CPhysicMemoryWriteBuffer()
{
	NX_DELETE_ARRAY(data);
}

void CPhysicMemoryWriteBuffer::clear()
{
	currentSize = 0;
}

NxStream& CPhysicMemoryWriteBuffer::storeByte(NxU8 b)
{
	storeBuffer(&b, sizeof(NxU8));
	return *this;
}
NxStream& CPhysicMemoryWriteBuffer::storeWord(NxU16 w)
{
	storeBuffer(&w, sizeof(NxU16));
	return *this;
}
NxStream& CPhysicMemoryWriteBuffer::storeDword(NxU32 d)
{
	storeBuffer(&d, sizeof(NxU32));
	return *this;
}
NxStream& CPhysicMemoryWriteBuffer::storeFloat(NxReal f)
{
	storeBuffer(&f, sizeof(NxReal));
	return *this;
}
NxStream& CPhysicMemoryWriteBuffer::storeDouble(NxF64 f)
{
	storeBuffer(&f, sizeof(NxF64));
	return *this;
}
NxStream& CPhysicMemoryWriteBuffer::storeBuffer(const void* buffer, NxU32 size)
{
	NxU32 expectedSize = currentSize + size;
	if(expectedSize > maxSize)
	{
		maxSize = expectedSize + 4096;

		NxU8* newData = new NxU8[maxSize];
		NX_ASSERT(newData!=NULL);

		if(data)
		{
			memcpy(newData, data, currentSize);
			delete[] data;
		}
		data = newData;
	}
	memcpy(data+currentSize, buffer, size);
	currentSize += size;
	return *this;
}


CPhysicMemoryReadBuffer::CPhysicMemoryReadBuffer(const NxU8* data) : buffer(data)
{
}

CPhysicMemoryReadBuffer::~CPhysicMemoryReadBuffer()
{
	// We don't own the data => no delete
}

NxU8 CPhysicMemoryReadBuffer::readByte( void ) const
{
	NxU8 b;
	memcpy(&b, buffer, sizeof(NxU8));
	buffer += sizeof(NxU8);
	return b;
}

NxU16 CPhysicMemoryReadBuffer::readWord( void ) const
{
	NxU16 w;
	memcpy(&w, buffer, sizeof(NxU16));
	buffer += sizeof(NxU16);
	return w;
}

NxU32 CPhysicMemoryReadBuffer::readDword( void ) const
{
	NxU32 d;
	memcpy(&d, buffer, sizeof(NxU32));
	buffer += sizeof(NxU32);
	return d;
}

float CPhysicMemoryReadBuffer::readFloat( void ) const
{
	float f;
	memcpy(&f, buffer, sizeof(float));
	buffer += sizeof(float);
	return f;
}

double CPhysicMemoryReadBuffer::readDouble( void ) const
{
	double f;
	memcpy(&f, buffer, sizeof(double));
	buffer += sizeof(double);
	return f;
}

void CPhysicMemoryReadBuffer::readBuffer(void* dest, NxU32 size) const
{
	memcpy(dest, buffer, size);
	buffer += size;
}
