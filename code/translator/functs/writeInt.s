EscreverInteiro:        ; Label in Portuguese due to teacher's requirements
enter 4,0
mov dword[ebp-4], 0     ; i = 0
mov ecx, dword[ebp + 8] ; ecx = &number
mov eax, [ecx]          ; eax = *(&number)
cmp eax, 0              ; if number[0] >= 0; check if is positive or negative number
jge __positiveWriteInt    
push eax                ; if negative, push number[o]
neg eax                
jo __printMinInt      ; jump if overflow
mov eax, 4
mov ebx, 1
mov dword[ecx], 0x2D    ; print "-"
mov edx, 1
int 0x80
inc dword[ebp-4]        ; i++
pop eax
mov [ecx], eax
neg eax

__positiveWriteInt:
mov ebx, 10
call __convertRecursiveInt
add eax, [ebp-4]
leave
ret 4

__convertRecursiveInt:
cdq             ; prepare to div 64bits
idiv ebx        ; eax = edx:eax/ebx, ebx start as 10
cmp eax, 0      ; if (eax/ebx) == 0
je __endWriteInt
add edx, 0x30   ; int2char(eax%ebx)
push edx        ; save the char
call __convertRecursiveInt
inc eax         ; eax = 2
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

__endWriteInt:
add edx, 0x30   ; int2char first digit
push edx        ; save
mov eax, 4      ; print first digit
mov ebx, 1
mov ecx, esp
mov edx, 1
int 0x80
mov eax, 1
pop edx         ; pop first digit
ret 0

__printMinInt:
pop eax
mov eax, 4
mov ebx, 1
mov ecx, __min_int
mov edx, 11
int 0x80
mov eax, 11
leave
ret 4