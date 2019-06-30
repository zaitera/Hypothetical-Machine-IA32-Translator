LeerChar:
enter 0,0
mov ecx, dword[ebp+8]
mov eax, 3
mov ebx, 0
mov edx, 1
int 0x80
mov eax, 1
leave
ret 4