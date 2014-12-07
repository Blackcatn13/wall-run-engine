#pragma once
#ifndef VISIBLE_H
#define VISIBLE_H

class CVisible
{
private:
    bool m_visible;
public:
    void setVisible(bool visible)
    {
        m_visible = visible;
    }
    bool getVisible()
    {
        return m_visible;
    }
};

#endif