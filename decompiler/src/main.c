#include <stdio.h>
#include <inttypes.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <Zydis/Zydis.h>
#include <libelf.h>
#include <gelf.h>

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
    
    ZyanU32 mode = (ehdr.e_machine == EM_X86_64) ? ZYDIS_MACHINE_MODE_LONG_64 : ZYDIS_MACHINE_MODE_LEGACY_32;
    ZyanU32 width = (ehdr.e_machine == EM_X86_64) ? ZYDIS_STACK_WIDTH_64 : ZYDIS_STACK_WIDTH_32;
    
    if (ehdr.e_type != ET_EXEC) {
        printf("Да иди ты нафиг, это не executable file! Сворачиваемся.\n");
        return 1;
    }
    
    ZyanU64 entry_point = ehdr.e_entry;
    printf("Фух. Мы прошли проверки 🎉\nВот entry point: %lu\n", entry_point);
    
    ZyanUSize phnum;
    elf_getphdrnum(e, &phnum);
    GElf_Phdr phdr;
    
    int64_t rax = 0;
    
    for (ZyanUSize i = 0; i < phnum; i++) {
        gelf_getphdr(e, i, &phdr);
        
        if (phdr.p_type == PT_LOAD && (phdr.p_flags & PF_X)) {
            printf("Уххх экзекьютабле секция номер %zu с размером %lu \n", i, phdr.p_memsz);
            
            ZyanU8 *data = malloc(phdr.p_filesz);
            lseek(fd, phdr.p_offset, SEEK_SET);
            read(fd, data, phdr.p_filesz);
            
            ZyanU64 entry_offset = entry_point - phdr.p_vaddr;
            if (entry_offset < phdr.p_filesz) {
                printf("Ухх энтри поинт с оффсетом %lu\n", entry_offset);
                ZydisDecoder decoder;
                ZydisDecoderInit(&decoder, mode, width);
                
                ZydisFormatter formatter;
                ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);
                
                ZyanUSize offset = entry_offset;
                ZyanU64 runtime_address = entry_point;
                
                ZydisDecodedInstruction instruction;
                ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT];
                
                while (offset < phdr.p_filesz && ZYAN_SUCCESS(ZydisDecoderDecodeFull(&decoder, data + offset, phdr.p_filesz - offset, &instruction, operands))) {
                    printf("%016" PRIX64 "  ", runtime_address);
                    
                    char buffer[256];
                    ZydisFormatterFormatInstruction(&formatter, &instruction, operands, instruction.operand_count_visible, buffer, sizeof(buffer), runtime_address, ZYAN_NULL);
                    puts(buffer);
                    
                    if (instruction.mnemonic == ZYDIS_MNEMONIC_MOV) {
                        if (operands[1].type == ZYDIS_OPERAND_TYPE_IMMEDIATE) {
                            if (operands[0].reg.value == ZYDIS_REGISTER_RAX) {
                                //TODO: научится определять, signed или unsigned
                                rax = operands[1].imm.value.s;
                                //printf("Ой, простите, вмешаюсь. RAX у нас: %ld\n", rax);
                            }
                        }
                    }
                    
                    if (instruction.mnemonic == ZYDIS_MNEMONIC_SYSCALL) {
                        if (rax == 60) {
                            printf("Ну, где-то здесь программа должна закончится по идее.\n");
                            break;
                        }
                    }
                    
                    offset += instruction.length;
                    runtime_address += instruction.length;
                }
            }
            
            free(data);
        }
    }
    
    elf_end(e);
    close(fd);
    return 0;
}
