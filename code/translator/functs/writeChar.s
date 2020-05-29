EscreverChar: ; Label in Portuguese due to teacher's requirements
enter 0,0
mov ecx, dword[ebp+8]
mov eax, 4
mov ebx, 1
mov edx, 1
int 0x80
mov eax, 1   ; quantity that has been read
leave
ret 4