//-----------------------------------------------------------------------------
//
// Web / iPhone platform: canvas framebuffer + key queue
//
//-----------------------------------------------------------------------------

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "doomstat.h"
#include "i_system.h"
#include "v_video.h"
#include "m_argv.h"
#include "d_main.h"
#include "doomdef.h"
#include "d_event.h"

#define KEYQ_SIZE 64

static unsigned int s_palette[256];
static uint32_t* s_rgba = NULL;
static int s_gfx_ok = 0;

static struct {
    int pressed;
    int key;
} s_keyq[KEYQ_SIZE];
static int s_keyq_r = 0;
static int s_keyq_w = 0;

EMSCRIPTEN_KEEPALIVE
void I_PushKey(int pressed, int key)
{
    int next = (s_keyq_w + 1) % KEYQ_SIZE;
    if (next == s_keyq_r)
	return;
    s_keyq[s_keyq_w].pressed = pressed;
    s_keyq[s_keyq_w].key = key;
    s_keyq_w = next;
}

EMSCRIPTEN_KEEPALIVE
uint32_t* I_GetFrameBuffer(void)
{
    return s_rgba;
}

EMSCRIPTEN_KEEPALIVE
int I_GetScreenWidth(void)
{
    return SCREENWIDTH;
}

EMSCRIPTEN_KEEPALIVE
int I_GetScreenHeight(void)
{
    return SCREENHEIGHT;
}

void I_ShutdownGraphics(void)
{
    s_gfx_ok = 0;
}

void I_StartFrame(void)
{
}

void I_StartTic(void)
{
    event_t ev;

    while (s_keyq_r != s_keyq_w)
    {
	ev.type = s_keyq[s_keyq_r].pressed ? ev_keydown : ev_keyup;
	ev.data1 = s_keyq[s_keyq_r].key;
	ev.data2 = 0;
	ev.data3 = 0;
	D_PostEvent(&ev);
	s_keyq_r = (s_keyq_r + 1) % KEYQ_SIZE;
    }
}

void I_UpdateNoBlit(void)
{
}

void I_FinishUpdate(void)
{
    int i;
    byte* src;

    if (!s_gfx_ok || !s_rgba || !screens[0])
	return;

    src = screens[0];
    for (i = 0; i < SCREENWIDTH * SCREENHEIGHT; i++)
	s_rgba[i] = s_palette[src[i]];

#ifdef __EMSCRIPTEN__
    EM_ASM({ if (Module.onFrame) Module.onFrame(); });
#endif
}

void I_ReadScreen(byte* scr)
{
    memcpy(scr, screens[0], SCREENWIDTH * SCREENHEIGHT);
}

void I_SetPalette(byte* palette)
{
    int i;
    int r, g, b;
    extern byte gammatable[5][256];
    extern int usegamma;

    for (i = 0; i < 256; i++)
    {
	r = gammatable[usegamma][*palette++] & ~3;
	g = gammatable[usegamma][*palette++] & ~3;
	b = gammatable[usegamma][*palette++] & ~3;
	/* ABGR for little-endian ImageData-compatible packing via HEAPU8 */
	s_palette[i] = (0xffu << 24) | ((unsigned)b << 16) | ((unsigned)g << 8) | (unsigned)r;
    }
}

void I_InitGraphics(void)
{
    static int first = 1;
    byte* doompal;

    if (!first)
	return;
    first = 0;

    screens[0] = (byte*)malloc(SCREENWIDTH * SCREENHEIGHT);
    s_rgba = (uint32_t*)malloc(SCREENWIDTH * SCREENHEIGHT * 4);
    if (!screens[0] || !s_rgba)
	I_Error("I_InitGraphics: alloc failed");

    memset(screens[0], 0, SCREENWIDTH * SCREENHEIGHT);
    memset(s_rgba, 0, SCREENWIDTH * SCREENHEIGHT * 4);

    /* Palette is set later via I_SetPalette from the game. */
    (void)doompal;
    s_gfx_ok = 1;

#ifdef __EMSCRIPTEN__
    EM_ASM({
	var el = document.getElementById('status');
	if (el) el.textContent = String();
    });
#endif
}
