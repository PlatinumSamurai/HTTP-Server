; Блок 1
section .text
global _start
_start:

xor edx, edx
push edx
push 0x74756f2e
push 0x74736574
push 0x5f707474
push 0x682f2f2e
mov esi, esp

; Блок 5
push edx
push esi
mov ecx, esp
xor eax, eax
mov al, 59
int 0x80

