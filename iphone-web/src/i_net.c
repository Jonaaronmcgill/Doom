//-----------------------------------------------------------------------------
//
// Web / iPhone platform: single-player networking stub
//
//-----------------------------------------------------------------------------

#include <stdlib.h>
#include <string.h>

#include "doomdef.h"
#include "doomstat.h"
#include "i_system.h"
#include "d_net.h"
#include "m_argv.h"
#include "i_net.h"

void I_InitNetwork(void)
{
    doomcom = malloc(sizeof(*doomcom));
    memset(doomcom, 0, sizeof(*doomcom));

    netgame = false;
    doomcom->id = DOOMCOM_ID;
    doomcom->numplayers = doomcom->numnodes = 1;
    doomcom->deathmatch = false;
    doomcom->consoleplayer = 0;
    doomcom->ticdup = 1;
    doomcom->extratics = 0;
}

void I_NetCmd(void)
{
    /* Single-player: no packets. */
    if (doomcom->command == CMD_GET)
	doomcom->remotenode = -1;
}
