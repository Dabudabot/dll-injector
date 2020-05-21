IFDEF RAX
;x64

.code
func_mul proc FRAME
sub     rsp, 28h
.ALLOCSTACK 28h
.ENDPROLOG
mov     rax, rcx
mul     rdx
add     rsp, 28h
ret
func_mul endp
PUBLIC func_mul

ELSE
;x86
.386
.MODEL FLAT
.code

func_mul proc
mov     eax, ecx
mul     edx
ret
func_mul endp

ALIAS <@func_mul@8> = <func_mul>

PUBLIC func_mul


ENDIF
END