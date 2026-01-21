#ifndef VODKA_FUNC
#define VODKA_FUNC

#include "pe_structs.h"

int load(char* file);
int check(FILE* file);

int launch(FILE* file, struct ImageOptionalHeader32 ioh, uint16_t number_of_sections);

#endif
