#pragma once
#include <Rutice/Generic>
#include <string.h>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <Rutice/Internal/components.hpp>
using namespace debugConsole;

typedef struct
{
    string name;
    float varfloat;
    string varstring;
} InterpObjectData;

/// Load a GameObject's attributes from a function list, with a string id.
/// GameObject data can be defined within this list.
void loadGameObjectAttr(string id, GameObject *gameObject)
{
    debug::Log("\nLoading GameObject data with id:\n " + id);
    try
    {
        if (id == "cursorObject")
        {
            gameObject->addComponent<components::CursorComponent>();
            gameObject->animation.frameTimer = 5;
            gameObject->animation.ids = {0, 1, 2, 3, 4, 3, 2, 1, 0, 0};
        }
        // Debug console
        else if (id == "consoleDebug_textHandlerObject")
        {

            components::debugInternal::debugConsole_textObject = gameObject;
            components::debugInternal::consoleFont = C2D_FontLoadSystem(CFG_REGION_USA);
            gameObject->addComponent<components::debugInternal::debugConsole_TextHandlerComponent>();
            gameObject->addComponent<components::debugInternal::debugConsole_ToggleDebugComponent>();
        }
        else if (id == "consoleDebug_debugLogButton")
        {
            gameObject->addComponent<components::debugInternal::debugConsole_DebugButton>();
        }
        else if (id == "consoleDebug_warnLogButton")
        {
            gameObject->addComponent<components::debugInternal::debugConsole_WarnButton>();
        }
        else if (id == "consoleDebug_errorLogButton")
        {
            gameObject->addComponent<components::debugInternal::debugConsole_ErrorButton>();
        }
        else
        {
            warn::Log("Attempted to load GameObject with invalid id:\n " + id);
            return;
        }
    }
    catch (std::exception &ex)
    {
        error::Log("Exception while attempting to load GameObject data:\n " + std::string(ex.what()));
    }
}

namespace data
{
    InterpObjectData getObjectData(string id, std::vector<InterpObjectData> &dataSet)
    {
        InterpObjectData outdata;
        outdata.varfloat = 0;
        outdata.varstring = "";
        outdata.name = "";

        for (int i = 0; i < (int)dataSet.size(); i++)
        {
            if (dataSet[i].name == id)
            {
                return dataSet[i];
            }
        }
        return outdata;
    }

    void setObjectData(InterpObjectData dataIn, std::vector<InterpObjectData> &dataSet)
    {
        for (int i = 0; i < (int)dataSet.size(); i++)
        {
            if (dataSet[i].name == dataIn.name)
            {
                dataSet[i].varstring = dataIn.varstring;
                dataSet[i].varfloat = dataIn.varfloat;
            }
        }
        dataSet.push_back(dataIn);
    }
    class omlInterpreter
    {

        bool l1 = false;

        // Filesystem things
        FILE *file;

    public:
        void openFile(string path)
        {
            file = fopen(path.c_str(), "r");
        }

        void closeFile()
        {
            fclose(file);
        }

        // Actually interpreting the data
        std::vector<std::vector<InterpObjectData>> getSectionData(string section)
        {
            // Interp flags
            bool varNoClear = false;
            bool allowDefaults = true;

            // Output
            std::vector<std::vector<InterpObjectData>> tempParent;
            std::vector<InterpObjectData> tempChild;
            std::vector<InterpObjectData> defaults;

            //
            bool inScope = false;

            if (!file)
            {
                warn::Log("Attempted to interpret a non-existent file.");
                return tempParent;
            }

            char buf[256];
            while (fgets(buf, sizeof(buf), file)) // do until EOF
            {
                string str2(buf);
               // debug::Log(str2 + "\n");

                std::string str1(buf);
                std::vector<string> split = splitString(str1 + " ", ' ', 9999);
                if(split.size() >= 1)
                {
                    split[split.size() - 1].erase(std::remove(split[split.size() - 1].begin(), split[split.size() - 1].end(), '\n'), split[split.size() - 1].end());
                   
                }
                int splitSize = split.size();
                //  debug::Log("splitSize=" + std::to_string(splitSize) + "\n");
                // Scope

                if (split[0] == "@section")
                {

                    if (split[1] == section)
                    {
               //         debug::Log("Entered scope.");
                        inScope = true;
                    }
                    else if (inScope == true)
                    {

                   //     debug::Log("Exited scope.");
                        inScope = false;
                        return tempParent;
                    }
                    if (!varNoClear)
                    {
                        tempChild.clear();
                    }

                    if (allowDefaults)
                    {
                        for (int i = 0; i < (int)defaults.size(); i++)
                        {
                            data::setObjectData(defaults[i], tempChild);
                        }
                    }
                }

                if (inScope)
                {

                    // Finish the data for this object and move on to the next
                    if (split[0] == "@submit")
                    {
                        tempParent.push_back(tempChild);

                        // Reset variables
                    }

                    // Read general data
                    if (splitSize >= 3)
                    {
                        if (split[0] == "@flag")
                        {

                            // Get boolean value
                            bool valid = true;
                            bool value = false;

                            if (split[2] == "false")
                            {
                                value = false;
                            }
                            else if (split[2] == "true")
                            {
                                value = true;
                            }
                            else
                            {
                                valid = false;
                            }

                            if (valid)
                            {
                                if (split[1] == "noclear")
                                {
                                    varNoClear = value;
                                }
                                else if (split[1] == "allowdefaults")
                                {
                                    allowDefaults = value;
                                }
                            }
                        }

                        // String Variable data
                        if (split[0] == "$svar")
                        {
                            InterpObjectData temp1;
                            temp1.name = split[1];
                            temp1.varstring = split[2];
                            temp1.varfloat = 0;
                            setObjectData(temp1, tempChild);
                        }

                        // Floating-point Variable data
                        if (split[0] == "$fvar")
                        {
                            InterpObjectData temp1;
                            temp1.name = split[1];
                            temp1.varstring = "";
                            temp1.varfloat = std::stof(split[2]);
                            setObjectData(temp1, tempChild);
                        }

                        // String Defaults data
                        if (split[0] == "$sdef")
                        {
                            InterpObjectData temp1;
                            temp1.name = split[1];
                            temp1.varstring = split[2];
                            temp1.varfloat = 0;
                            setObjectData(temp1, defaults);
                        }

                        // Floating-point Defaults data
                        if (split[0] == "$fdef")
                        {
                            InterpObjectData temp1;
                            temp1.name = split[1];
                            temp1.varstring = "";
                            temp1.varfloat = std::stof(split[2]);
                            setObjectData(temp1, defaults);
                        }
                    }
                }
            }
            return tempParent;
        }
    };
}

Scene *loadSceneUnstable(string id)
{

    Scene *temp = new Scene();

    // debug::Log("(UNSTABLE) Loading scene '" + id + "'.\n");
    data::omlInterpreter interpreter;
    interpreter.openFile("romfs:/data/scene/sceneExample.oml");

    std::vector<std::vector<InterpObjectData>> data = interpreter.getSectionData(id);

    for (int i = 0; i < (int)data.size(); i++)
    {

        // GameObject &gameObject = temp->Create("romfs:/gfx/cursor_sprites.t3x", Render_top, {0, 0, 0}, "Cursor", 0);
        // loadGameObjectAttr("cursorObject", &gameObject);
        debug::Log(data::getObjectData("screen", data[i]).varstring);
        if (data::getObjectData("screen", data[i]).varstring == "false")
        {

            GameObject &gameObject = temp->Create(data::getObjectData("sheet", data[i]).varstring, Render_top, {data::getObjectData("posx", data[i]).varfloat, data::getObjectData("posy", data[i]).varfloat, data::getObjectData("posz", data[i]).varfloat}, data::getObjectData("name", data[i]).varstring, (int)data::getObjectData("index", data[i]).varfloat);
            loadGameObjectAttr(data::getObjectData("id", data[i]).varstring, &gameObject);
        }
        else
        {
            GameObject &gameObject = temp->Create(data::getObjectData("sheet", data[i]).varstring, Render_bottom, {data::getObjectData("posx", data[i]).varfloat, data::getObjectData("posy", data[i]).varfloat, data::getObjectData("posz", data[i]).varfloat}, data::getObjectData("name", data[i]).varstring, (int)data::getObjectData("index", data[i]).varfloat);
            loadGameObjectAttr(data::getObjectData("id", data[i]).varstring, &gameObject);
        }
    }

    interpreter.closeFile();

    // bool foundScene = false;

    /*
    for(int i = 0; i < sceneCount; i++)
    {
        if(sceneDefs[i].name == id )
        {
            debug::Log("(UNSTABLE) Found Scene ID, \n ");
            foundScene = true;
            for(int h = 0; h < sceneDefs[i].objNum; h++)
            {
                debug::Log("sceneDefs[i].objects[h].texturePath=" + sceneDefs[i].objects[h].texturePath + "\n");
                debug::Log("sceneDefs[i].objects[h].name=" + sceneDefs[i].objects[h].name + "\n");
                debug::Log("sceneDefs[i].objects[h].index=" + std::to_string(sceneDefs[i].objects[h].index) + "\n");

                debug::Log("(UNSTABLE) Attempting to load object[" + std::to_string(i) + "]");
                if(sceneDefs[i].objects[h].renderTarget == false)
                {
                    debug::Log("Object assigned to top screen\n");
                    GameObject &gameObject = temp->Create(sceneDefs[i].objects[h].texturePath, Render_top, sceneDefs[i].objects[h].position, sceneDefs[i].objects[h].name, sceneDefs[i].objects[h].index);
                    loadGameObjectAttr(sceneDefs[i].objects[h].id, &gameObject);
                } else {
                    debug::Log("Object assigned to bottom screen\n");
                    GameObject &gameObject = temp->Create(sceneDefs[i].objects[h].texturePath, Render_bottom, sceneDefs[i].objects[h].position, sceneDefs[i].objects[h].name, sceneDefs[i].objects[h].index);
                    loadGameObjectAttr(sceneDefs[i].objects[h].id, &gameObject);
                }
            }
        }
    }


    if(!foundScene)
    {
        warn::Log("\n(UNSTABLE) Could not find Scene with ID\n \"" + id + "\".");
    }*/
    return temp;
}

Scene *loadScene(string id, C3D_RenderTarget *target)
{
    debug::Log("Setting up Scene id:\n " + id);
    Scene *temp = new Scene();
    if (id == "testing")
    {
        GameObject &gameObject = temp->Create("romfs:/gfx/cursor_sprites.t3x", Render_top, {0, 0, 0}, "Cursor", 0);
        loadGameObjectAttr("cursorObject", &gameObject);
    }
    else if (id == "debugConsole")
    {
        // Text handler / hide button
        GameObject &gameObject0 = temp->Create("romfs:/gfx/eng_debug_menu.t3x", Render_bottom, {0, 224, 0}, "consoleDebug_textHandler", 0);
        loadGameObjectAttr("consoleDebug_textHandlerObject", &gameObject0);

        // Log buttons
        GameObject &gameObject1 = temp->Create("romfs:/gfx/eng_debug_menu.t3x", Render_bottom, {16, 224, 0}, "consoleDebug_debugButton", 1);
        loadGameObjectAttr("consoleDebug_debugLogButton", &gameObject1);
        GameObject &gameObject2 = temp->Create("romfs:/gfx/eng_debug_menu.t3x", Render_bottom, {32, 224, 0}, "consoleDebug_warningButton", 2);
        loadGameObjectAttr("consoleDebug_warnLogButton", &gameObject2);
        GameObject &gameObject3 = temp->Create("romfs:/gfx/eng_debug_menu.t3x", Render_bottom, {48, 224, 0}, "consoleDebug_errorButton", 3);
        loadGameObjectAttr("consoleDebug_errorLogButton", &gameObject3);
        // Cursor
        GameObject &gameObject = temp->Create("romfs:/gfx/cursor_sprites.t3x", Render_bottom, {0, 0, 0}, "Debug Cursor", 0);
        loadGameObjectAttr("cursorObject", &gameObject);
    }
    else
    {
        warn::Log("Attempted to set up Scene with invalid id:\n " + id);
    }

    return temp;
}
