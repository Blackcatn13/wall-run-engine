#pragma once
#ifndef NAMED_H
#define NAMED_H

#include <string>

class CNamed
{
private:
    std::string m_Name;
public:
    void setName(std::string name)
    {
        m_Name = name;
    }
    std::string getName()
    {
        return m_Name;
    }
};

#endif