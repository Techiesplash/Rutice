#pragma once
#include <Rutice/Generic>

/*
#include <Rutice/Internal/basics.hpp>
#include <Rutice/Internal/sprites.hpp>
#include <Rutice/Internal/object.hpp>
*/
namespace components
{
    namespace debugInternal
    {

        /// Show-Hide button
        class debugConsole_ToggleDebugComponent : public Component
        {
            bool touchedFrame = false;
            int touchedFrameCount = 0;
            bool cVisibility = false;
            C2D_Sprite currentSprite;
            int yOffset = 0;

        public:
            void init() override
            {
                this->object->visible = false;
                this->object->touchBoundary[0].x = 0;
                this->object->touchBoundary[0].y = 0;
                this->object->touchBoundary[1].x = 16;
                this->object->touchBoundary[1].y = 16;
                this->object->touchable = true;

                debugConsole_SpriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/eng_debug_menu.t3x");
            }
            void update() override
            {
                printf("\ndebug_logPos=%f ", debug_logPosition);
                if (touchedFrame == true)
                {
                    touchedFrameCount++;
                    if (touchedFrameCount >= 3)
                    {
                        touchedFrame = false;
                        touchedFrameCount = 0;
                    }
                }
                if (cVisibility)
                {
                    C2D_SpriteFromSheet(&currentSprite, debugConsole_SpriteSheet, 6);
                }
                else
                {
                    C2D_SpriteFromSheet(&currentSprite, debugConsole_SpriteSheet, 7);
                }

                groupAnimLogic();
            }
            int animState = 0;
            bool animDirection = false;

            void groupAnimLogic()
            {
                // Do not run if it is at rest
                if (animState != 3)
                {
                    if (animDirection == false)
                    {
                        // Downwards
                        debugConsole_Visible = false;
                        if (controlBarY >= 224)
                        {
                            // Destination
                            controlBarY = 224;
                            offset.y = offsetMaster.y;
                            animState = 3;
                        }
                        else
                        {
                            controlBarY += 9;
                            offset.y += 9;
                        }
                    }
                    else if (animDirection == true)
                    {
                        // Downwards

                        if (controlBarY <= -16)
                        {
                            // Destination
                            debugConsole_Visible = true;
                            controlBarY = -16;
                            animState = 3;
                        }
                        else
                        {
                            debugConsole_Visible = false;
                            offset.y -= 9;
                            controlBarY -= 9;
                        }
                    }
                }
            }

            void onTouch() override
            {
                touchedFrameCount = 0;
                if (touchedFrame != true)
                {
                    if (!debugConsole_Visible)
                    {
                        animDirection = true;
                        animState = 0;
                        cVisibility = true;
                    }
                    else
                    {
                        animDirection = false;
                        animState = 0;
                        cVisibility = false;
                    }
                    touchedFrame = true;
                }
            }

            void draw() override
            {
                if (controlBarY > 0)
                {
                    this->object->position.y = controlBarY;
                    C2D_SpriteMove(&currentSprite, 0, controlBarY);
                }
                else
                {
                    this->object->position.y = 0;
                    C2D_SpriteMove(&currentSprite, 0, 0);
                }
                C2D_DrawSprite(&currentSprite);

                if (!debugConsole_Visible)
                {
                    for (int i = 0; i < 16; i++)
                    {
                        C2D_Sprite temp;
                        C2D_SpriteFromSheet(&temp, debugConsole_SpriteSheet, 0);
                        C2D_SpriteMove(&temp, (i * 16) + 64, controlBarY);
                        C2D_DrawSprite(&temp);
                    }
                }
            }
        };

        class debugConsole_DebugButton : public Component
        {
            // Setting this to ERROR should allow it to reload textures immediately.
            ACTIVECONSOLETYPE prev = ERROR;
        public:
            void init() override
            {
                this->object->touchBoundary[0].x = 0;
                this->object->touchBoundary[0].y = 0;
                this->object->touchBoundary[1].x = 16;
                this->object->touchBoundary[1].y = 16;
                this->object->touchable = true;
                this->object->textureAsset.enableAnim = false;
            }

            void update() override
            {
                if (debugConsole_Visible)
                {
                    this->object->visible = false;
                    this->object->touchable = false;
                }
                else
                {
                    this->object->visible = true;
                    this->object->touchable = true;
                    this->object->position.y = controlBarY;
                }

                if(prev != debugConsole_activeconsole){
                    if(debugConsole_activeconsole == DEBUG) {
                        this->object->textureIndex = 11;
                    } else {
                        this->object->textureIndex = 1;
                    }
                    this->object->reloadTextures();
                }

                prev = debugConsole_activeconsole;
            }

            void onTouch() override
            {
                debugConsole_activeconsole = DEBUG;
            }
        };

        class debugConsole_WarnButton : public Component
        {
        public:
         
            ACTIVECONSOLETYPE prev;
            void init() override
            {
                this->object->touchBoundary[0].x = 0;
                this->object->touchBoundary[0].y = 0;
                this->object->touchBoundary[1].x = 16;
                this->object->touchBoundary[1].y = 16;
                this->object->touchable = true;
                this->object->textureAsset.enableAnim = false;
            }

            void update() override
            {

                if (debugConsole_Visible)
                {
                    this->object->visible = false;
                    this->object->touchable = false;
                }
                else
                {
                    this->object->visible = true;
                    this->object->touchable = true;
                    this->object->position.y = controlBarY;
                }

                if(prev != debugConsole_activeconsole){
                    if(debugConsole_activeconsole == WARN) {
                        this->object->textureIndex = 12;
                    } else {
                        this->object->textureIndex = 2;
                    }
                    this->object->reloadTextures();
                }

                prev = debugConsole_activeconsole;
            }

            void onTouch() override
            {
                debugConsole_activeconsole = WARN;
            }
        };

        class debugConsole_ErrorButton : public Component
        {
        public:
            ACTIVECONSOLETYPE prev;
            void init() override
            {
                this->object->touchBoundary[0].x = 0;
                this->object->touchBoundary[0].y = 0;
                this->object->touchBoundary[1].x = 16;
                this->object->touchBoundary[1].y = 16;
                this->object->touchable = true;
                this->object->textureAsset.enableAnim = false;
            }

            void update() override
            {
                if (debugConsole_Visible)
                {
                    this->object->visible = false;
                    this->object->touchable = false;
                }
                else
                {
                    this->object->visible = true;
                    this->object->touchable = true;
                    this->object->position.y = controlBarY;
                }

                if(prev != debugConsole_activeconsole){
                    if(debugConsole_activeconsole == ERROR) {
                        this->object->textureIndex = 13;
                    } else {
                        this->object->textureIndex = 3;
                    }
                    this->object->reloadTextures();
                }

                prev = debugConsole_activeconsole;
            }

            void onTouch() override
            {
                debugConsole_activeconsole = ERROR;
            }
        };

        /// Handles text on the screen.
        class debugConsole_TextHandlerComponent : public Component
        {
        public:
            // Returns line returns
            int countLineReturns(string str, int returnLength, int mask)
            {

                int count = 0;
                try
                {
                    int hy = 0;
                    int hx = 0;
                    bool gx = true;

                    for (int i = 0; i < (int)str.size() / 2; i++)
                    {
                        if (str[i] == '\n' && gx == true)
                        {
                            hy++;
                            hx = 0;
                            count++;
                        }
                        else if (hx >= returnLength)
                        {
                            hy++;
                            hx = 0;
                            if (str[i + 1] == '\n')
                            {
                                gx = false;
                            }

                            count++;
                        }
                        gx = true;

                        hx++;
                    }
                }
                catch (std::exception &ex)
                {
                    printf("drawText() exception : \n %s", ex.what());
                }

                return count;
            }

            void init() override
            {
                this->timer[0] = 5;

                components::debugInternal::debugConsole_activeconsole = DEBUG;
                this->object->position.x = 0;
                this->object->touchBoundary[0].x = 0;
                this->object->touchBoundary[0].y = 0;
                this->object->touchBoundary[1].x = 16;
                this->object->touchBoundary[1].y = 16;
                
            }

            void timers(int timerNumber) override
            {
                try
                {
                    /*   printf("\ndebug_logPosition=%f ", debug_logPosition);
                       this->timer[0] = 5;
                       debug_logPosition += 0.1f;
                       if (debug_maxLogPosition > 24)
                       {
                           if (debug_logPosition > debug_maxLogPosition - (maxLines - 1))
                           {
                               debug_logPosition = 0;
                           }
                           else if (debug_logPosition >= debug_maxLogPosition - (maxLines - 0))
                           {
                               debug_logPosition = floor(debug_logPosition);
                           }
                       }
                       else
                       {

                           if (debug_logPosition > debug_maxLogPosition)
                           {
                               debug_logPosition = 0;
                           }
                       }*/
                }
                catch (std::exception &ex)
                {
                    printf("[textHandlerComponent.timers( %i ) exception] : \n%s\n", timerNumber, ex.what());
                }
            }

            int prevTouchY;
            bool touchDead = true;
            void update() override
            {
                try {
                this->object->position.x = 0;
                if (debugConsole_activeconsole == DEBUG)
                {
                    if ((int)debugLog.size() > 24)
                    {
                        if (touch.py > 15 && touch.py < 224 && !debugConsole_Visible && !touchDead)
                        {
                            if (prevTouchY != touch.py)
                            {

                                debug_logPosition += (prevTouchY - touch.py) / 2;
                                if (debug_logPosition < 0)
                                {
                                    debug_logPosition = 0;
                                }
                                else if (debug_logPosition > debug_maxLogPosition - (maxLines - 1))
                                {
                                    debug_logPosition = debug_maxLogPosition - maxLines;
                                }
                                else if (debug_logPosition >= debug_maxLogPosition - (maxLines - 0))
                                {
                                    debug_logPosition = debug_maxLogPosition - maxLines;
                                }
                            }
                        }
                        else if (touch.py > 15 && touch.py < 224 && !debugConsole_Visible && touchDead)
                        {

                            touchDead = false;
                        }
                        else
                        {
                            touchDead = true;
                        }

                        prevTouchY = touch.py;
                    }
                }
                else if (debugConsole_activeconsole == WARN)
                {
                    if ((int)warnLog.size() > 24)
                    {
                        if (touch.py > 15 && touch.py < 224 && !debugConsole_Visible && !touchDead)
                        {
                            if (prevTouchY != touch.py)
                            {

                                warn_logPosition += (prevTouchY - touch.py) / 2;
                                if (warn_logPosition < 0)
                                {
                                    warn_logPosition = 0;
                                }
                                else if (warn_logPosition > debug_maxLogPosition - (maxLines - 1))
                                {
                                    warn_logPosition = warn_maxLogPosition - maxLines;
                                }
                                else if (warn_logPosition >= warn_maxLogPosition - (maxLines - 0))
                                {
                                    warn_logPosition = warn_maxLogPosition - maxLines;
                                }
                            }
                        }
                        else if (touch.py > 15 && touch.py < 224 && !debugConsole_Visible && touchDead)
                        {

                            touchDead = false;
                        }
                        else
                        {
                            touchDead = true;
                        }

                        prevTouchY = touch.py;
                    }
                }
                else if (debugConsole_activeconsole == ERROR)
                {
                    if ((int)warnLog.size() > 24)
                    {
                        if (touch.py > 15 && touch.py < 224 && !debugConsole_Visible && !touchDead)
                        {
                            if (prevTouchY != touch.py)
                            {

                                error_logPosition += (prevTouchY - touch.py) / 2;
                                if (error_logPosition < 0)
                                {
                                    error_logPosition = 0;
                                }
                                else if (error_logPosition > error_maxLogPosition - (maxLines - 1))
                                {
                                    error_logPosition = error_maxLogPosition - maxLines;
                                }
                                else if (error_logPosition >= error_maxLogPosition - (maxLines - 0))
                                {
                                    error_logPosition = error_maxLogPosition - maxLines;
                                }
                            }
                        }
                        else if (touch.py > 15 && touch.py < 224 && !debugConsole_Visible && touchDead)
                        {

                            touchDead = false;
                        }
                        else
                        {
                            touchDead = true;
                        }

                        prevTouchY = touch.py;
                    }
                }
                } 
                catch (...)
                {
                    
                }
            }

            void draw() override
            {

                try
                {
                    if (!debugConsole_Visible)
                    {
                        if (debugConsole_activeconsole == DEBUG)
                        {
                            if (debug_maxLogPosition > 24)
                            {
                                if (debug_logPosition >= debug_maxLogPosition - (maxLines - 0))
                                {
                                    debug_logPosition = floor(debug_logPosition);
                                }
                            }
                            else if (debug_logPosition > debug_maxLogPosition || debug_logPosition < 0)
                            {
                                debug_logPosition = debug_maxLogPosition;
                            }
                            C2D_Sprite temp;
                            C2D_SpriteFromSheet(&temp, debugConsole_SpriteSheet, 10);
                            C2D_SpriteMove(&temp, 0, controlBarY - 240);
                            C2D_DrawSprite(&temp);

                            for (int i = 0; i < (int)debugLog.size() - floor(debug_logPosition); i++)
                            {
                                if (i > maxLines)
                                {
                                    break;
                                }

                                Text::drawText(offset.x, offset.y + (i * 9 + (std::fmod(debug_logPosition, 1.0f) * -8)), 0.0f, debugLog[i + floor(debug_logPosition)].c_str(), consoleFont, 0.5f, 0.5f , C2D_Color32f(1.0f, 1.0f, 1.0f, 1.0f));
                            }
                            for (int i = 0; i < 20; i++)
                            {
                                C2D_SpriteFromSheet(&temp, debugConsole_SpriteSheet, 0);
                                C2D_SpriteMove(&temp, i * 16, controlBarY - 224);
                                C2D_DrawSprite(&temp);
                            }
                  //          Text::drawText(0, controlBarY - 224, 1.0f, "Debug Log", consoleFont,  0.5f, 0.5f, C2D_Color32f(1.0f, 1.0f, 1.0f, 1.0f));
                        }
                        else if (debugConsole_activeconsole == ERROR)
                        {
                            if (error_maxLogPosition > 24)
                            {
                                if (error_logPosition >= error_maxLogPosition - (maxLines - 0))
                                {
                                    error_logPosition = floor(error_logPosition);
                                }
                            }
                            else if (error_logPosition > error_maxLogPosition || error_logPosition < 0)
                            {
                                error_logPosition = error_maxLogPosition;
                            }
                            C2D_Sprite temp;
                            C2D_SpriteFromSheet(&temp, debugConsole_SpriteSheet, 10);
                            C2D_SpriteMove(&temp, 0, controlBarY - 240);
                            C2D_DrawSprite(&temp);

                            for (int i = 0; i < (int)errorLog.size() - floor(error_logPosition); i++)
                            {
                                if (i > maxLines)
                                {
                                    break;
                                }

                                Text::drawText(offset.x, offset.y + (i * 9 + (std::fmod(error_logPosition, 1.0f) * -8)), 0.0f, errorLog[i + floor(error_logPosition)].c_str(), consoleFont, 0.5f, 0.5f, C2D_Color32f(1.0f, 1.0f, 1.0f, 1.0f));
                            }
                            for (int i = 0; i < 20; i++)
                            {
                                C2D_SpriteFromSheet(&temp, debugConsole_SpriteSheet, 0);
                                C2D_SpriteMove(&temp, i * 16, controlBarY - 224);
                                C2D_DrawSprite(&temp);
                            }
                        //    Text::drawText(0, controlBarY - 224, 1.0f, "Error Log", consoleFont,  0.5f, 0.5f, C2D_Color32f(1.0f, 1.0f, 1.0f, 1.0f));
                        }
                        else if (debugConsole_activeconsole == WARN)
                        {
                            if (warn_maxLogPosition > 24)
                            {
                                if (warn_logPosition >= warn_maxLogPosition - (maxLines - 0))
                                {
                                    warn_logPosition = floor(warn_logPosition);
                                }
                            }
                            else if (warn_logPosition > warn_maxLogPosition || warn_logPosition < 0)
                            {
                                warn_logPosition = warn_maxLogPosition;
                            }
                            C2D_Sprite temp;
                            C2D_SpriteFromSheet(&temp, debugConsole_SpriteSheet, 10);
                            C2D_SpriteMove(&temp, 0, controlBarY - 240);
                            C2D_DrawSprite(&temp);

                            for (int i = 0; i < (int)warnLog.size() - floor(warn_logPosition); i++)
                            {
                                if (i > maxLines)
                                {
                                    break;
                                }

                                Text::drawText(offset.x, offset.y + (i * 9 + (std::fmod(warn_logPosition, 1.0f) * -8)), 0.0f, warnLog[i + floor(warn_logPosition)].c_str(), consoleFont, 0.5f, 0.5f, C2D_Color32f(1.0f, 1.0f, 1.0f, 1.0f));
                            }
                            for (int i = 0; i < 20; i++)
                            {
                                C2D_SpriteFromSheet(&temp, debugConsole_SpriteSheet, 0);
                                C2D_SpriteMove(&temp, i * 16, controlBarY - 224);
                                C2D_DrawSprite(&temp);
                            }
                         //   Text::drawText(0, controlBarY - 224, 1.0f, "Warn Log", consoleFont, 0.5f, 0.5f, C2D_Color32f(1.0f, 1.0f, 1.0f, 1.0f));
                        }
                    }
                }
                catch (std::exception &ex)
                {
                    printf("\n[textConsoleComponent.draw() exception] : \n%s\n", ex.what());
                }
            }
        };
    }
}
