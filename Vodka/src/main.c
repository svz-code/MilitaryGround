#include <stdio.h>
#include "include/func.h"

int main(int argc, char** argv) {
    printf("Добро пожаловать в Vodka! Сейчас парсер Vodka проанализирует ваш .exe.\n");
    printf("Ваш аргумент (путь к файлу): %s\n", argv[1]);
    printf("Не переживайте, наша команда профессионалов начала работу.\nОжидайте.\n");
    
    load(argv[1]);
    
    return 0;
}
