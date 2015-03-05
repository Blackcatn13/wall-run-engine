#pragma once
#ifndef LUA_GLOBALS_H
#define LUA_GLOBALS_H

#include <string>

class CLuaGlobals
{
protected:
    std::string m_string;
    int			m_int;
    float		m_float;
    static CLuaGlobals* m_instance;
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
    }

    void setInt (int i)
    {
        m_int = i;
    }

    void setFloat (float f)
    {
        m_float = f;
    }

    std::string getString ()
    {
        return m_string;
    }

    int getInt ()
    {
        return m_int;
    }

    float getFloat ()
    {
        return m_float;
    }
};

#endif