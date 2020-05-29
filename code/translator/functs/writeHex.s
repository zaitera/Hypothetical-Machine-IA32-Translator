EscreverHexa: ; Label in Portuguese due to teacher's requirements
enter 36,0
mov dword[ebp-36], 0
mov esi, 32         ; i = 8
mov eax, [ebp+8]    ; eax = &value
mov eax, [eax]      ; eax = *(&value)
__loopWriteHexa:
mov edx, 0          ; prepare edx
mov ecx, 16
div ecx             ; eax = value/16, edx = value%16
mov ebx, ebp
sub ebx, esi
mov dword[ebx], edx ; output[i] = value%16
inc dword[ebp-36]
cmp eax, 0
je __printHex
sub esi, 4          ; i--
cmp esi, 0          ; if i!=0, repeat
jne __loopWriteHexa

__printHex:
mov ecx, ebp
sub ecx, esi
mov ebx, [ecx]
add ebx, 30h
cmp dword[ecx], 9
jl __notAF
add ebx, 7
__notAF:
mov [ecx], ebx        
mov ebx, 1
mov eax, 4
mov edx, 1
int 80h
add esi, 4          ; i++
cmp esi, 33         ; if i != 8 repeat
jl __printHex
mov eax, dword[ebp-36]
leave               ; end EscreverHexa
ret
