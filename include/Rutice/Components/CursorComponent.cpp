#pragma once
#include <Rutice/Generic>
#include <Rutice/Internal/Components/debug.cpp>
namespace components
{

    class CursorComponent : public Component
    {
    public:
        void update() override
        {
            // Move the cursor to input.
            object->position.x = touch.px - 8;
            object->position.y = touch.py - 8;

            // Hide the cursor if there is no input.
            if (touch.px == 0 && touch.py == 0)
            {
                object->visible = false;
            }
            else
            {
                object->visible = true;
            }
        }
    };
}
