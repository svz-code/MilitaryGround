format PE CONSOLE
entry start

section '.text' code readable executable
start:
	mov eax, 1
	mov ebx, 332 ; так задумано, но в Vodka я не смог посмотреть код ошибки
	syscall
