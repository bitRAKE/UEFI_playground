include 'UEFI64_app.inc'

; Boot Services Memory Functions:
;	AllocatePages		PTR ? ; EFI_STATUS (EFI_ALLOCATE_TYPE,EFI_MEMORY_TYPE,UINTN,*EFI_PHYSICAL_ADDRESS)
;	FreePages		PTR ? ; EFI_STATUS (EFI_PHYSICAL_ADDRESS,UINTN)
;	GetMemoryMap		PTR ? ; EFI_STATUS (*UINTN,*EFI_MEMORY_DESCRIPTOR,*UINTN,*UINTN,*UINT32)
;	AllocatePool		PTR ? ; EFI_STATUS (EFI_MEMORY_TYPE,UINTN,**)
;	FreePool		PTR ? ; EFI_STATUS (*)
;	CopyMem			PTR ?
;	SetMem			PTR ?
;
; Runtime Services Memory Functions:
;	SetVirtualAddressMap	PTR ? ;
;	ConvertPointer		PTR ? ;



section '.text' executable readable
include 'lib_ucs2.inc'

start:	entry $
	push rbp rbx rsi rdi
	virtual at RSP
				rq 4 ; shadow space
		.P5		dq ?

	; local variable space:
		.image_handle	dq ? ; EFI_HANDLE
		.system_table	dq ? ; EFI_SYSTEM_TABLE

		.buff		dq ?	; EFI_PHYSICAL_ADDRESS (*EFI_MEMORY_DESCRIPTOR array)
		.buff_pages	dq ?	; UINTN, buffer pages
		.extra_pages	dd ?
		.vdesc		dd ?	; UINT32, version EFI_MEMORY_DESCRIPTOR (1)
		.idesc		dq ?	; UINTN, sizeof EFI_MEMORY_DESCRIPTOR
		.imap		dq ?	; UINTN, buffer bytes
		.kmap		dq ?	; UINTN, map delta key

		.dcount		dd ?	; descriptor array size
		rb ($10 - ((8 + $ - $$) and $F)) and $F ; 8 mod 16
		.FRAME := $ - $$
	end virtual
	sub rsp,.FRAME

	mov [.image_handle],rcx
	mov [.system_table],rdx
	mov rbp,[rdx + EFI_SYSTEM_TABLE.BootServices]

	and [.imap],0
	and [.buff],0
	and [.buff_pages],0
	mov [.extra_pages],1		; accellerate settling
	jmp .no_buffer

; Don't make any assumptions about memory allocations, loop until there is
; sufficient space allocated. It is possible that allocations increase the
; buffer size needed, but this should settle because pages are larger than
; EFI_MEMORY_DESCRIPTOR size (also because the allocator probably coalesces
; regions - not required).

.expand_buffer:

	; free existing buffer if it exists:

	mov rcx,[.buff]			; EFI_PHYSICAL_ADDRESS
	jrcxz @F
	mov edx,dword[.buff_pages]	; UINTN
	call [rbp + EFI_BOOT_SERVICES.FreePages]
	and [.buff],0
@@:
	; determine new buffer size needed:

	mov r8,[.imap]			; only parameter relied on from GetMemoryMap
	add r8,4096 - 1
	shr r8,12
	mov rax,EFI_INVALID_PARAMETER
	jz .efierr_forward		; this should never happen
	add r8d,[.extra_pages]

	; don't keep doing the same thing if it failed last time!
	cmp [.buff_pages],r8
	jc @F
	inc [.extra_pages]
	inc r8
@@:	mov [.buff_pages],r8

	mov ecx,AllocateAnyPages	; EFI_ALLOCATE_TYPE
	mov edx,EfiLoaderData		; EFI_MEMORY_TYPE
;	mov r8,[.buff_pages]		; UINTN 4K pages
	lea r9,[.buff]			;*EFI_PHYSICAL_ADDRESS
	call [rbp+EFI_BOOT_SERVICES.AllocatePages]
	xchg rcx,rax
	jrcxz .good_buffer		; EFI_SUCCESS

	; only fails when out of memory?

.efierr_forward_RCX:
	xchg rcx,rax
.efierr_forward:
	add rsp,.FRAME
	pop rdi rsi rbx rbp
	retn


.good_buffer:
	imul rax,[.buff_pages],4096
	mov [.imap],rax			; update map size buffer based on allocated pages
.no_buffer:
	lea rcx,[.imap]			;*UINTN
	mov rdx,[.buff]			;*EFI_MEMORY_DESCRIPTOR array
	lea r8,[.kmap]			;*UINTN key
	lea r9,[.idesc]			;*UINTN single EFI_MEMORY_DESCRIPTOR size in bytes
	lea rax,[.vdesc]
	mov [.P5],rax			;*UINT32 EFI_MEMORY_DESCRIPTOR version (1)
	call [rbp + EFI_BOOT_SERVICES.GetMemoryMap]
	xchg rcx,rax
	jrcxz .good_map			; EFI_SUCCESS?
	cmp ecx,(EFI_BUFFER_TOO_SMALL - EFIERR) ; assume high dword is $80..
	jnz .efierr_forward_RCX
	jmp .expand_buffer
.good_map:
	mov rbp,[.system_table]
	mov r15,[.buff]			;*EFI_MEMORY_DESCRIPTOR array

	mov rax,[.imap]
	cqo
	div [.idesc]
	mov [.dcount],eax
	test rdx,rdx
	jz .map_valid
	and [.dcount],0
.map_valid:
	add [.imap],r15			; end address after array

	lea rdx,[_header]
	jmp .start_descriptor
.next_descriptor:
	push 0				; terminator for MultiCat_UCS2
	lea rax,[_eol]
	push rax

	lea rdi,[_buildup]
	mov rcx,[r15 + EFI_MEMORY_DESCRIPTOR.Attribute]
	push rdi
	call MemAttribute_toString_UCS2

	push rdi
	mov eax,' '
	stosw
	push 2
	pop rcx
	push [r15 + EFI_MEMORY_DESCRIPTOR.NumberOfPages]
	call PartitionedNibbles_UCS2
	mov eax,' '
	stosd

	_U rax,'(unknown)'
	cmp [r15 + EFI_MEMORY_DESCRIPTOR.Type],sizeof MemType_Table
	jnc .type_unknown
;	MemType_Table.Select rax,[rbx + EFI_MEMORY_DESCRIPTOR.Type]
	lea rdx,[MemType_Table]
	imul eax,[r15 + EFI_MEMORY_DESCRIPTOR.Type],MemType_Table.MAX
	add rax,rdx
.type_unknown:
	push rax

	push rdi
	push 4
	pop rcx
	push [r15 + EFI_MEMORY_DESCRIPTOR.PhysicalStart]
	call PartitionedNibbles_UCS2
	mov eax,' '
	stosd

	lea rdi,[_line_buffer]
	call MultiCat_UCS2
	xchg eax,esi
	stosd ; terminate

	lea rdx,[_line_buffer]
.start_descriptor:
	mov rcx,[rbp + EFI_SYSTEM_TABLE.ConOut]
	call [rcx + EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL.OutputString]

;include 'protocol/simple_text_input.inc'
; check if (output rows) MOD (text mode rows - 1) = 0
;	call ConIn__WaitForKey
; display header again, and continue

	add r15,[.idesc]
	cmp r15,[.imap]
	jc .next_descriptor


; we embrace some laziness:
macro __PRINT line&
	local base
	base = 10 ; default base
	lea rdi,[_line_buffer]
	iterate L,line
		match =DWORD? V,L
			mov eax,V
			push base
			pop rcx
			call UINT64__Baseform_UCS2
			sub rdi,2
		else match =QWORD? V,L
			mov rax,V
			push base
			pop rcx
			call UINT64__Baseform_UCS2
			sub rdi,2
		else match =BASE? V,L
			base = V
		else ; < text >
			_UX rsi,L
		@@:	movsw
			cmp word [rsi],0
			jnz @B
		end match
	end iterate
	xor eax,eax
	stosw

	mov rcx,[rbp + EFI_SYSTEM_TABLE.ConOut]
	lea rdx,[_line_buffer]
	call [rcx + EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL.OutputString]
end macro

; char(9) is not tab?

__PRINT <13,10,'EFI_MEMORY_DESCRIPTOR(s):',13,10,\
	'    version: '>,DWORD [.vdesc],<13,10,\
	'    size: '>,QWORD [.idesc],<' bytes',13,10,\
	'    count: '>,DWORD [.dcount],<13,10>


	mov rax,[rbp + EFI_SYSTEM_TABLE.BootServices]
	mov rcx,[.buff]			; EFI_PHYSICAL_ADDRESS
	mov edx,dword[.buff_pages]	; UINTN
	call [rax + EFI_BOOT_SERVICES.FreePages]

	xor eax,eax
	jmp .efierr_forward



; RCX : attribute flags
; RDI : string to update
MemAttribute_toString_UCS2:
	push 0
	_U rbx,'|'
	lea rdx,[MemAttribute_Table]
	bsr rax,rcx
	jnz .last_flag
	_U rax,'(none)'
	jmp .done
.another_flag:
	push rbx	; |
.last_flag:
	btr rcx,rax	; clear flag
	imul eax,eax,MemAttribute_Table.MAX
	add rax,rdx
.done:
	push rax
	bsr rax,rcx
	jnz .another_flag
	call MultiCat_UCS2
	xchg eax,esi
	stosd ; terminate
	retn


USTR equ _header du \
	'Address:            Type:                   Pages:    Attributes:',13,10,0
;	 0000_0000_0000_0000 MemoryMappedIOPortSpace 0000_0000 UC|WC|WT|SP|RO|RT|XP|WB|MR
USTR equ _eol	du 13,10,0

MemType_Table CONST_Table ' ',\; <-- fill char
	'ReservedMemoryType',\
	'LoaderCode',\
	'LoaderData',\
	'BootServicesCode',\
	'BootServicesData',\
	'RuntimeServicesCode',\
	'RuntimeServicesData',\
	'ConventionalMemory',\
	'UnusableMemory',\
	'ACPIReclaimMemory',\
	'ACPIMemoryNVS',\
	'MemoryMappedIO',\
	'MemoryMappedIOPortSpace',\
	'PalCode'


MemAttribute_Table CONST_Table 0,\; <-- fill char
	'UC',		'WC',		'WT',		'WB',\
	'UCE',		'05',		'06',		'07',\
	'08',		'09',		'0A',		'0B',\
	'WP',		'RP',		'XP',		'NV',\
\
	'MR',		'RO',		'SP',		'crypto',\
	'14',		'15',		'16',		'17',\
	'18',		'19',		'1A',		'1B',\
	'1C',		'1D',		'1E',		'1F',\
\
	'20',		'21',		'22',		'23',\
	'24',		'25',		'26',		'27',\
	'28',		'29',		'2A',		'2B',\
	'2C',		'2D',		'2E',		'2F',\
\
	'30',		'31',		'32',		'33',\
	'34',		'35',		'36',		'37',\
	'38',		'39',		'3A',		'3B',\
	'3C',		'3D',		'3E',		'RT'


section '.data' data readable writeable

align 64
label _buildup:256
rw sizeof _buildup

label _line_buffer:128
rw sizeof _line_buffer
