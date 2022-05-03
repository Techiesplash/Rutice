#pragma once
#include <Rutice/Generic>
#include <Rutice/Internal/debug.hpp>
#include <Rutice/Internal/sprites.hpp>
namespace components
{

    class CursorComponent : public Component
    {
    public:
        SpriteAsset sprite1;
        void init() override
        {
            sprite1.loadSprite("missingTexture");
            object->textureAsset.animFrameTimer = 15;
        }

        void update() override
        {
            
            // Move the cursor to input.
            object->position.x = touch.px - 8;
            object->position.y = touch.py - 8;

            // Hide the cursor if there is no input.
            if (touch.px == 0 && touch.py == 0)
            {
                //object->visible = false;
            }
            else
            {
               // object->visible = true;
            }
        }

        void draw()
        {
            
            graphics::drawSprite(sprite1, 0, object->position.x + 16, object->position.y);
        }
    };
}
