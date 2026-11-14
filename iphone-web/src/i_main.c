//-----------------------------------------------------------------------------
//
// Web / iPhone entry
//
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

#include "doomdef.h"
#include "m_argv.h"
#include "d_main.h"

int main(int argc, char** argv)
{
    char* web_argv[2];

    web_argv[0] = "doom";
    web_argv[1] = NULL;

    if (argc > 1)
    {
	myargc = argc;
	myargv = argv;
    }
    else
    {
	myargc = 1;
	myargv = web_argv;
    }

    setenv("HOME", "/home/web_user", 1);
    setenv("DOOMWADDIR", ".", 1);

    D_DoomMain();
    return 0;
}
