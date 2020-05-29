EscreverString: ; Label in Portuguese due to teacher's requirements
enter 4,0
mov ecx, dword[ebp + 12] ; size
__repeatWriteString:
push ecx
mov eax, 4
mov ebx, 1
mov ecx, dword[ebp+8]
mov edx, 1
int 0x80
mov eax, ecx    ;string[i]
inc ecx         ;string[i+1]
inc dword[ebp-4]    ; count++
mov dword[ebp + 8], ecx
pop ecx
loopne __repeatWriteString
mov eax, [ebp+12] ; size
leave
ret 8