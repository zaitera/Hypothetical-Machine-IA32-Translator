LerString: ; Label in Portuguese due to teacher's requirements
enter 4,0
mov dword[ebp-4], 0 ; save 0
;;Prepare to input;;
mov ebx, 0
mov edx, 1
;;End prepare;;
mov ecx, dword[ebp + 12]   ;&string
__repeatReadString:
mov eax, 3
push ecx    ; save &string
mov ecx, dword[ebp+8]  
int 0x80
mov eax, ecx; eax = &string_in + i
inc ecx     ; eax = &string_in + i + 1
inc dword[ebp-4]  ; inc quantity that has been read
cmp byte[eax], 0xA  ;if ENTER
mov dword[ebp + 8], ecx ; &string = &string_in + i + 1
pop ecx
loopne __repeatReadString
mov eax, [ebp-4]
leave
ret 8