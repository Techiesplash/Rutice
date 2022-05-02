#pragma once
#include <Rutice/Generic>

struct logEntry {
    string str;
    uint8_t tag;  
};

/// Split up a string by a character into a vector. 
/// Additional integer argument allows automatic line return.
std::vector<string> splitString(string str, char seperator, int newlineDist = -1)
{
    std::vector<string> tempVector;
    string tempString = "";
    int newlineCounter = 0;
    for (int i = 0; i < (int)str.size(); i++)
    {
        if (str[i] == seperator && tempString != "")
        {
            tempVector.push_back(tempString);
            tempString = "";
            newlineCounter = 0;
        }
        else if (str[i] == seperator && tempString == "")
        {
            // Ignore
        } else if(str[i] != seperator && newlineCounter >= newlineDist && newlineDist >= 0) {
            tempString = tempString + str[i];
            tempVector.push_back(tempString);
            tempString = "";
            newlineCounter = 0;
        }
        else
        {
            tempString = tempString + str[i];
        }

        if(newlineDist >= 0) {
            newlineCounter++;
        }
    }
    // if( tempString != u"" ) {
    //       tempVector.push_back( tempString );
    //   }
    return tempVector;
}

namespace components
{
    namespace debugInternal
    {

        std::vector<string> errorLog;
        std::vector<string> warnLog;
        std::vector<string> debugLog;
        float error_logPosition = 0;
        int error_maxLogPosition = 0;
        float warn_logPosition = 0;
        int warn_maxLogPosition = 0;
        float debug_logPosition = 0;
        int debug_maxLogPosition = 0;

        enum ACTIVECONSOLETYPE
        {
            DEBUG,
            WARN,
            ERROR,
            OBJECT,
            VAR,
            OFF
        };

        ACTIVECONSOLETYPE debugConsole_activeconsole = OFF;
        GameObject *debugConsole_textObject;
        Scene *debugConsole_Scene;
        bool debugConsole_Visible;
        C2D_SpriteSheet debugConsole_SpriteSheet;
        C2D_Font consoleFont;
        int controlBarY = 224;
        fvect2 offsetMaster = {0.0, 16.0};
        fvect2 offset = {0.0, 16.0};
        int maxLineHistory = 1024;
        int maxLines = 23;
    }
}
