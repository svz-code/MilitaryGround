#include <stdio.h>
#include <inttypes.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <Zydis/Zydis.h>
#include <libelf.h>
#include <gelf.h>

#include "vec.h"

int main(int argc, char** argv) {
    printf("От создателей $sudo-bot, TestPrefSumm, Vodka, и udpping/tcpping...\nВстречайте: КАРТОН тхе ДЕКОМПИЛЕР 🔥🔥🔥\n");
    
    if (argc != 2) {
        printf("Больше/меньше 1 аргумента не принимаю, ну ты в курсе, ты либо дай один файл, либо не запускай эту программу.\nСворачиваемся.\n");
        return 1;
    }
    
    if (elf_version(EV_CURRENT) == EV_NONE) {
        printf("Чет как-то не вышло, не фортануло, ну ты в курсе, сворачиваемся.\n");
        return 1;
    }
    
    int fd = open(argv[1], O_RDONLY, 0);
    if (fd < 0) {
        printf("Пососи, выдал те код ошибки open, думай дальше сам, сворачиваемся.\n");
        return fd;
    }
    
    Elf *e = elf_begin(fd, ELF_C_READ, NULL);
    if (e == NULL) {
        printf("Афигеть, elf_begin в чем-то...Нуу, сам понимаешь, сворачиваемся.\n");
        return 1;
    }
    
    if (elf_kind(e) != ELF_K_ELF) {
        printf("Откуда ты вообще достал этот файл? Это не ELF, если это PE - иди Vodka мучай, не нас.\nСворачиваемся.\n");
        return 1;
    }
    
    GElf_Ehdr ehdr;
    if (gelf_getehdr(e, &ehdr) == NULL) {
        printf("Да ты издеваешься, все, я лив, сворачиваемся.\n");
        return 1;
    }
    
    if (ehdr.e_machine != EM_X86_64 && ehdr.e_machine != EM_386) {
        printf("Ой, все, пока, сворачиваемся.\n");
        return 1;
    }
    
    if (ehdr.e_type != ET_EXEC) {
        printf("Да иди ты нафиг, это не executable file! Сворачиваемся.\n");
        return 1;
    }
    
    ZyanU64 entry_point = ehdr.e_entry;
    printf("Фух. Мы прошли проверки 🎉\nВот entry point: %lu\n", entry_point);
    
    ZyanUSize phnum;
    elf_getphdrnum(e, &phnum);
    GElf_Phdr phdr;
    ZyanUSize file_offset = -1; // я отлично осведомлен о том, что unsigned пошлет меня куда подальше с этим -1
    
    for (ZyanUSize i = 0; i < phnum; i++) {
        gelf_getphdr(e, i, &phdr);
        if (phdr.p_type == PT_LOAD && entry_point >= phdr.p_vaddr && entry_point < (phdr.p_vaddr + phdr.p_memsz)) {
            file_offset = phdr.p_offset + (entry_point - phdr.p_vaddr);
            break;
        }
    }
    
    if (file_offset == (ZyanUSize)-1) {
        printf("Гг, тима раков, сворачиваемся.\n");
        return 1;
    }
    
    ZyanU8* data = vector_create();
    vector_reserve(&data, 256);
    lseek(fd, file_offset, SEEK_SET);
    ZyanUSize bread = read(fd, data, 256);
    
    if (bread <= 0) {
        printf("⚡️⚡️⚡️ Чтение - В С Е. Сворачиваемся.\n");
        return 1;
    }
    
    ZydisDecoder decoder;
    ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64);
    
    ZydisFormatter formatter;
    ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);
    
    ZyanU64 runtime_address = entry_point;
    ZyanUSize offset = 0;
    
    ZydisDecodedInstruction instruction;
    ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT];
    while (ZYAN_SUCCESS(ZydisDecoderDecodeFull(&decoder, data + offset, bread - offset,
        &instruction, operands))) {
        printf("%016" PRIX64 "  ", runtime_address);
        
        char buffer[256];
        ZydisFormatterFormatInstruction(&formatter, &instruction, operands, instruction.operand_count_visible, buffer, sizeof(buffer), runtime_address, ZYAN_NULL);
        puts(buffer);
        
        offset += instruction.length;
        runtime_address += instruction.length;
    }
    
    vector_free(data);
    elf_end(e);
    close(fd);
    return 0;
}
