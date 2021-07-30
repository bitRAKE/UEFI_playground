include 'format/format.inc'		; object file structure support
include 'encoding/utf8_to_ucs2.inc'	; utf8 -> UCS-2 support

; fasmg language extensions:
include 'macro/xcalm.inc'
include 'macro/struct.inc'
include 'macro/proc64.inc'
include 'macro/@@.inc'

format pe64 efi on 'nul'

; readonly section supports simple string list
macro _U register,quoted
	USTR equ quoted
	repeat 1,k:0+quoted
		lea register,[_U#k]
	end repeat
end macro

postpone
	; output constant data
	section '.rdata' data readable
	irpv U,USTR ; string constant based on simple string value to de-duplicate
		match X O,U
			; don't make any assumptions about complex strings
			; just execute constant definition:
			U
		else
		repeat 1,k:0+U
			if defined _U#k & _U#k < $
			else
				_U#k du U,0
			end if
		end repeat
		end match
	end irpv

	data fixups
	end data
	namespace PE
		; fixups are DATA_DIRECTORY 5
		load DATA_BYTES:dword from PE:RvaAndSizes.Size + 5*RvaAndSizes.ENTRY_LENGTH
		if DATA_BYTES = 0
			; clear RVA if length is zero
			store 0:dword at PE:RvaAndSizes.Rva + 5*RvaAndSizes.ENTRY_LENGTH
			display 10,'fyi: relocation data was not needed'
		end if
	end namespace
end postpone



; machine (register) size type abstractions:

iterate T, PTR,INTN,UINTN,EFI_HANDLE,EFI_EVENT
	calminstruction T values&
		asmcmd =ALIGN 8
		asmcmd =DQ values
	end calminstruction
	calminstruction(name) T values&
		asmcmd =ALIGN 8
		asmcmd name =DQ values
	end calminstruction
end iterate

EFIERR := 1 shl 63

include 'UEFI 2.9.inc'
;include 'UEFI Shell 2.2.inc'
