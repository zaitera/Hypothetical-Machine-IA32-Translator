LerInteiro: ; Label in Portuguese due to teacher's requirements
enter 4,0
mov dword[ebp-4], 1 ; flag

mov eax, 3
mov ebx, 0
mov ecx, dword[ebp+8]; &Label
mov edx, 1
int 0x80

cmp byte[ecx], 0x2D ; if label[0] == '-'
je __negativeReadInt  
push 0              ; value = 0 save in ebp-4
mov eax, dword[ecx] ; eax = *(&Label)
sub eax, 0x30       ; eax = char2int(Label)

__positiveReadInt:
call __digit        ; check if is 0~9 digit
push eax            ; save first digit in ebp-8
mov ecx, 11         ; i = 11

__loopReadInt:
mov eax, 3
push ecx            ; push i
mov ecx, dword[ebp + 8] ; ecx = &Label
int 0x80
cmp byte[ecx], 0xA  ; if Label == ENTER
je __endReadInt       
mov eax, dword[ecx] ; eax = *(&Label) 
sub eax, 0x30       ; eax = char2int (label[])
call __digit        ; check if is 0~9 digit
mov dword[ecx], eax ; new = char2int() 
mov eax, [esp + 4]  ; eax = value, first time is 0->pos, 1->neg
sal eax, 3
add eax, [esp+4]
add eax, [esp+4]
add eax, [ecx]      ; eax = value*10 + new
pop ecx             ; ecx = i
mov [esp], eax      ;
inc dword[ebp-4]    ; flag--
loop __loopReadInt    ; while (i>0)

__endReadInt:
pop eax
pop eax
pop ecx
cmp ecx, 0
je __noSignalInt
neg eax

__noSignalInt:
mov ecx, [ebp + 8]
mov [ecx], eax
mov eax, [ebp -4]
leave
ret 4

__negativeReadInt:
push 1              ; save first digit in ebp-4
mov eax, 0
jmp __positiveReadInt

__digit:
cmp eax, 9
jg __endReadInt;__print_err_int_read
cmp eax, 0
jl __endReadInt;__print_err_int_read
ret 0
