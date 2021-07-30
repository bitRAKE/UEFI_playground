; based on specification 2.9, 2021 03 18

struct EFI_TABLE_HEADER
	Signature	dq ?
	Revision	dd ?
	HeaderSize	dd ?
	CRC32		dd ?
	Reserved	dd ?
ends

include 'table\system.inc'
include 'table\boot_services.inc'
include 'table\runtime_services.inc'
;include 'table\configuration.inc'


include 'protocol\simple_text_input.inc'
include 'protocol\simple_text_output.inc'

;include 'protocol\graphics_output.inc'
;include 'protocol\simple_pointer.inc'
;include 'protocol\absolute_pointer.inc'


EFI_SUCCESS := 0
iterate I,EFI_LOAD_ERROR,EFI_INVALID_PARAMETER,EFI_UNSUPPORTED,EFI_BAD_BUFFER_SIZE,\
	EFI_BUFFER_TOO_SMALL,EFI_NOT_READY,EFI_DEVICE_ERROR,EFI_WRITE_PROTECTED,\
	EFI_OUT_OF_RESOURCES,EFI_VOLUME_CORRUPTED,EFI_VOLUME_FULL,EFI_NO_MEDIA,\
	EFI_MEDIA_CHANGED,EFI_NOT_FOUND,EFI_ACCESS_DENIED,EFI_NO_RESPONSE,\
	EFI_NO_MAPPING,EFI_TIMEOUT,EFI_NOT_STARTED,EFI_ALREADY_STARTED,\
	EFI_ABORTED,EFI_ICMP_ERROR,EFI_TFTP_ERROR,EFI_PROTOCOL_ERROR,\
	EFI_INCOMPATIBLE_VERSION,EFI_SECURITY_VIOLATION,EFI_CRC_ERROR,EFI_END_OF_MEDIA,\
	EFI_ERR_29,EFI_ERR_30,EFI_END_OF_FILE,EFI_INVALID_LANGUAGE,\
	EFI_COMPROMISED_DATA,EFI_ERR_34,EFI_HTTP_ERROR
	I := EFIERR or %
end iterate
iterate I,EFI_WARN_UNKNOWN_GLYPH,EFI_WARN_DELETE_FAILURE,EFI_WARN_WRITE_FAILURE,EFI_WARN_BUFFER_TOO_SMALL,\
	EFI_WARN_STALE_DATA,EFI_WARN_FILE_SYSTEM
	I := %
end iterate

;	for EFI_SPECIFICATION_VERSION <= EFI_2_00_SYSTEM_TABLE_REVISION :
;define EFI_NOT_AVAILABLE_YET   EFIERR (28)
;define EFI_UNLOAD_IMAGE        EFIERR (29)



; network errors?
iterate I,EFI_NETWORK_UNREACHABLE,EFI_HOST_UNREACHABLE,EFI_PROTOCOL_UNREACHABLE,EFI_PORT_UNREACHABLE,\
	EFI_CONNECTION_FIN,EFI_CONNECTION_RESET,EFI_CONNECTION_REFUSED
	I := EFIERR or (99 + %)
end iterate



EFI_SMBIOSERR := $30000
iterate I,EFI_SMBIOSERR_FAILURE,EFI_SMBIOSERR_STRUCT_NOT_FOUND,EFI_SMBIOSERR_TYPE_UNKNOWN,EFI_SMBIOSERR_UNSUPPORTED
	I := EFIERR or EFI_SMBIOSERR or %
end iterate





struc EFI_GUID line&
	align 16
	label .:16
	match { AAAA=, BB=, CC=, { D=, E=, F=, G=, H=, I=, J=, K } },line
		dd AAAA
		dw BB,CC
		db D,E,F,G,H,I,J,K
	else match ,line ; blank form reserves space for GUID
		rb 16
	else
		err 'expected UEFI spec form of GUID, not:',10,9,`line,10
	end match
end struc
