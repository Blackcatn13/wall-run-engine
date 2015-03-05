#pragma once
#ifndef LUA_GLOBALS_H
#define LUA_GLOBALS_H

#include <string>

class CLuaGlobals
{
protected:
    std::string m_string;
    static CLuaGlobals* m_instance;
    bool        m_changed;
public:
    CLuaGlobals();
    //Ojo que este es solo para probar el ScriptManager

    static CLuaGlobals* getInstance()
    {
        if (m_instance == 0) {
            m_instance = new CLuaGlobals();
        }
        return m_instance;
    }
    void setString (std::string s)
    {
        m_string = s;
        m_changed = true;
    }

    std::string getString ()
    {
        m_changed = false;
        return m_string;
    }

    bool ValueChanged()
    {
        return m_changed;
    }
};

#endif