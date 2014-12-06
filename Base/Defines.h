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

#endif