#pragma once

#include <Rutice/Generic>

typedef struct SpriteFrames
{
    C2D_SpriteSheet sheetData;

    int frameCount;
};

class SpriteAsset
{

public:
    // Data
    SpriteFrames frames;
    bool dead = false;
    bool enableAnim = true;
    C2D_Sprite activeSprite;
    // Whet animation frame is the sprite on currently?
    int currentIndex = 0;
    // How many animation frames should occur between each frame?
    int animFrameTimer = 1;
    // For actually processing the animation
    int animFrameCount = 0;

    // Construction, destruction
    SpriteAsset(id)
    {
        // Load the sprite sheet
        // Check if file exists
        FILE *file = fopen("");
        if (!file)
        {
            // Load specific sprite
        }
        else
        {

            //  C2D_SpriteSheetCount();

            // Register the asset
            graphics::Internal::spriteAssets.push_back(this);
        }
        // Load the active sprite at index 0
        C2D_SpriteFromSheet(activeSprite, frames.sheetData, 0);
    }
    ~SpriteAsset()
    {
        bool dead = true;
    }

    /// Load sprite sheet data from filesystem
    void loadData(string path)
    {
    }

}

namespace graphics
{

    namespace Internal
    {
        // Management of all registered SpriteAssets loaded in memory
        std::vector<SpriteAsset &> spriteAssets;

        void Tick()
        {
            int i = 0;
            for (auto &a : spriteAssets)
            {
                // Remove dead
                if (a.dead)
                {
                    spriteAssets.erase(spriteAssets.begin() + i);
                }
                else
                {
                    // Logic
                    if (a.enableAnim)
                    {
                        // Animation logic

                        a.animFrameCount++;
                        if (a.animFrameCount >= a.animFrameTimer)
                        {
                            a.currentIndex++;
                            if (a.currentIndex >= a.frames.frameCount)
                            {
                                a.currentIndex = 0;
                            }
                            a.animFrameCount = 0;
                        }
                    }
                }

                i++;
            }
        }

    }

    void drawSprite(SpriteAsset sprite, int index, float x, float y)
    {
        if (index<0 | index> sprite.frames.frameCount - 1)
        {
            // Invalid index
            index = 0;
        }
    }

    SpriteAsset loadSprite(string path);
    {
    }
}