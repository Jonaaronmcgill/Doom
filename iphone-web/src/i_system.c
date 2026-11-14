//-----------------------------------------------------------------------------
//
// Web / iPhone platform: system timing, zone, errors
// Based on linuxdoom-1.10 i_system.c
//
//-----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "doomdef.h"
#include "m_misc.h"
#include "i_video.h"
#include "i_sound.h"

#include "d_net.h"
#include "g_game.h"

#include "i_system.h"

int	mb_used = 6;

void I_Tactile(int on, int off, int total)
{
    (void)on;
    (void)off;
    (void)total;
}

ticcmd_t emptycmd;
ticcmd_t* I_BaseTiccmd(void)
{
    return &emptycmd;
}

int I_GetHeapSize(void)
{
    return mb_used * 1024 * 1024;
}

byte* I_ZoneBase(int* size)
{
    *size = mb_used * 1024 * 1024;
    return (byte*)malloc(*size);
}

int I_GetTime(void)
{
#ifdef __EMSCRIPTEN__
    return (int)(emscripten_get_now() * TICRATE / 1000.0);
#else
    struct timeval tp;
    static int basetime = 0;
    gettimeofday(&tp, NULL);
    if (!basetime)
	basetime = tp.tv_sec;
    return (tp.tv_sec - basetime) * TICRATE + tp.tv_usec * TICRATE / 1000000;
#endif
}

void I_Init(void)
{
    I_InitSound();
}

void I_Quit(void)
{
    D_QuitNetGame();
    I_ShutdownSound();
    I_ShutdownMusic();
    M_SaveDefaults();
    I_ShutdownGraphics();
#ifdef __EMSCRIPTEN__
    emscripten_cancel_main_loop();
#endif
    exit(0);
}

void I_WaitVBL(int count)
{
    (void)count;
}

void I_BeginRead(void)
{
}

void I_EndRead(void)
{
}

byte* I_AllocLow(int length)
{
    byte* mem = (byte*)malloc(length);
    memset(mem, 0, length);
    return mem;
}

extern boolean demorecording;

void I_Error(char* error, ...)
{
    va_list argptr;

    va_start(argptr, error);
    fprintf(stderr, "Error: ");
    vfprintf(stderr, error, argptr);
    fprintf(stderr, "\n");
    va_end(argptr);
    fflush(stderr);

    if (demorecording)
	G_CheckDemoStatus();

    D_QuitNetGame();
    I_ShutdownGraphics();

#ifdef __EMSCRIPTEN__
    emscripten_cancel_main_loop();
    EM_ASM({
	var msg = UTF8ToString($0);
	var el = document.getElementById('status');
	if (el) el.textContent = 'Error: ' + msg;
    }, error);
#endif
    exit(-1);
}
