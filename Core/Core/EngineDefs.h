#ifndef ENGINE_DEFS_H
#define ENGINE_DEFS_H

#include <string>
#include <vector>

typedef struct Conf_info {
    int                         Screen_Width;
    int                         Screen_Heigth;
    int                         Win_posX;
    int                         Win_posY;
    bool                        FullScreen;
    bool                        Mouse_Exclusive;
    bool                        Mouse_Draw;
    std::string                 ActionsPath;
    std::string                 FontsPath;
    std::vector<std::string>    LanguagesPath;
    std::string                 CurrentLanguage;
} CONFIG_INFO;


#endif