#pragma once

#include <Rutice/Internal/basics.hpp>
#include <Rutice/Internal/types.hpp>

namespace Text
{
    C2D_TextBuf dynamicBuf;

    void init()
    {
        dynamicBuf = C2D_TextBufNew(4096);
    }

    void drawText(float x, float y, float z, const char* str, C2D_Font font, float sx, float sy)
    {
        C2D_Text temp;
        C2D_TextFontParse(&temp, font, dynamicBuf, str);
        C2D_TextOptimize(&temp);
        C2D_DrawText(&temp, C2D_WithColor, x, y, z, sx, sy, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
       
    }
    void drawText(float x, float y, float z, const char* str, C2D_Font font, float sx, float sy, u32 color)
    {
        C2D_Text temp;
        C2D_TextFontParse(&temp, font, dynamicBuf, str);
        C2D_TextOptimize(&temp);
        C2D_DrawText(&temp, C2D_WithColor, x, y, z, sx, sy, color);
       
    }

    void deinit()
    {
        C2D_TextBufDelete(dynamicBuf);
    }
}