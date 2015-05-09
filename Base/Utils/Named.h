#pragma once
#ifndef NAMED_H
#define NAMED_H

#include <string>

class CNamed {
 protected:
  std::string m_Name;
 public:
  CNamed() {} //Ojo que este es solo para probar el ScriptManager

  void setName(std::string name) {
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
    if (name == "Enter3D") //Test scripting
      printf("Enter3D\n");
    if (name == "Enter2D") //Test scripting
      printf("Enter2D\n");
    if (name == "INITOK") //Test scripting
      printf("InitOK\n");
    if (name == "UpdatePass1") //Test scripting
      printf("UpdatePass1\n");
    if (name == "UpdatePass2") //Test scripting
      printf("UpdatePass2\n");
    if (name == "UpdatePassFinal") //Test scripting
      printf("UpdatePassFinal\n");
    if (name == "Enter5") //Test scripting
      printf("Enter5\n");
    if (name == "Enter6") //Test scripting
      printf("Enter6\n");
    if (name == "ObjSet") //Test scripting
      printf("ObjSet\n");
    if (name == "ObjectCamSet") //Test scripting
      printf("ObjectCamSet\n");
    if (name == "CameraSet") //Test scripting
      printf("CameraSet\n");
    if (name == "cambio") //Test scripting
      printf("cambio\n");
    if (name == "10") //Test scripting
      printf("10\n");
    if (name == "-10") //Test scripting
      printf("-10\n");
    if (name == "LuaBreak") //Test scripting
      printf("Break\n");
    if (name == "entra1") //Test scripting
      printf("entra1\n");
    if (name == "entra2") //Test scripting
      printf("entra2\n");
    if (name == "entra3") //Test scripting
      printf("entra3\n");
    if (name == "entra4") //Test scripting
      printf("entra4\n");
    if (name == "entra5") //Test scripting
      printf("entra5\n");
    if (name == "entra6") //Test scripting
      printf("entra6\n");
  }
  std::string getName() {
    return m_Name;
  }
};

#endif