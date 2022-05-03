#include <Rutice/Generic>
#include <Rutice/Internal/sound.hpp>
#include <Rutice/Internal/data.hpp>
#include <Rutice/Internal/gc/gc.hpp>
#include <malloc.h>


//#include <Rutice/Internal/libs/gc/include/gcdeps.hpp>
//#include <gc.h>
extern "C" void critical_signal_handler(int signal_number)
{
    
    consoleInit(GFX_BOTTOM, NULL);
    // Identify what signal it is
    string temp = "UNKNOWN";
    string desc = "No description available.";
    switch (signal_number)
    {
    case SIGABRT:
        temp = "SIGABRT";
        desc = "Signal 6 ( SIGABRT ) - Abort Signal";
    case SIGSEGV:
        temp = "SIGSEGV";
        desc = "Signal 11 ( SIGSEGV ) - Segmentation Fault";
        break;
    default:
        temp = "UNKNOWN";
        desc = "No description available.";
        break;
    }

    // Inform the user
    // consoleClear();
    printf("\n\n----[ Termination Signal ]");
    printf("\nSignal number %i ( %s ) raised. ", signal_number, temp.c_str());
    printf("\n\n----[ Description ]\n%s\n", desc.c_str());
    printf("\n\n Press START to exit.\n");

    // ?????
    bool loop = true;
    while (loop)
    {
        hidScanInput();

        u32 kDown = hidKeysDown();
        if (kDown & KEY_START)
        {
            loop = false;
        }
    }

    exit(0);
    // PROFIT.
}

void exitFunc(void)
{
    printf("\nPress START to exit");

    bool loop = true;
    while (loop)
    {
        hidScanInput();

        u32 kDown = hidKeysDown();
        if (kDown & KEY_START)
        {
            loop = false;
        }
    }
}

int main()
{
    debugConsole::debug::Log("MIT LICENSED 3DS Engine - 'Rutice'\nSprite-based engine for Nintendo 3DS\n");

    

    //	debug::Log("data.Internal.checkForTag returned " + std::to_string(data::Internal::checkForTag("<tag>fadsfasf</tag>","tag1")));

    // Abort handling
    signal(SIGABRT, &critical_signal_handler);
    signal(SIGSEGV, &critical_signal_handler);
    // signal(SIGSTOP, &critical_signal_handler);
    //	signal(SIGQUIT, &critical_signal_handler);

    // std::atexit(&exitFunc);

    // Initialize graphics
    romfsInit();
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    cfguInit();
    csndInit();
    Text::init();
    srvInit();
    aptInit();
    hidInit();
    consoleInit(GFX_BOTTOM, NULL);
    gdbHioDevInit();
    
    Render_top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    Render_bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_RIGHT);

    Scene *scene = loadSceneUnstable("testing");

    //Scene *consoleScene = loadScene("debugConsole", Render_bottom);
    printf("Console initialized.");
    //	printf("GPLv3 3DS Engine - 'Rutice'\nSprite-based engine for Nintendo 3DS\n");

    updateRate = 0;
    
    // Main loop
    while (aptMainLoop())
    {
     //   printf("\x1b[36m");
    //    	printf("\x1b[36m\x1b[2;1HCPU:     %6.2f%%\x1b[K\x1b[0m", C3D_GetProcessingTime() * 6.0f);
    //    	printf("\x1b[36m\x1b[3;1HGPU:     %6.2f%%\x1b[K\x1b[0m", C3D_GetDrawingTime() * 6.0f);
    //    	printf("\x1b[36m\x1b[4;1HCmdBuf:  %6.2f%%\x1b[K\x1b[0m", C3D_GetCmdBufUsage() * 100.0f);
    //    	printf("\x1b[0m");
        graphics::Internal::Tick();

        	

        C2D_TextBufClear(Text::dynamicBuf);
        // gspWaitForVBlank();
        // gfxSwapBuffers();

        // Inputs
        hidScanInput();
        hidTouchRead(&touch);

        u32 kDown = hidKeysDown();
        if (kDown & KEY_START)
        {
            // Break in order to return to Homebrew Menu
            break;
        }

        if (kDown & KEY_DDOWN)
        {
            Controls::dPadDown = true;
        }
        else
        {
            Controls::dPadDown = false;
        }

        if (kDown & KEY_DUP)
        {
            Controls::dPadUp = true;
        }
        else
        {
            Controls::dPadUp = false;
        }
        if (kDown & KEY_DLEFT)
        {
            Controls::dPadLeft = true;
        }
        else
        {
            Controls::dPadLeft = false;
        }

        // Events
        scene->Tick();
   //     consoleScene->Tick();
        
        // Top screen
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(Render_top, C2D_Color32f(0.1f, 0.1f, 0.1f, 1.0f));
        C2D_SceneBegin(Render_top);
        
        scene->Render(Render_top);

        C3D_FrameEnd(0);
     
        // Bottom screen
    //    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
      //  C2D_TargetClear(Render_bottom, C2D_Color32f(0.1f, 0.1f, 0.1f, 1.0f));
    //    C2D_SceneBegin(Render_bottom);

        //	scene->Render(Render_bottom);
    //    consoleScene->Render(Render_bottom);

        // C2D_Text temp;

        // C2D_TextFontParse(&temp, components::consoleFont, Text::dynamicBuf, "Sample Text");
        // C2D_TextOptimize(&temp);
        // C2D_DrawText(&temp, C2D_WithColor, 1, 1, 1, 0.5f, 0.5f, C2D_Color32f(1.0f, 1.0f, 1.0f, 1.0f));

        // Text::drawText(0.0f, 0.0f, 10.0f, "Sample Text", components::consoleFont, 0.5f, 0.5f);
      //  C3D_FrameEnd(0);


      //  int i;
       // const int size = 10;
 
      //  for (i = 0; i < size; ++i)
      //  {
            //GC::Pointer<int> pInt;
      //      GC::Pointer<int> iPtr = (int*)GC::allocate(sizeof(int), (void**)iPtr);
          //  pInt[0] = 0;
           
      //  }
        printf("OS_USEDBYTES=%i\n", osGetMemRegionUsed(MEMREGION_ALL));
        printf("GC-AllocatedBytes=%i\n", GC::GarbageCollector::GetTotalBytesAllocated());
        
        
        
    }
    audio::audio_stop();
    graphics::Internal::Wipe();

    // Deinitialize and quit
    gdbHioDevExit();
    csndExit();
    C2D_Fini();
    C3D_Fini();
    gfxExit();
    cfguExit();
    romfsExit();
    hidExit();
    aptExit();
    srvExit();
    Text::deinit();

    return 0;
}
