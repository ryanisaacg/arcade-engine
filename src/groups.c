#include "groups.h"

Group group_new() {
	static uint64_t current = 0;
	if(current == 0) {
		current = 1;
	} else {
		current *= 2;
	}
	return (Group) { current, 0 };
}

void group_blacklist(Group *group, Group *other) {
	group->blacklist |= other->id;
}

void group_blacklist_self(Group *group) {
	group_blacklist(group, group);
}

void group_whitelist(Group *group, Group *other) {
	group->blacklist &= !other->id;
}

void group_whitelist_self(Group *group) {
	group_whitelist(group, group);
}

bool group_interacts(Group *a, Group *b) {
	return !(a->blacklist & b->id);
}
