#pragma once
//#include <Rutice/Generic>
#include <Rutice/Internal/basics.hpp>

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
        //GameObject *debugConsole_textObject;
        //Scene *debugConsole_Scene;
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


namespace debugConsole
{
    int linereturnDist = 42;
    namespace error
    {
        void Log(string str1)
        {
            string str = str1 + "\n";
            printf("\033[1;31m%s\n\033[31m", str.c_str());
            std::vector<string> tempSVector = splitString(str + '\n', '\n', linereturnDist);
            for (auto &v : tempSVector)
            {

                components::debugInternal::errorLog.push_back(v);
                if ((int)components::debugInternal::errorLog.size() > components::debugInternal::maxLineHistory)
                {
                    components::debugInternal::errorLog.erase(components::debugInternal::errorLog.begin());
                }
                else
                {
                    components::debugInternal::error_maxLogPosition++;
                }
            }
            components::debugInternal::debugLog.push_back(" ");
        }
    }
    namespace warn
    {
        
        void Log(string str1)
        {
            string str = str1 + "\n";
            printf("\033[1;33m%s\n\033[31m", str.c_str());
            std::vector<string> tempSVector = splitString(str + '\n', '\n', linereturnDist);
            for (auto &v : tempSVector)
            {

                components::debugInternal::warnLog.push_back(v);
                if ((int)components::debugInternal::warnLog.size() > components::debugInternal::maxLineHistory)
                {
                    components::debugInternal::warnLog.erase(components::debugInternal::warnLog.begin());
                }
                else
                {
                    components::debugInternal::warn_maxLogPosition++;
                }
            }
            components::debugInternal::debugLog.push_back(" ");
        }
    }
    namespace debug
    {
        void Log(string str1)
        {
            string str = str1 + "\n";
            printf(str.c_str());
            std::vector<string> tempSVector = splitString(str + '\n', '\n', linereturnDist);
            for (auto &v : tempSVector)
            {
                components::debugInternal::debugLog.push_back(v);
                if ((int)components::debugInternal::debugLog.size() > components::debugInternal::maxLineHistory)
                {
                    components::debugInternal::debugLog.erase(components::debugInternal::debugLog.begin());
                }
                else
                {
                    components::debugInternal::debug_maxLogPosition++;
                }
            }
            components::debugInternal::debugLog.push_back(" ");
        }
    }
}
