EscreverChar:
enter 0,0
mov ecx, dword[ebp+8]
mov eax, 4
mov ebx, 1
mov edx, 1
int 0x80
mov eax, 1
leave
ret 4