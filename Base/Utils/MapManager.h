#pragma once
#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H

#include <map>
#include <string>
#include "Utils\Logger.h"
#include "Utils\Defines.h"


template<class T>
class CMapManager
{
public:
    typedef std::map<std::string, T*> TMapResource;
protected:
    TMapResource m_Resources;
public:
    virtual T * GetResource(const std::string &Name)
    {
        TMapResource::iterator it = m_Resources.find(Name);
        if (it != m_Resources.end()) {
            return it->second;
        } else {
            LOGGER->AddNewLog(ELL_ERROR, "Resource not found in TMapManager with name '%s'", Name.c_str());
            return NULL;
        }
    }

    bool ExisteResource(const std::string &Name)
    {
        return (m_Resources.find(Name) != m_Resources.end());
    }

    virtual bool AddResource(const std::string &Name, T *Resource)
    {
        if (m_Resources.find(Name) != m_Resources.end()) {
            LOGGER->AddNewLog(ELL_ERROR, "Resource already found in TMapManager with name '%s'", Name.c_str());
			return false;
        } else {
            m_Resources[Name] = Resource;
			return true;
        }
    }

    void Destroy()
    {
        for (TMapResource::iterator it = m_Resources.begin(); it != m_Resources.end(); it++) {
            CHECKED_DELETE(it->second);
        }
        m_Resources.clear();
    }

	TMapResource & GetResources()
	{
		return m_Resources;
	}
};

#endif