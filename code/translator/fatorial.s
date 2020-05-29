global _start
;;;;SECTION TEXT ;;;;
section .text
_start:
;;;;LOAD N ;;;;
mov eax, dword[N]
;;;;FAT : SUB ONE ;;;;
FAT: sub eax, dword[ONE]
;;;;STORE AUX ;;;;
mov dword[AUX], eax
;;;;STOP ;;;;
__exit:
mov eax, 1
mov ebx, 0
int 0x80

;USED IO FUNCTIONS:

;;;;SECTION DATA ;;;;
section .data
__min_int db "-2147483648"
;;;;AUX : SPACE ;;;;
AUX dd 00
;;;;N : SPACE ;;;;
N dd 00
;;;;ONE : CONST 1 ;;;;
ONE dd 1
