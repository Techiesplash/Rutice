#pragma once

//#include <Rutice/Internal/data.hpp>
#include <Rutice/Internal/basics.hpp>
#include <Rutice/Internal/debug.hpp>


typedef struct
{
    C2D_SpriteSheet sheetData;

    int frameCount;
} SpriteFrames;

bool fileExists(string path)
{
    FILE *file = fopen(path.c_str(), "r");
    if (!file)
    {
        return false;
    }
    return true;
}

class SpriteAsset
{

public:
    // Data
    SpriteFrames frames;
    bool enableAnim = true;
    C2D_Sprite activeSprite;
    int prevIndex = 0;
    // Whet animation frame is the sprite on currently?
    int currentIndex = 0;
    // How many animation frames should occur between each frame?
    int animFrameTimer = 1;
    // For actually processing the animation
    int animFrameCount = 0;

    // Construction, destruction
    /// Argument is the name of sprite file, minus extension
    SpriteAsset(string name);

    ~SpriteAsset();

    /// Load sprite sheet data from filesystem
    /// Argument is the name of sprite file, minus extension
    void loadSprite(string name);
};

namespace graphics
{

    namespace Internal
    {
        // Management of all registered SpriteAssets loaded in memory
        std::vector<std::unique_ptr<SpriteAsset>> spriteAssets;

        void Tick()
        {

            for (int i = 0; i < (int)spriteAssets.size(); i++)
            {

                // Logic
                if (spriteAssets[i]->enableAnim && spriteAssets[i]->frames.frameCount > 0)
                {
                    // Animation logic

                    spriteAssets[i]->animFrameCount++;
                    if (spriteAssets[i]->animFrameCount >= spriteAssets[i]->animFrameTimer)
                    {
                        spriteAssets[i]->currentIndex++;
                        if (spriteAssets[i]->currentIndex >= spriteAssets[i]->frames.frameCount)
                        {
                            spriteAssets[i]->currentIndex = 0;
                        }
                        spriteAssets[i]->animFrameCount = 0;
                    }
                }
            }
        }
        /// Recommended to use for deinitialization
        /// Will break things if used elsewhere
        void Wipe()
        {
            spriteAssets.clear();
        }
    }

    void drawSprite(SpriteAsset sprite, int index, float x, float y)
    {
 
        int index1 = 0;
        if (index < -1 || index > sprite.frames.frameCount - 1)
        {
            // Invalid index
            index1 = 0;
        }
        else
        {
            index1 = index;
        }
        if (index == -1)
        {
            
            index1 = sprite.currentIndex;
            if (sprite.currentIndex != sprite.prevIndex)
            {
                C2D_SpriteFromSheet(&sprite.activeSprite, sprite.frames.sheetData, index1);
                sprite.prevIndex = sprite.currentIndex;
            }
            sprite.activeSprite.params.pos.x = x;
            sprite.activeSprite.params.pos.y = y;

            C2D_DrawSprite(&sprite.activeSprite);
        }
        else
        {
          //  debugConsole::debug::Log("I0=" + std::to_string(index));
          //  debugConsole::debug::Log("I1=" + std::to_string(index1));
            if (sprite.frames.frameCount > 0)
            {
                if (sprite.prevIndex != index1)
                {
                    C2D_SpriteFromSheet(&sprite.activeSprite, sprite.frames.sheetData, index1);
                }

                sprite.activeSprite.params.pos.x = x;
                sprite.activeSprite.params.pos.y = y;

                C2D_DrawSprite(&sprite.activeSprite);
                sprite.prevIndex = index1;
            }
        }
    }
}

SpriteAsset::SpriteAsset(string name = "")
{
    // Load the sprite sheet
    // Check if file exists

    if (!fileExists("romfs:/gfx/" + name + ".t3x"))
    {
        // Load specific sprite
        string tempstr = "romfs:/gfx/missingTexture.t3x";
        frames.sheetData = C2D_SpriteSheetLoad(tempstr.c_str());
    }
    else
    {
        string tempstr = "romfs:/gfx/" + name + ".t3x";
        frames.sheetData = C2D_SpriteSheetLoad(tempstr.c_str());
    }
    // Register the asset
    std::unique_ptr<SpriteAsset> uPtr(this);
    graphics::Internal::spriteAssets.push_back(std::move(uPtr));

    // Count the number of frames available
    frames.frameCount = C2D_SpriteSheetCount(frames.sheetData);

    // Load the active sprite at index 0
    C2D_SpriteFromSheet(&activeSprite, frames.sheetData, 0);
}
SpriteAsset::~SpriteAsset()
{
    svcOutputDebugString("Destroyed SpriteAsset.", sizeof("Destroyed SpriteAsset."));
   // C2D_SpriteSheetFree(frames.sheetData);
}

/// Load sprite sheet data from filesystem
void SpriteAsset::loadSprite(string name)
{
    C2D_SpriteSheetFree(frames.sheetData);
    
    if (!fileExists("romfs:/gfx/" + name + ".t3x"))
    {
        // Load specific sprite
        string tempstr = "romfs:/gfx/missingTexture.t3x";
        frames.sheetData = C2D_SpriteSheetLoad(tempstr.c_str());
    }
    else
    {
        string tempstr = "romfs:/gfx/" + name + ".t3x";
        frames.sheetData = C2D_SpriteSheetLoad(tempstr.c_str());
    }
    // Register the asset
    std::unique_ptr<SpriteAsset> uPtr(this);
    graphics::Internal::spriteAssets.push_back(std::move(uPtr));

    // Count the number of frames available
    frames.frameCount = C2D_SpriteSheetCount(frames.sheetData);

    // Load the active sprite at index 0
    C2D_SpriteFromSheet(&activeSprite, frames.sheetData, 0);
    
}