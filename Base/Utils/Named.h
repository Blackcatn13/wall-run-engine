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
        if (name == "TriggerEnter") //Test scripting
            printf("Triggers Enter\n");
        if (name == "TriggerStay") //Test scripting
            printf("Triggers Stay\n");
        if (name == "TriggerExit") //Test scripting
            printf("Triggers Exit\n");
        if (name == "Testing-Chanchullo_Lua") //Test scripting
            printf("Testing-Chanchullo_Lua\n");
		if(name=="TestCamara")
			printf("Camara3DOK");
		if(name=="InitCamara")
			printf("InitCamaraOK");
		if(name=="InitCamaraBefore")
			printf("InitCamaraOKBefore");
		if(name=="Empieza")
			printf("EmpiezaOK");
    }
    std::string getName()
    {
        return m_Name;
    }
};

#endif