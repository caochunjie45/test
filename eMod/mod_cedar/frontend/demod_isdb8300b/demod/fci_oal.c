/*****************************************************************************
	Copyright(c) 2013 FCI Inc. All Rights Reserved

	File name : fci_oal.c

	Description : source of OS adaptation layer

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

	History :
	----------------------------------------------------------------------
*******************************************************************************/
#include "fci_types.h"

static __krnl_event_t* sem;


void msWait(__s32 ms)
{
    esKRNL_TimeDly((ms + 9)/10);
    return;
}

/* Write your own mutual exclusion method */
void OAL_CREATE_SEMAPHORE(void)
{
	/* called in driver initialization */
	sem = esKRNL_SemCreate(1);
}

void OAL_DELETE_SEMAPHORE(void)
{
	__u8 err;
	/* called in driver deinitializaton */

	esKRNL_SemDel(sem, OS_DEL_ALWAYS, &err);

}

void OAL_OBTAIN_SEMAPHORE(void)
{
	esKRNL_SemPend(sem,0,NULL);
}

void OAL_RELEASE_SEMAPHORE(void)
{
	esKRNL_SemPost(sem);
}


