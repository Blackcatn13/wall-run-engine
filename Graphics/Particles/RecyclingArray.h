#pragma once

#ifndef __RECYCLING_ARRAY_H__
#define __RECYCLING_ARRAY_H__

#define __DONT_INCLUDE_MEM_LEAKS__

#include "Utils\BaseUtils.h"


template<class T>

class CRecyclingArray
{
	public:
		CRecyclingArray(uint32 _uiMax): m_uiUsedElements(0), m_uiFreeElements(_uiMax), m_uiTotalElements(_uiMax), m_vRecyclingArray(0),
										m_vRecyclingArrayStatus(0) { Init(); }

		~CRecyclingArray() { Release(); }
		const uint32 GetNumFreeElements() const { return m_uiFreeElements; };
		const uint32 GetNumUsedElements() const { return m_uiUsedElements; };
		const uint32 GetTotalElements() const { return m_uiTotalElements; };
		T* GetAt(uint32 _uiIndex) const;
		bool IsFree(uint32 _uiIndex) const;
		T* New();
		void Free(uint32 _uiIndex);
		void DeleteAllElements();

	protected:
		void Release();

	private:
		void Init();
		uint32  m_uiFreeElements;
		uint32  m_uiUsedElements;
		uint32  m_uiTotalElements;
		T*      m_vRecyclingArray;
		T       m_TDefault;
		bool*   m_vRecyclingArrayStatus;

};


template<class T>

void CRecyclingArray<T>::Init(void)
{
	m_vRecyclingArray = new T[m_uiTotalElements];
	m_vRecyclingArrayStatus = new bool[m_uiTotalElements];

	for(uint32 i = 0; i < m_uiTotalElements; i++)
		m_vRecyclingArrayStatus[i] = true;
};

template<class T>

void CRecyclingArray<T>::Release(void)
{
	CHECKED_DELETE_ARRAY(m_vRecyclingArray);
	CHECKED_DELETE_ARRAY(m_vRecyclingArrayStatus);
};


template<class T>

void CRecyclingArray<T>::DeleteAllElements(void)
{
	for(uint32 i = 0; i < m_uiTotalElements; i++)
		m_vRecyclingArrayStatus[i] = true;

	m_uiFreeElements = m_uiTotalElements;
	m_uiUsedElements = 0;
};


template<class T>

void CRecyclingArray<T>::Free(uint32 _uiIndex)
{
	if(!m_vRecyclingArrayStatus[_uiIndex])
	{
		m_uiUsedElements--;
		m_uiFreeElements++;

		m_vRecyclingArrayStatus[_uiIndex] = true;
	}
};


template<class T>

T* CRecyclingArray<T>::New(void)
{
	bool   l_bFound  = false;
	uint32 l_uiIndex = 0;

	if(m_uiFreeElements > 0)
	{
		while(!l_bFound && l_uiIndex < m_uiTotalElements)
		{
			l_bFound = m_vRecyclingArrayStatus[l_uiIndex];
			l_uiIndex++;
		}
	}

	if(l_bFound)
	{
		l_uiIndex--;
		m_uiFreeElements--;	
		m_uiUsedElements++;

		m_vRecyclingArrayStatus[l_uiIndex] = false;
		m_vRecyclingArray[l_uiIndex] = m_TDefault;

		return &(m_vRecyclingArray[l_uiIndex]);
	}else
		return 0;
};


template<class T>

bool CRecyclingArray<T>::IsFree(uint32 _uiIndex) const
{
	return m_vRecyclingArrayStatus[_uiIndex];
};


template<class T>

T* CRecyclingArray<T>::GetAt(uint32 _uiIndex) const
{
	return &(m_vRecyclingArray[_uiIndex]);
};


#endif
