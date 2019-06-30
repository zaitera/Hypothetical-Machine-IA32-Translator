LerString:
enter 4,0
mov dword[ebp-4], 0
mov ebx, 0
mov edx, 1
mov ecx, dword[ebp + 12]
__repeat_read:
mov eax, 3
push ecx
mov ecx, dword[ebp+8]
int 0x80
mov eax, ecx
inc ecx
inc dword[ebp-4]
cmp byte[eax], 0xA
mov dword[ebp + 8], ecx
pop ecx
loopne __repeat_read
mov eax, [ebp-4]
leave
ret 8