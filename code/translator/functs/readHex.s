LerHexa: ; Label in Portuguese due to teacher's requirements
enter 8, 0
mov eax, dword[ebp+8]
mov dword[eax], 0
mov dword[ebp-8], 0
mov dword[ebp-4], 8d
jmp __initLerHex

__isEnter:
cmp byte[ecx], 0xA  ; if = 'ENTER'
je __endLerHexa 
ret

__isxdigit:
cmp ebx, 0  ; if < '0'
jl __endErrorHexa
cmp ebx, 9  ; if > '9'
jg __isAF
ret

__isAF:
sub ebx, 7
cmp ebx, 10  ; if < 'A'(10)
jl __endErrorHexa
cmp ebx, 15  ; if > 'F'(15)
jg __ifaf
ret

__ifaf:
sub ebx, 32
cmp ebx, 10  ; if < 'a'(10)
jl __endErrorHexa
cmp ebx, 15  ; if > 'a'(15)
jg __endErrorHexa
ret

__loopReadHex:
mov [ebp-4], ecx
__initLerHex:
mov eax, 3
mov ebx, 0
mov ecx, esp; label
mov edx, 1
int 0x80
call __isEnter
mov ebx, dword[ecx]
sub ebx, 30h
call __isxdigit
mov ecx, [ebp+8]        ; ecx = (&value)
mov eax, [ecx]          ; value' = eax = *(&value)
sal eax, 4              ; value' = value'*16
add eax, ebx            ; value'*16 + new_digit
mov dword[ecx], eax     ; *(&value) = value'*16 + new_digit
mov ecx, [ebp-4]        ; ecx = count
loop __loopReadHex

__endLerHexa:
mov eax, 8
sub eax, [ebp-4]
leave
ret

__endErrorHexa:
mov ecx, [ebp+8]        ; eax = (&value)
mov dword[ecx], 0       ; eax = *(&value)
mov eax, 0
jmp __endLerHexa