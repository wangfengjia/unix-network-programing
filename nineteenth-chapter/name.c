#include "lib.h"

int
idlistlookup(char *name, struct idlist *il)
{
	for (; il->name != NULL; il++) {
		if (strcmp(name, il->name) == 0)
			return il->val;
	}
	return -1;
}

int
getsatypebyname(char *name)
{
	return idlistlookup(name, satype);
}

int
getsaalgbyname(int type, char *name)
{
	switch (type) {
	case SADB_SATYPE_AH:
		return idlistlookup(name, ahalg);
	case SADB_SATYPE_ESP:
		return idlistlookup(name, espalg);
	default:
		return -1;
	}
}
