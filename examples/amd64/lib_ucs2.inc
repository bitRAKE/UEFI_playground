if used UINT64__Baseform_UCS2
UINT64__Baseform_UCS2: ; dynamic size numerical output ; 32 bytes :-)
; RAX : number to convert
; RDI : string buffer of length [4,130] bytes
; RCX : number base to use [2,36]
	push 0 ; use zero sentinel on stack to terminate
	lea rbx,[digit_table]
@@:	xor edx,edx
	div rcx
	test rax,rax
	movzx edx,byte [rbx+rdx]
	push rdx
	jnz @B
@@:	pop rax
	stosw ; UCS2 output
	test eax,eax
	jnz @B
	retn
; RAX : 0
; RBX : trashed
; RCX : unchanged
; RDX : byte 0x??
; RDI : end of null-terminated string
;
; buffer space needed for each base:
;	base	max bytes
;	2	130
;	10	42
;	16	34
;	36	28
;	n	2 * (ceiling[log_n(2^64)] + 1)
; https://www.wolframalpha.com/input/?i=2+*+ceiling+%5Blog_n+%282%5E64%29%5D%2B2
end if


if used PartitionedNibbles_UCS2
PartitionedNibbles_UCS2:
;  +8 : array of words to output
; RCX : [1-4+] word partitions to output in hexadecimal, high-word first
; RDI : UCS2 string buffer
	jmp @0F
@1:	mov al,'_'
	stosw
@0:	movzx edx,word [rsp+2*(rcx-1)+8]
	push rcx
	mov cl,4 shl 2
	call Nibbles_UCS2
	pop rcx
	loop @1B
	ret 8 ; larger word arrays need manual correction ;-)
end if


if used Nibbles_UCS2
Nibbles_UCS2: ; constant size numerical output
; RAX : used, byte ??
;  CL : (nibbles to output [1-16]) shl 2
; RDX : value to output, unchanged
	lea rbx,[digit_table]
.nibble:
;	shrx rax,rdx,rcx ; 60,56,52,..,0
;	and eax,$F
	shrd rax,rdx,cl
	shr rax,64-4			; mask and scale, just top nibble
	movzx eax,byte [rbx+rax]	; translate to ASCII hex
	stosw				; UCS2 output
	sub cl,4
	ja .nibble
	retn
end if


if used digit_table
USTR equ digit_table db '0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ'
end if


if used MultiCat_UCS2
MultiCat_UCS2: ; gather string array on stack at RDI
	pop rax
	pop rsi			; 1+ strings
@@:	movsw			; 1+ chars
	cmp word [rsi],0
	jnz @B
	pop rsi
	test rsi,rsi
	jnz @B
	jmp rax
end if

macro MULTICAT_UCS2 str_term:0
	local o
	pop rsi
o:	movsw
	cmp word [rsi],str_term
	jnz o
	pop rsi
	test rsi,rsi
	jnz o
end macro
