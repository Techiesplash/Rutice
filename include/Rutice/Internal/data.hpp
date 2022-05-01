#pragma once
#include <Rutice/Generic>
#include <string.h>
#include <string>
#include <stdio.h>
#include <unistd.h>

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
    namespace dotscene
    {
        bool l1 = false;
        FILE *file;
        void openFile(string path)
        {
            file = fopen(path.c_str(), "r");
        }

        void closeFile()
        {
            fclose(file);
        }

        Scene *loadSceneData(string sceneName)
        {
            debug::Log("Loading scene '" + sceneName + "' data from file.\n");
            bool inScope = false;
            Scene *tempScene = new Scene();
            if (!file)
            {
                warn::Log("Attempted to load scene data without a file.\n");
                return tempScene;
            }

            string sheet = "romfs:/gfx/notex.t3x";
            string id = "Empty";
            string name = "GameObject";
            fvect3 pos = {0, 0, 0};
            bool screen = false;
            int16_t index = 0;
            char buf[100];
            while (fgets(buf, sizeof(buf), file)) // do until EOF
            {
                string str2(buf);
                debug::Log(str2 + "\n");

                std::string str1(buf);
                std::vector<string> split = splitString(str1 + " ", ' ', 9999);
                int splitSize = split.size();
                //  debug::Log("splitSize=" + std::to_string(splitSize) + "\n");
                // Scope
                if (splitSize == 2)
                {
                    if (split[0] == "@scene")
                    {
                        //  debug::Log(split[1]);
                        if (split[1] == sceneName + "\n")
                        {

                            debug::Log("Entered scene location\n");
                            inScope = true;
                        }
                        else
                        {
                            inScope = false;
                        }
                    }
                    else if (split[0] == "@sprites")
                    {
                        //   debug::Log(split[1]);

                        sheet = split[1];
                    }
                }

                if (inScope)
                {
                    // debug::Log("In scope.\n");
                    //  In the scope, check for objects to create.
                    if (split[0] == "@pushobj\n")
                    {
                        debug::Log("Pushing object");
                        if (screen == false)
                        {
                            GameObject &gameObject0 = tempScene->Create(sheet, Render_top, {pos.x, pos.y, pos.z}, name, index);
                            loadGameObjectAttr(id, &gameObject0);
                        }
                        else
                        {
                            GameObject &gameObject0 = tempScene->Create(sheet, Render_bottom, {pos.x, pos.y, pos.z}, name, index);
                            loadGameObjectAttr(id, &gameObject0);
                        }
                        id = "Empty";
                        name = "GameObject";
                        pos = {0, 0, 0};
                        screen = false;
                        index = 0;
                    }
                    // Use the $var signs for setting variables on created objects.
                    if (splitSize >= 3)
                    {
                        if (split[0] == "$var")
                        {
                            debug::Log("'$var' identified.");
                            if (split[1] == "id")
                            {

                                id = split[2];
                                debug::Log(split[2]);
                            }
                            if (split[1] == "name\n")
                            {

                                name = split[2];
                            }
                            if (split[1] == "screen")
                            {
                                if (split[2] == "false\n")
                                {
                                    screen = false;
                                }
                                else if (split[2] == "true\n")
                                {
                                    screen = true;
                                }
                            }
                            if (split[1] == "index")
                            {
                                index = std::stoi(split[2]);
                            }
                        }
                    }
                }
            }
            return tempScene;
        }

    }
}

Scene *loadSceneUnstable(string id)
{

    // debug::Log("(UNSTABLE) Loading scene '" + id + "'.\n");
    data::dotscene::openFile("romfs:/data/scene/sceneExample.scene");
    Scene *temp = data::dotscene::loadSceneData(id);
    data::dotscene::closeFile();
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
