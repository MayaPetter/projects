#include <assert.h> /* assert */

#include "uid.h"

void CheckUid()
{
	ilrd_uid_t new_uid1 = {0, 0};
	ilrd_uid_t new_uid2 = {0, 0};
	ilrd_uid_t new_uid3 = {0, 0};
	
	new_uid1 = UIDCreate();
	new_uid2 = UIDCreate();
	new_uid3 = new_uid2;
	
	assert(0 == UIDIsSame(new_uid1, BAD_UID));
	assert(0 == UIDIsSame(new_uid2, BAD_UID));
	assert(0 == UIDIsSame(new_uid3, BAD_UID));
	assert(1 == UIDIsSame(new_uid3, new_uid2));
	assert(0 == UIDIsSame(new_uid1, new_uid2)); 
	 
}

int main()
{
	CheckUid();
	
	return 0;
}


