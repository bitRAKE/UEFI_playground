
macro local_include? instr
	local pos,chr,path
	pos = lengthof __FILE__
	while pos
		chr = (__FILE__ shr (8*(pos-1))) and 0FFh
		if chr = '/' | chr = '\'
			break
		end if
		pos = pos - 1
	end while
	path = string __FILE__ and not ( (-1) shl (8*pos) )
	macro instr file
		include string path + file shl (8*lengthof path)
	end macro
end macro

local_include format?.include

macro format?.PE? settings
	PE.Settings.Characteristics = IMAGE_FILE_EXECUTABLE_IMAGE or IMAGE_FILE_32BIT_MACHINE or IMAGE_FILE_LINE_NUMS_STRIPPED or IMAGE_FILE_LOCAL_SYMS_STRIPPED
	PE.Settings.DllCharacteristics = 0
	PE.Settings.Subsystem = IMAGE_SUBSYSTEM_EFI_APPLICATION
	local seq
	define seq settings:
	while 1
		match :, seq
			break
		else match =large? more, seq
			PE.Settings.Characteristics = PE.Settings.Characteristics or IMAGE_FILE_LARGE_ADDRESS_AWARE
			redefine seq more
		else match =at? base =on? stub :, seq
			PE.Settings.ImageBase = base
			PE.Settings.Stub = stub
			break
		else match =at? base :, seq
			PE.Settings.ImageBase = base
			break
		else match =on? stub :, seq
			PE.Settings.Stub = stub
			break
		else
			match =EFI? more, seq
				PE.Settings.Magic = 0x20B
				PE.Settings.Subsystem = IMAGE_SUBSYSTEM_EFI_APPLICATION
				redefine seq more
			else match =EFIboot? more, seq
				PE.Settings.Magic = 0x20B
				PE.Settings.Subsystem = IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER
				redefine seq more
			else match =EFIruntime? more, seq
				PE.Settings.Magic = 0x20B
				PE.Settings.Subsystem = IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER
				redefine seq more
			else
				err 'invalid argument'
				break
			end match
			match V.v more, seq
				PE.Settings.MajorSubsystemVersion = V
				PE.Settings.MinorSubsystemVersion = v
				redefine seq more
			end match
		end match
	end while
	format binary as '.efi'
	format?.include 'pe.inc'
;	format?.include '../cpu/p6.inc'
;default to modern processors
	format?.include '../x86/x64.inc'
	use32
end macro

macro format?.PE64? settings
	PE.Settings.Magic = 0x20B
	PE.Settings.Machine = IMAGE_FILE_MACHINE_AMD64
	PE.Settings.Characteristics = IMAGE_FILE_EXECUTABLE_IMAGE or IMAGE_FILE_LARGE_ADDRESS_AWARE or IMAGE_FILE_LINE_NUMS_STRIPPED or IMAGE_FILE_LOCAL_SYMS_STRIPPED
	PE.Settings.DllCharacteristics = 0
	PE.Settings.Subsystem = IMAGE_SUBSYSTEM_EFI_APPLICATION
	PE.Settings.MajorSubsystemVersion = 5
	PE.Settings.MinorSubsystemVersion = 0
	local seq
	define seq settings:
	while 1
		match :, seq
			break
		else match =at? base =on? stub :, seq
			PE.Settings.ImageBase = base
			PE.Settings.Stub = stub
			break
		else match =at? base :, seq
			PE.Settings.ImageBase = base
			break
		else match =on? stub :, seq
			PE.Settings.Stub = stub
			break
		else
			match =EFI? more, seq
				PE.Settings.Subsystem = IMAGE_SUBSYSTEM_EFI_APPLICATION
				redefine seq more
			else match =EFIboot? more, seq
				PE.Settings.Subsystem = IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER
				redefine seq more
			else match =EFIruntime? more, seq
				PE.Settings.Subsystem = IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER
				redefine seq more
			else
				err 'invalid argument'
				break
			end match
			match V.v more, seq
				PE.Settings.MajorSubsystemVersion = V
				PE.Settings.MinorSubsystemVersion = v
				redefine seq more
			end match
		end match
	end while
	format binary as '.efi'
	format?.include 'pe.inc'
	format?.include '../x86/x64.inc'
	use64
end macro
