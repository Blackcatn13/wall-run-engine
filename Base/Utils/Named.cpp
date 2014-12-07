#include "Named.h"

void CNamed::setName(std::string name)
{
    m_Name = name;
}
std::string CNamed::getName()
{
    return m_Name;
}