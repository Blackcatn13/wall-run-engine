#pragma once
#ifndef NAMED_H
#define NAMED_H

#include <string>

class CNamed
{
protected:
    std::string m_Name;
public:
    CNamed() {} //Ojo que este es solo para probar el ScriptManager

    void setName(std::string name)
    {
        m_Name = name;
        if (name == "Tete") //Test scripting
            printf("Yaay\n");
    }
    std::string getName()
    {
        return m_Name;
    }
};

#endif