#pragma once

#include <inttypes.h>
#include <stdbool.h>

typedef struct {
	uint64_t id, blacklist;
} Group;

Group group_new();
void group_blacklist(Group *group, Group *other);
void group_blacklist_self(Group *group);
void group_whitelist(Group *group, Group *other);
void group_whitelist_self(Group *group);
bool group_interacts(Group *a, Group *b);
