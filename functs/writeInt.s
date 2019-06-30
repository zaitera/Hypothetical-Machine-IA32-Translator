EscreverInteiro:
enter 4,0
mov dword[ebp-4], 0
mov ecx, dword[ebp + 8]
mov eax, [ecx]
cmp eax, 0
jge __positive_write
push eax
neg eax
jo __print_min_int
mov eax, 4
mov ebx, 1
mov dword[ecx], 0x2D
mov edx, 1
int 0x80
inc dword[ebp-4]
pop eax
mov [ecx], eax
neg eax

__positive_write:
mov ebx, 10
call __print_recursive_int
add eax, [ebp-4]
leave
ret 4

__print_recursive_int:
cdq
idiv ebx
cmp eax, 0
je __end_write
add edx, 0x30
push edx
call __print_recursive_int
inc eax
push eax
mov eax, 4
mov ebx, 1
mov ecx, esp
add ecx, 4
mov edx, 1
int 0x80
pop eax
pop edx
ret 0

__end_write:
add edx, 0x30
push edx
mov eax, 4
mov ebx, 1
mov ecx, esp
mov edx, 1
int 0x80
mov eax, 1
pop edx
ret 0

__print_min_int:
pop eax
mov eax, 4
mov ebx, 1
mov ecx, __min_int
mov edx, 11
int 0x80
mov eax, 11
leave
ret 4