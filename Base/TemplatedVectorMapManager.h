#pragma once
#ifndef TEMPLATED_VECTOR_MAP_MANAGER_H
#define TEMPLATED_VECTOR_MAP_MANAGER_H

#include <map>
#include <string>
#include <vector>
#include "Utils\Logger.h"
#include "Utils\Defines.h"


template<class T>
class CTemplatedVectorMapManager
{
public:
	class CMapResourceValue
	{
	public:
		T *m_Value;
		size_t m_Id;
		CMapResourceValue(T *Value, size_t Id)
		: m_Value(Value)
		, m_Id(Id)
		{
		}
	};
	typedef std::vector<T *> TVectorResources;
	typedef std::map<std::string, CMapResourceValue> TMapResources;
protected:
	TVectorResources	m_ResourcesVector;
	TMapResources		m_ResourcesMap;
public:
	CTemplatedVectorMapManager()
	{
	}

	virtual ~CTemplatedVectorMapManager() 
	{
		Destroy();
	}

	void RemoveResource(const std::string &Name)
	{
		m_ResourcesMap::iterator it = m_ResourcesMap.find(Name);
		if(it != m_ResourcesMap.end()) {
			size_t index = m_ResourcesMap[Name].m_Id;
			CHECKED_DELETE(m_ResourcesVector[m_Id]);
			m_ResourcesVector.erase(m_ResourcesVector.begin() + m_Id);
			m_ResourcesMap.erase(it);
			for(m_ResourcesMap::iterator it = m_ResourcesMap.begin(); ++it)
			{
				if(it->second.m_Id >= m_Id)
					it->second.m_Id--;
			}
		} else {
			LOGGER->AddNewLog(ELL_ERROR, "Resource not found in TVectorMapManager with name '%s'", Name.c_str());
		}
	}

	virtual T * GetResourceById(size_t Id)
	{
		if(m_ResourcesVector.size() <= Id || Id < 0) {
			LOGGER->AddNewLog(ELL_ERROR, "Resource not found in TVectorMapManager with id '%d'", Id);
			return NULL;
		} else {
			return m_ResourcesVector[Id];
		}
	}

	virtual T * GetResource(const std::string &Name)
	{
		m_ResourcesMap::iterator it = m_ResourcesMap.find(Name);
		if(it != m_ResourcesMap.end()) {
			return it->second.m_Value;
		} else {
			LOGGER->AddNewLog(ELL_ERROR, "Resource not found in TVectorMapManager with name '%s'", Name.c_str());
			return NULL;
		}
	}

	virtual bool AddResource(const std::string &Name, T *Resource)
	{
		m_ResourcesMap::iterator it = m_ResourcesMap.find(Name);
		if(it != m_ResourcesMap.end()) {
			LOGGER->AddNewLog(ELL_ERROR, "Resource already found in TVectorMapManager with name '%s'", Name.c_str());
			return false;
		}
		m_ResourcesMap[Name] = CMapResourceValue(Resource, m_ResourcesVector.size());
		m_ResourcesVector.push_back(Resource);
	}

	virtual void Destroy()
	{
		for(m_ResourcesVector::it = m_ResourcesVector.begin(); it != m_ResourcesVector.end(); ++it)
		{
			CHECKED_DELETE(it);
		}
		m_ResourcesVector.clear();
		m_ResourcesMap.clear();
	}

	TMapResources & GetResourcesMap() 
	{
		return m_ResourcesMap;
	}

	TVectorResources & GetResourcesVector() 
	{
		return m_ResourcesVector;
	}
};

#endif