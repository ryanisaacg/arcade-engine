#pragma once

#define assert(name, test) do { if (!(test)) return name; } while (0)
#define run_test(test) do { char *name = test(); tests_run++; \
                                if (name != NULL) { return name; } } while (0)
extern int tests_run;
