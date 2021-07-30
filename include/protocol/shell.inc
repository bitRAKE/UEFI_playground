; https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Protocol/Shell.h
define EFI_SHELL_PROTOCOL_GUID \
{0x6302D008,0x7F9B,0x4F30,{0x87,0xAC,0x60,0xC9,0xFE,0xF5,0xDA,0x4E}}

struct EFI_SHELL_PROTOCOL
	Execute			PTR ?	; EFI_STATUS (*EFI_HANDLE,?*CHAR16,?**CHAR16,?*EFI_STATUS)
	GetEnv			PTR ?	; *CHAR16 (?*CHAR16)
	SetEnv			PTR ?	; EFI_STATUS (*CHAR16,*CHAR16,BOOLEAN)
	GetAlias		PTR ?	; *CHAR16 (*CHAR16,*BOOLEAN)
	SetAlias		PTR ?	; EFI_STATUS (*CHAR16,?*CHAR16,BOOLEAN,BOOLEAN)
	GetHelpText		PTR ?	; EFI_STATUS (*CHAR16,?*CHAR16,**CHAR16)
	GetDevicePathFromMap	PTR ?	; *EFI_DEVICE_PATH_PROTOCOL (*CHAR16)
	GetMapFromDevicePath	PTR ?	; *CHAR16 (**EFI_DEVICE_PATH_PROTOCOL)
	GetDevicePathFromFilePath PTR ?	; *EFI_DEVICE_PATH_PROTOCOL (*CHAR16)
	GetFilePathFromDevicePath PTR ?	; *CHAR16 (*EFI_DEVICE_PATH_PROTOCOL)
	SetMap			PTR ?	; EFI_STATUS (?*EFI_DEVICE_PATH_PROTOCOL,*CHAR16)
	GetCurDir		PTR ?	; *CHAR16 (?*CHAR16)
	SetCurDir		PTR ?	; EFI_STATUS (?*CHAR16,*CHAR16)
	OpenFileList		PTR ?	; EFI_STATUS (*CHAR16,UINT64,**EFI_SHELL_FILE_INFO)
	FreeFileList		PTR ?	; EFI_STATUS (**EFI_SHELL_FILE_INFO)
	RemoveDupInFileList	PTR ?	; EFI_STATUS (**EFI_SHELL_FILE_INFO)
	BatchIsActive		PTR ?	; BOOLEAN
	IsRootShell		PTR ?	; BOOLEAN
	EnablePageBreak		PTR ?	; -
	DisablePageBreak	PTR ?	; -
	GetPageBreak		PTR ?	; BOOLEAN
	GetDeviceName		PTR ?	; EFI_STATUS (EFI_HANDLE,EFI_SHELL_DEVICE_NAME_FLAGS,*CHAR8,**CHAR16)
	GetFileInfo		PTR ?	; *EFI_FILE_INFO (SHELL_FILE_HANDLE)
	SetFileInfo		PTR ?	; EFI_STATUS (SHELL_FILE_HANDLE,*EFI_FILE_INFO)
	OpenFileByName		PTR ?	; EFI_STATUS (*CHAR16,*SHELL_FILE_HANDLE,UINT64)
	CloseFile		PTR ?	; EFI_STATUS (SHELL_FILE_HANDLE)
	CreateFile		PTR ?	; EFI_STATUS (*CHAR16,UINT64,*SHELL_FILE_HANDLE)
	ReadFile		PTR ?	; EFI_STATUS (SHELL_FILE_HANDLE,*UINT64,*VOID)
	WriteFile		PTR ?	; EFI_STATUS (SHELL_FILE_HANDLE,*UINT64,*VOID)
	DeleteFile		PTR ?	; EFI_STATUS (SHELL_FILE_HANDLE)
	DeleteFileByName	PTR ?	; EFI_STATUS (*CHAR16)
	GetFilePosition		PTR ?	; EFI_STATUS (SHELL_FILE_HANDLE,*UINT64)
	SetFilePosition		PTR ?	; EFI_STATUS (SHELL_FILE_HANDLE,UINT64)
	FlushFile		PTR ?	; EFI_STATUS (SHELL_FILE_HANDLE)
	FindFiles		PTR ?	; EFI_STATUS (*CHAR16,**EFI_SHELL_FILE_INFO)
	FindFilesInDir		PTR ?	; EFI_STATUS (SHELL_FILE_HANDLE,**EFI_SHELL_FILE_INFO)
	GetFileSize		PTR ?	; EFI_STATUS (SHELL_FILE_HANDLE,*UINT64)
	OpenRoot		PTR ?	; EFI_STATUS (*EFI_DEVICE_PATH_PROTOCOL,*SHELL_FILE_HANDLE)
	OpenRootByHandle	PTR ?	; EFI_STATUS (EFI_HANDLE,*SHELL_FILE_HANDLE)
	ExecutionBreak		EFI_EVENT
	MajorVersion		dd ? ; UINT32
	MinorVersion		dd ? ; UINT32
; Version 2.1+
	RegisterGuidName	PTR ?	; EFI_STATUS (*EFI_GUID,*CHAR16)
	GetGuidName		PTR ?	; EFI_STATUS (*EFI_GUID,**CHAR16)
	GetGuidFromName		PTR ?	; EFI_STATUS (*CHAR16,*EFI_GUID)
	GetEnvEx		PTR ?	; *CHAR16 (?*CHAR16,?*UINT32)
ends

; EFI_DEVICE_NAME_FLAGS
EFI_DEVICE_NAME_USE_COMPONENT_NAME := 0x00000001
EFI_DEVICE_NAME_USE_DEVICE_PATH := 0x00000002

struct EFI_LIST_ENTRY
	Flink	dq ?
	Blink	dq ?
ends

struct EFI_SHELL_FILE_INFO
	Link		EFI_LIST_ENTRY
	Status		dq ? ; EFI_STATUS
	FullName	dq ? ; *CHAR16
	FileName	dq ? ; *CHAR16
	Handle		dq ? ; SHELL_FILE_HANDLE
	Info		dq ? ; *EFI_FILE_INFO
ends




; UEFI Shell applications should only include this file, and non-UEFI spec support.

; Required UEFI support:
;	EFI_STATUS
;	EFI_HANDLE
;	EFI_DEVICE_PATH_PROTOCOL
;	EFI_FILE_PROTOCOL
;	EFI_EVENT
;	EFI_GUID 
include '../uefi.inc'



iterate I,EFI_REDIRECTION_SAME,EFI_REDIRECTION_NOT_ALLOWED
	I := EFIERR or (1000 + %)
end iterate


; SHELL_STATUS (returned by applications for )
SHELL_SUCCESS			:= 0	; The operation completed successfully
SHELL_LOAD_ERROR		:= 1	; The image failed to load.
SHELL_INVALID_PARAMETER		:= 2	; There was an error in the command-line options.
SHELL_UNSUPPORTED		:= 3	; The operation is not supported.
SHELL_BAD_BUFFER_SIZE		:= 4	; The buffer was not the proper size for the request.
SHELL_BUFFER_TOO_SMALL		:= 5	; The buffer is not large enough to hold the requested data. The required buffer size is returned in the appropriate parameter when this error occurs.
SHELL_NOT_READY			:= 6	; There is no data pending upon return.
SHELL_DEVICE_ERROR		:= 7	; The physical device reported an error while attempting the operation.
SHELL_WRITE_PROTECTED		:= 8	; The device cannot be written to.
SHELL_OUT_OF_RESOURCES		:= 9	; A resource has run out.
SHELL_VOLUME_CORRUPTED		:= 10	; An inconstancy was detected on the file system causing the operating to fail.
SHELL_VOLUME_FULL		:= 11	; There is no more space on the file system.
SHELL_NO_MEDIA			:= 12	; The device does not contain any medium to perform the operation.
SHELL_MEDIA_CHANGED		:= 13	; The medium in the device has changed since the last access.
SHELL_NOT_FOUND			:= 14	; The item was not found.
SHELL_ACCESS_DENIED		:= 15	; Access was denied.
SHELL_TIMEOUT			:= 18	; The timeout time expired.
SHELL_NOT_STARTED		:= 19	; The specified operation could not be started.
SHELL_ALREADY_STARTED		:= 20	; The specified operation had already started.
SHELL_ABORTED			:= 21	; The operation was aborted by the user
SHELL_INCOMPATIBLE_VERSION	:= 25	; The function encountered an internal version that was incompatible with a version requested by the caller.
SHELL_SECURITY_VIOLATION	:= 26	; The function was not performed due to a security violation.
SHELL_NOT_EQUAL			:= 27	; The function was performed and resulted in an unequal comparison..


define EFI_SHELL_PARAMETERS_PROTOCOL_GUID { 0x752F3136, 0x4E16, 0x4FDC, { 0xA2, 0x2A, 0xE5, 0xF4, 0x68, 0x12, 0xF4, 0xCA } }

virtual at 0
EFI_SHELL_PARAMETERS_PROTOCOL:
.Argv	dq ? ; **CHAR16
.Argc	dq ? ; UINTN
.StdIn	dq ? ; SHELL_FILE_HANDLE
.StdOut	dq ? ; SHELL_FILE_HANDLE
.StdErr	dq ? ; SHELL_FILE_HANDLE
end virtual


define EFI_SHELL_DYNAMIC_COMMAND_PROTOCOL_GUID { 0x3C7200E9, 0x5F, 0x4EA4, { 0x87, 0xDE, 0xA3, 0xDF, 0xAC, 0x8A, 0x27, 0xC3 } }

virtual at 0
EFI_SHELL_DYNAMIC_COMMAND_PROTOCOL:
.CommandName	dq ? ; *CHAR16
.Handler	dq ? ; EFI_STATUS (*this,*EFI_SYSTEM_TABLE,*EFI_SHELL_PARAMETERS_PROTOCOL,*EFI_SHELL_PROTOCOL)
.GetHelp	dq ? ; *CHAR16 (*this,*CHAR8)
end virtual


