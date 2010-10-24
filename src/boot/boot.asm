;FAT12 boot
;
;
; memory layout:
;
; 0 0000
;	Realmode IVT
; 0 0400
;	BDA
; 0 0500
;	stack
; 0 1000
;	FAT 
; 0 2200
;	map of sectors of current file/dir
; 0 4200
;	current directory part
; 0 4400
; 0 7C00
;	bootcode ie this code
; 0 7E00
;	loaded kernel ( 607,5 kb )
; 9 FC00
;	EBDA
; A 0000
;	video memory
; C 0000
;	bios stuff
; F FFFF
;
[ORG 0x7C00]
[BITS 16]


;------ FAT structure
		jmp near bootCode	; JmpBoot
		db "sSenSsEN"		; OSName	--- (8 characters)
		dw 0x0200 ;512		; BytesPerSec
		db 0x01			; SecPerClus
		dw 0x0001		; RsvdSecCnt
		db 0x02			; NumFATs
		dw 0x00e0 ;224		; RootEntCnt
		dw 0x0b40 ;2880		; TotSec
		db 0xF0			; MediaType
		dw 0x0009		; FATSize
		dw 0x0012 ;18		; SecPerTrack
		dw 0x0002		; NumHeads
		dd 0x00000000		; HiddenSec
		dd 0x00000000		; TotSec32
		db 0x00			; DrvNum
		db 0x00			; Reserved
		db 0x29			; BootSig
		dd 0x37816173 ;eligible	; VolumeID	--- ((doubleword) usually generated from date)
		db "sSen       "	; VolumeLabel	--- (11 characters)
		db "FAT12   "		; FileSysType


;------ data

; kernel path: FAT shortnames (11 bytes long and uppercase) stacked together. (zero terminated)
; if the kernel is located at "/SSEN/KERNEL.BIN" the corresponding kernel path would be "'SSEN       KERNEL  BIN',0"
kernelPath:	db "SSEN       KERNEL  BIN",0


bootDrive:	db 0x0


GDT:
NULLSEL	EQU $-GDT	; null descriptor ( violated as lgdt structure )
GDTsize		DW GDTend-GDT-1
GDTbase		DD GDT
		DW 0
CODESEL	EQU $-GDT	; 4GB Flat Code at 0x0 with max 0xFFFFF limit
		DW 0xFFFF	; Limit(2):0xFFFF
		DW 0x0		; Base(3)
		DB 0x0		; Base(2)
		DB 0x9A		; Type: present,ring0,code,exec/read/accessed (10011000)
		DB 0xCF		; Limit(1):0xF | Flags:4Kb inc,32bit :0xC<<4 = 1100xxxx
		DB 0x0		; Base(1)
DATASEL	EQU $-GDT       ; 4GB Flat Data at 0x0 with max 0xFFFFF limit
		DW 0xFFFF	; Limit(2):0xFFFF
		DW 0x0		; Base(3)
		DB 0x0		; Base(2)
		DB 0x92		; Type: present,ring0,data/stack,read/write (10010010)
		DB 0xCF		; Limit(1):0xF | Flags:4Kb inc,32bit :0xC<<4 = 1100xxxx
		DB 0x0		; Base(1)
GDTend:



;----- functions

; ax<- start sector of file/dir (from FAT)
; 0x1000<- contains fat
; 0x2200-> generated map
%macro getMap 0
		mov di,0x2200
getMapLoop:

		mov bx,ax
		add bx,31
		mov [di],bx	; add new sector
		inc di
		inc di

		; calculate Fat table-entry offset
		mov cx,ax

		xor dx,dx
		mov bx,3
		mul bx 
		dec bx
		div bx	; dx cleared through mul
		mov bx,ax
		mov ax,[bx+0x1000]

		test cl,1	; even or odd datasector?
		jz fatRight
		shr ax,4
fatRight:
		and ax,0xFFF

		cmp ax,0xFFF	; end of file ?
		jne getMapLoop

		mov byte [di],0	; mark end of map
%endmacro



; ax<- = sector to read from floppy (first sector is 0)
; es<- = address to place data (has to be 0x20(0) aligned to allow read over 64k boundaries)
; es-> = es + 0x20 (set for next read)
readFloppy:

		; calculate chs
		xor dx,dx
		mov bx,18	; sector per cylinder/track
		div bx		; ax/bx -> ax,dx
		inc dx
		mov cl,dl	; sector -> cl
		xor dx,dx
		mov bx,2	; num heads
		div bx
		mov ch,al	; track -> ch
		mov dh,dl	; head  -> dh
		mov dl,[bootDrive]

		; read the data
		xor bx,bx	; address is in es
readFloppyAgain:
		mov ax,0x0201	; read 1 sector
		int 0x13
		jnc readOK 
resetFloppy:
		xor ax,ax	; reset floppydrive
		int 0x13
		jc resetFloppy
		jmp readFloppyAgain
readOK:
		; set next address
		mov bx,es
		add bx,0x20
		mov es,bx

		ret



;-------- lets go

bootCode:

		; initialize environment
 		jmp 0x0:biosfix	; some bioses set cs and ds wrong
biosfix:	
 		xor ax,ax
 		mov ds,ax

 		mov ss,ax	; set stack to 0x1000
		mov ah, 0x10
		mov sp, ax

 		mov [bootDrive],dl


		; load fat table
		mov ah,0x01	; read to 0x1000
		mov es,ax
		xor si,si	; fat place on floppy
		mov cx,9	; fat size
fatLoop:
		inc si
		mov ax,si
		call readFloppy
		loop fatLoop


		; create "fake" map for root dir
		mov bx,0x2200
		mov ax,19
		mov cx,14
fakeMap:
		mov [bx],ax
		inc ax
		inc bx
		inc bx
		loop fakeMap
		mov [bx],cl	; mark end of map ( cl == 0 )
		

		mov bp,kernelPath

; find the file/dir
; bp<- = points to path
; 0x2200 contains map for the current directory
; 0x1000 contains fat
find:
		mov bx,0x2200
findLoop:
		; get next dir part from map
		mov ax,[bx]	; get sector
		or ax,ax
		jz $		; end of map indicates dir/file not found
		inc bx
		inc bx
		push bx
			; load next part of dir
			mov bx,0x0420
			mov es,bx
			call readFloppy

			; search dir/file entry
			push ds		; cmpsb compares [ds:si] and [es:di]
			pop es
			mov bx,0x4200	; begin of dir part
			mov cx,16
findCheckLoop:
			push cx
				mov di,bx
				mov si,bp
				mov cl,11	; ch is 0
				repe cmpsb
				je findOK
			pop cx
			add bx,32
			loop findCheckLoop	; next dir entry
		pop bx
		jmp findLoop


; found next part of path
findOK:
		; add sp,6	; consistent stack not needed

		mov ax,[di+15]	; get first sector
		mov si,[di]
		push di
			getMap
		pop di
		test si,0x10	; is directory ?
		jz foundKernel
		add bp,11
		cmp byte [bp],0	; end of path ?
		je $
		jmp find


; kernel located
foundKernel:
		mov bp,di	; save file entry location

		; load kernel to 0x7E00
		mov ax,0x07E0
		mov es,ax
		mov di,0x2200
loadLoop:
		mov ax,[di]	; get sector
		or ax,ax
		jz loadedKernel	; end of map indicates end of kernel
		inc di
		inc di

		call readFloppy

		jmp loadLoop


; kernel loaded
loadedKernel:
		cli		; disable interrupts


		; stop floppy engine
		mov al,0x0C
		mov dx,0x03F2
		out dx,al



; enable A20 gate

;   via system control port A
		mov al,0x02
		out 0x92,al
		in al,0x92
		test al,0x02
		jnz A20Enabled

;   via keyboard controller
kbdctrl:
		in al,0x64
		test al,2
		jnz kbdctrl
		mov al,0xDF
		out 0x64,al

A20Enabled:


; switch to protected mode
		lgdt [GDT]	; load GDT
		mov eax,cr0
		or al,0x01	; set protected mode bit
		mov cr0,eax
		jmp CODESEL:PM	; flush pipeline

[BITS 32]
PM:
		; set segment registers
		mov ax,DATASEL
		mov ds,ax
		mov es,ax
		;mov fs,ax
		;mov gs,ax
		mov ss,ax


		mov eax,'ONKN'
		mov [0xb8001],eax


		; search multiboot magic
		mov esi,0x7E00
		mov edi,esi
		mov eax,0x1BADB002
		mov ecx,0x2000 / 0x4	; magic value must be within the first 8kb of the kernel
multiMagic:
		cmp [edi],eax
		je magicFound
		inc edi
		inc edi
		inc edi
		inc edi
		loop multiMagic
		jmp $
magicFound:
		
		mov ebx,[edi+28]	; get start address
		
		mov eax,[edi+12]
		sub eax,edi
		add eax,esi
		mov edi,eax	; calculated loadaddress

		; copy the kernel into high memory
		mov ecx,[bp+17]	; size of kernel
		rep movsb


		; switch to the kernel
		mov eax,0x2BADB002	; multiboot bootloader magic

		jmp ebx	; jump into kernel


;------- make Bootable

	times 510-($-$$) db 0
	dw 0xAA55	;MagicKey



