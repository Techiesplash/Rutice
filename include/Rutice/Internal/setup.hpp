#pragma once

#include <Rutice/Generic>
#include <Rutice/Internal/font.hpp>
using namespace debugConsole;

/// Load a GameObject's attributes from a function list, with a string id.
/// GameObject data can be defined within this list.
void loadGameObjectAttr(string id, GameObject *gameObject)
{
    debug::Log("\nLoading GameObject data with id:\n " + id);
    try {
    if (id == "cursorObject")
    {
        gameObject->addComponent<components::CursorComponent>();
        gameObject->animation.frameTimer = 5;
        gameObject->animation.ids = {0, 1, 2, 3, 4, 3, 2, 1, 0, 0};
    } 
    // Debug console
    else if (id == "consoleDebug_textHandlerObject") {
        
        components::debugInternal::debugConsole_textObject = gameObject;
        components::debugInternal::consoleFont = C2D_FontLoadSystem(CFG_REGION_USA);
        gameObject->addComponent<components::debugInternal::debugConsole_TextHandlerComponent>();
        gameObject->addComponent<components::debugInternal::debugConsole_ToggleDebugComponent>();
    } else if (id == "consoleDebug_debugLogButton") {
        gameObject->addComponent<components::debugInternal::debugConsole_DebugButton>();
    } else if (id == "consoleDebug_warnLogButton") {
        gameObject->addComponent<components::debugInternal::debugConsole_WarnButton>();
    } else if (id == "consoleDebug_errorLogButton") {
        gameObject->addComponent<components::debugInternal::debugConsole_ErrorButton>();
    }
    else
    {
        warn::Log("Attempted to load GameObject with invalid id:\n " + id);
        return;
    }
    } catch (std::exception& ex) {
        error::Log("Exception while attempting to load GameObject data:\n " + std::string(ex.what()));
    }

}

Scene *loadScene(string id, C3D_RenderTarget *target)
{
    debug::Log("\nSetting up Scene id:\n " + id);
    Scene *temp = new Scene();
    if (id == "testing")
    {
        GameObject &gameObject = temp->Create("romfs:/gfx/cursor_sprites.t3x", Render_bottom, {0, 0, 0}, "Cursor", 0);
        loadGameObjectAttr("cursorObject", &gameObject);
    }
    else if (id == "debugConsole")
    {
        // Text handler / hide button
        GameObject &gameObject0 = temp->Create("romfs:/gfx/eng_debug_menu.t3x", Render_bottom, {0, 224, 0}, "consoleDebug_textHandler", 0);
        loadGameObjectAttr("consoleDebug_textHandlerObject", &gameObject0);
        components::debugInternal::debugConsole_Scene = temp;
        // Log buttons
        GameObject &gameObject1 = temp->Create("romfs:/gfx/eng_debug_menu.t3x", Render_bottom, {16, 224, 0}, "consoleDebug_debugButton", 1);
        loadGameObjectAttr("consoleDebug_debugLogButton", &gameObject1);
        GameObject &gameObject2 = temp->Create("romfs:/gfx/eng_debug_menu.t3x", Render_bottom, {32, 224, 0}, "consoleDebug_warningButton", 2);
        loadGameObjectAttr("consoleDebug_warnLogButton", &gameObject2);
        GameObject &gameObject3 = temp->Create("romfs:/gfx/eng_debug_menu.t3x", Render_bottom, {48, 224, 0}, "consoleDebug_errorButton", 3);
        loadGameObjectAttr("consoleDebug_errorLogButton", &gameObject3);
        

        // Other
        
    } else {
        warn::Log("Attempted to set up Scene with invalid id:\n " + id);
        
    }

    return temp;
}