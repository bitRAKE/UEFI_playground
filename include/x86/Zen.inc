; shouldn't need this but error without
macro calminstruction?.asmcmd? pattern&
	local	cmd
	arrange cmd, pattern
	assemble cmd
end macro


; see https://gpuopen.com/videos/amd-ryzen-processor-software-optimization/

; Zen(+): ADX,CLFLUSHOPT,RDSEED,SHA,SMAP,XGETBV,XSAVEC,XSAVES,AVX2,BMI2,
; MOVBE,RDRND,SMEP,FSGSBASE,XSAVEOPT,BMI,FMA,F16C,AES,AVX,OSXSAVE,PCLMULQDQ,
; SSE4.1,SSE4.2,XSAVE,SSSE3,MONITORX,CLZERO
;
; Zen 2: MOVBE,MMX,SSE,SSE2,SSE3,SSSE3,SSE4A,SSE4.1,SSE4.2,POPCNT,
; AVX,AVX2,AES,PCLMUL,FSGSBASE,RDRND,FMA3,F16C,BMI,BMI2,RDSEED,
; ADCX,PREFETCHW,CLFLUSHOPT,XSAVE,SHA,UMIP,CLZERO
;
; New to Zen 2 (over Zen 1): 2019 17h family
;	CLWB - Write back modified cache line and may retain line in cache hierarchy
;	WBNOINVD - Write back and do not flush internal caches, initiate same of external caches
;	MCOMMIT - Commit stores to memory
;	RDPID - Read Processor ID
;	RDPRU - Read Processor Register
;	User mode instruction prevention (UMIP) support.



include 'cpu\ext\avx2.inc'
include 'cpu\ext\sse4a.inc'
include 'cpu\ext\bmi2.inc'
include 'cpu\ext\fma.inc'
include 'cpu\ext\adx.inc'
include 'cpu\ext\vaes.inc'
include 'cpu\ext\f16c.inc'
include 'cpu\ext\rdtscp.inc'
include 'cpu\ext\rdrand.inc'
include 'cpu\ext\rdseed.inc'
include 'cpu\ext\xsave.inc'


calminstruction clzero? dest*
        asmcmd  =x86.=parse_operand =@dest,dest
        check   @dest.type = 'reg' & @dest.rm = 0 & @dest.size <> 1
        jyes    _prefix
        asmcmd  =err 'operand must be rAX'
        exit
_prefix:
        asmcmd  =x86.=store_operand_prefix =@dest.=size
        asmcmd  =db 0Fh,1,0FCh
end calminstruction

macro clwb?
db 66h,0Fh,0AEh ; /6
end macro
macro wbnoinvd?
db 0F3h,0Fh,09h
end macro
macro mcommit?
db 0F3h,0Fh,01h,0FAh
end macro
macro rdpid?
db 0F3h,0Fh,0C7h
end macro
macro rdpru?
db 0Fh,01h,0FDh
end macro

macro mwaitx?
db 0Fh,01h,0FBh
end macro
macro monitorx?
db 0Fh,01h,0FAh
end macro
