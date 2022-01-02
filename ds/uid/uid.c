/******************************************
* Author:   Maya Petter                   *
* Reviewer: Hila Bagam                    *
* Purpose:                                *
*******************************************/
#include "uid.h"

const ilrd_uid_t BAD_UID = {0, 0};


ilrd_uid_t UIDCreate(void)
{
	static size_t counter = 1;
	ilrd_uid_t new_uid = {0};
	
	new_uid.uid = time(NULL);
	
	++counter;
	
	new_uid.counter = counter;
	
	return new_uid;
}

int UIDIsSame(ilrd_uid_t uid1, ilrd_uid_t uid2)
{
	/* returns 1 if same, 0 otherwise*/
	return ((uid1.uid == uid2.uid) && (uid1.counter == uid2.counter));  
}

