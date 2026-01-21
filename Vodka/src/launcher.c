#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "include/func.h"

int launch(FILE* file, struct ImageOptionalHeader32 ioh, uint16_t number_of_sections) {
    void* ib = mmap((void*)ioh.image_base, ioh.size_of_image, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    if (ib == MAP_FAILED) {
        printf("Это было логично, но мы факапнулись.\n");
        return 1;
    }
    
    struct ImageSectionHeader sections[number_of_sections];
    fread(sections, sizeof(struct ImageSectionHeader), number_of_sections, file);

    for (int i = 0; i < number_of_sections; i++) {
        fseek(file, sections[i].pointer_to_raw_data, SEEK_SET);
        fread((char*)ib + sections[i].virtual_address, sections[i].size_of_raw_data, 1, file);
    }
    
    void (*entry_point)() = (void(*)())((char*)ib + ioh.address_of_entry_point); // стращна
    entry_point();
    
    return 0;
}
