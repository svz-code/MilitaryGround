#include <stdio.h>
#include "include/func.h"
#include "include/pe_const.h"

int load(char* file) {
    FILE* f;
    f = fopen(file, "rb");
    if (!f) {
        printf("Эээ...Ты нам неверные данные дал. Это 307 УК РФ))\n");
        return 1;
    }
    
    return check(f);
}

int check(FILE* file) {
    struct ImageDosHeader idh;
    
    fread(&idh, sizeof(struct ImageDosHeader), 1, file);
    
    if (!((idh.signature == MZ) || (idh.signature == ZM))) { // Капец, условие надо переписать по-хорошему
        printf("Мы все понимаем, но это вообще не .exe. Пока\n");
        return 1;
    }
    
    struct ImageNtHeaders32 inth;
    
    fseek(file, idh.pe_header_address, SEEK_SET);
    fread(&inth, sizeof(struct ImageNtHeaders32), 1, file);
    
    if (inth.signature == PE) {
        printf("Да, это PE файл. Мы можем продолжать работу!\n");
    } else {
        printf("Это не PE файл. Пока.\n");
        return 1;
    }
    
    struct ImageFileHeader ifh = inth.file_header;
    struct ImageOptionalHeader32 ioh = inth.optional_header;
    
    if (ifh.machine == x86) {
        printf("Вау, оно и 32-битное! Мы РЕАЛЬНО можем продолжить работу!\n");
    } else if (ifh.machine == x64) {
        printf("Ахах, соррян, я думаю, мы не готовы к x64.\n");
        return 1;
    } else if (ifh.machine == EFI) {
        printf("Нахера козе баян?!\n");
        return 1;
    } else {
        printf("Ты что нам вообще дал?\n");
        return 1;
    }
    
    /*if (ifh.characteristics & dll) {
        printf("Ахах, это dll. Ливаем!\n");
        return 1;
    }*/
    
    if (ioh.magic_number != ioh_magic) {
        printf("Согласно документации Microsoft, \"магическое\" число IOH всегда равно 0x010B. \nТы что такое?\n");
        return 1;
    }
    
    if (ioh.subsystem == windows_gui) {
        printf("Ахах, GUIшка не поддерживается.\n");
        return 1;
    } else if (ioh.subsystem == os2_cui) {
        printf("И зачем нам OS/2? xD\n");
        return 1;
    }
    
    printf("Если мы дошли до сюда, то .exe все проверки прошел.\nКоманда LeadWay уже вызвала бригадиров для запуска (псевдо-).exe файла.\nЕсли вы нам дали .exe файл от Windows, значит вы не прочитали README.md к Vodka.\n");
    
    return launch(file, ioh, ifh.number_of_sections);
}
