LerInteiro:
enter 4,0
mov dword[ebp-4], 1 ; count = 1

mov eax, 3
mov ebx, 0
mov ecx, dword[ebp+8]; label
mov edx, 1
int 0x80

cmp byte[ecx], 0x2D ; if label[0] == '-'
je __negative_read  ; goto
push 0              ; flag = 0 if positive
mov eax, dword[ecx] 
sub eax, 0x30       ; eax = char2int(label[0])

__positive_read:
call __digit  
push eax
mov ecx, 11

__loop_read:
mov eax, 3
push ecx
mov ecx, dword[ebp + 8] ; label
int 0x80
cmp byte[ecx], 0xA  ; if label[] == ENTER
je __end_read       ; goto
mov eax, dword[ecx] ; 
sub eax, 0x30       ; eax = char2int (label[])
call __digit    
mov dword[ecx], eax ; label = char2int() 
mov eax, [esp + 4]
sal eax, 3
add eax, [esp+4]
add eax, [esp+4]
add eax, [ecx]      ; eax = count*10 + label
pop ecx
mov [esp], eax
inc dword[ebp-4] ; count++;
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
push 1              ; flag = 0 if negative
mov eax, 0
jmp __positive_read

__digit:
cmp eax, 9
jg __end_read;__print_err_int_read
cmp eax, 0
jl __end_read;__print_err_int_read
ret 0
