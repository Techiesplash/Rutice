#pragma once

#include <Rutice/Generic>


#include <Rutice/Internal/font.hpp>
using namespace debugConsole;

struct gameobjectDefinition {
    std::string id;
};

struct gameobjectInstanceDefinition {
    std::string id;
    std::string texturePath;
    std::string name;
    fvect3 position;
    int16_t index;
    bool renderTarget;
};

struct sceneDefinition {
    std::string name;
    uint16_t objNum;
    struct gameobjectInstanceDefinition objects[512];
};

int sceneCount = 1;
sceneDefinition sceneDefs[1] = 
{   // Scene Definitons
    {   // Scenes
        "testing",
        1,
        {   // Objects
            {   // Cursor Object
                "cursorObject",
                "romfs:/gfx/cursor_sprites.t3x",
                "Cursor",
                { 0, 0, 0},
                0,
                false
            }
        }
    }
};






