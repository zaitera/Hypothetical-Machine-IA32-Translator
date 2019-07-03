LerString: ; Label in Portuguese due to teacher's requirements
enter 4,0
mov dword[ebp-4], 0 ; save 0
mov ecx, dword[ebp + 12]   ;size string
__repeatReadString:
push ecx    ; save &string
mov eax, 3
mov ebx, 0
mov ecx, dword[ebp+8]  
mov edx, 1
int 0x80
mov eax, ecx; eax = &string_in + i
inc ecx     ; ecx = &string_in + i + 1
mov dword[ebp + 8], ecx ; &string = &string_in + i + 1
cmp byte[eax], 0xA  ;if ENTER
je __EndLerString
inc dword[ebp-4]  ; inc quantity that has been read
pop ecx
loopne __repeatReadString
__EndLerString:
mov eax, [ebp-4]    ; 
leave
ret 8