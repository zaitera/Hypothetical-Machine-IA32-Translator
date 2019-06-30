LerInteiro:
enter 4,0
mov dword[ebp-4], 1
mov ebx, 0
mov edx, 1
mov ecx, dword[ebp+8]
mov eax, 3
int 0x80
cmp byte[ecx], 0x2D
je __negative_read
push 0
mov eax, dword[ecx]
sub eax, 0x30
__positive_read:
call __digit
push eax
mov ecx, 11
__loop_read:
mov eax, 3
push ecx
mov ecx, dword[ebp + 8]
int 0x80
cmp byte[ecx], 0xA
je __end_read
mov eax, dword[ecx]
sub eax, 0x30
call __digit
mov dword[ecx], eax
mov eax, [esp + 4]
sal eax, 3
add eax, [esp+4]
add eax, [esp+4]
add eax, [ecx]
pop ecx
mov [esp], eax
inc dword[ebp-4]
loop __loop_read
__end_read:
pop eax
pop eax
pop ecx
cmp ecx, 0
je __no_signal
neg eax
__no_signal:
mov ecx, [ebp + 8]
mov [ecx], eax
mov eax, [ebp -4]
leave
ret 4
__negative_read:
push 1
mov eax, 0
jmp __positive_read
__digit:
cmp eax, 9
jg __print_err_int_read
cmp eax, 0
jl __print_err_int_read
ret 0