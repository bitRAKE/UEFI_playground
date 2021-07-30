; Universal Coded Character Set (UCS)
; UCS-2 is obsolete, mainly only used for UEFI

macro UCS2 arg
  local current,__input,char,wide
  if arg eqtype ''
    virtual at 0
      __input::
        db arg
      count = $
    end virtual
    current = 0
    while current < count
      load wide byte from __input:current
      current = current + 1
      if wide > 0C0h
	if wide < 0E0h
	  load char byte from __input:current
	  current = current + 1
	  wide = (wide and 11111b) shl 6 + (char and 111111b)
	else if wide < 0F0h
	  load char byte from __input:current
	  wide = (wide and 1111b) shl 6 + (char and 111111b)
	  load char byte from __input:current+1
	  wide = wide shl 6 + (char and 111111b)
	  current = current + 2
	else if wide < 0F8h
	  load char byte from __input:current
	  wide = (wide and 111b) shl 6 + (char and 111111b)
	  load char byte from __input:current+1
	  wide = wide shl 6 + (char and 111111b)
	  load char byte from __input:current+2
	  wide = wide shl 6 + (char and 111111b)
	  current = current + 3
	else if wide < 0FCh
	  load char byte from __input:current
	  wide = (wide and 11b) shl 6 + (char and 111111b)
	  load char byte from __input:current+1
	  wide = wide shl 6 + (char and 111111b)
	  load char byte from __input:current+2
	  wide = wide shl 6 + (char and 111111b)
	  load char byte from __input:current+3
	  wide = wide shl 6 + (char and 111111b)
	  current = current + 4
	else
	  load char byte from __input:current
	  wide = (wide and 1) shl 6 + (char and 111111b)
	  load char byte from __input:current+1
	  wide = wide shl 6 + (char and 111111b)
	  load char byte from __input:current+2
	  wide = wide shl 6 + (char and 111111b)
	  load char byte from __input:current+3
	  wide = wide shl 6 + (char and 111111b)
	  load char byte from __input:current+4
	  wide = wide shl 6 + (char and 111111b)
	  current = current + 5
	end if
      end if
      if wide < 10000h
	dw wide
      else
	err 'UTF-8 code point outside of UCS-2 range'
      end if
    end while
  else
    wide = arg
    if wide < 10000h
      dw wide
    else
	err 'UTF-8 code point outside of UCS-2 range'
    end if
  end if
end macro


calminstruction du? definitions&
	local	value, n
	transform definitions
    start:
	match	value=,definitions, definitions, ()
	jyes	recognize
	match	value, definitions
	arrange definitions,
    recognize:
	match	n =dup? value, value, ()
	jyes	duplicate
	match	?, value
	jyes	reserve
	arrange value, =UCS2 value
	assemble value
    next:
	match	, definitions
	jno	start
	take	, definitions
	take	definitions, definitions
	jyes	next
	exit
    reserve:
	arrange value, =dw ?
	assemble value
	jump	next
    duplicate:
	match	(value), value
    stack:
	check	n
	jno	next
	take	definitions, value
	arrange value, definitions
	compute n, n - 1
	jump	stack
end calminstruction

calminstruction (label) du? definitions&
	local	cmd
	arrange cmd, =label label : =word
	assemble cmd
	arrange cmd, =du definitions
	assemble cmd
end calminstruction
