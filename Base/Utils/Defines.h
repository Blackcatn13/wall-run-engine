#ifndef _DEFINES_H
#define _DEFINES_H

#define GET_SET(TYPE, PARAM) \
	void set##PARAM (TYPE PARAM) { \
		m_##PARAM = PARAM; \
	} \
	TYPE get##PARAM () { \
		return m_##PARAM; \
	}

#define GET_SET_REF(TYPE, PARAM) \
	void set##PARAM (const TYPE &PARAM) { \
		m_##PARAM = PARAM; \
	} \
	const TYPE & get##PARAM () { \
		return m_##PARAM; \
	}

#define CHECKED_DELETE(x) if(x != 0) {delete x; x=0;}
#define CHECKED_RELEASE(x)  if(x) {x->Release(); x=0;}
#define CHECKED_DELETE_ARRAY(x) if(x) {delete[] x; x=0;}

#define NULL 0

#endif