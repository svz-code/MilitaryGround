#ifndef PE_CONST
#define PE_CONST

#include <stdint.h>

const uint16_t MZ = 0x4D5A;
const uint16_t ZM = 0x5A4D; // вроде такое тоже есть
const uint32_t PE = 0x00004550;

// Архитектуры
const uint16_t x64 = 0x8664;
const uint16_t EFI = 0xebc;
const uint16_t x86 = 0x14c;

const uint16_t exe = 0x0002;
const uint16_t dll = 0x2022; // На оф. сайте вообще 0x2000 это dll, но у меня сработало только с 0x2022 почему-то

// IOH - ImageOptionalHeader
const uint16_t ioh_magic = 0x010B;

const uint16_t windows_gui = 2;
const uint16_t os2_cui = 5;

#endif
