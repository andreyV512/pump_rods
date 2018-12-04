#include "stdafx.h"
#include "Hash.h"

unsigned int HashFAQ6(unsigned char *str, unsigned len)
{
	unsigned int hash = 0;

	for (unsigned i = 0; i < len; ++i)
	{
		hash += str[i];
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);

	return hash;
}