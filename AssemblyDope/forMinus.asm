IFDEF RAX
;x64

.code
func_minus proc FRAME
sub     rsp, 28h
.ALLOCSTACK 28h
.ENDPROLOG
sub     rcx, rdx
mov     rax, rcx
add     rsp, 28h
ret
func_minus endp
PUBLIC func_minus

ELSE
;x86
.386
.MODEL FLAT
.code

func_minus proc
sub     ecx, edx
mov     eax, ecx
ret
func_minus endp

func_mul proc
mov     eax, [esp+4]
mov     ecx, [esp+8]
mul     ecx
ret 8
func_mul endp

ALIAS <@func_minus@8> = <func_minus>
ALIAS <@func_mul@8> = <func_mul>

PUBLIC func_minus
PUBLIC func_mul

ENDIF
END