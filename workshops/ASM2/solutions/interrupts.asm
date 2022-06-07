PIXEL_YY EQU 600AH
PIXEL_XX EQU 600CH
PIXEL_COLOR EQU 6012H
PIXEL_CLEAR EQU 6000H

LINE EQU 10

PLACE 1000H

STACK 100H
SP_INIT:

;
; TODO: Declarar BTE
;

BTE_Start:
	WORD move_right_interrupt
	WORD 0
	WORD 0
	WORD 0

POSITION: WORD 0
MOVE_RIGHT_FLAG: WORD 0

PLACE 0

MOV SP, SP_INIT

;
; TODO: Inicializar BTE e ativar interrupts
;

MOV BTE, BTE_Start
EI0
EI

MOV R1, [POSITION]
MOV R2, LINE

CALL paint_pixel

main:
	
	CALL move_right
	
	JMP main


move_right:
	PUSH R0
	PUSH R1
	PUSH R2

	;
	; Sincronizar com interrupçao
	;

	MOV R0, [MOVE_RIGHT_FLAG]
	CMP R0, 0
	JZ move_right_ret
	MOV R0, 0
	MOV [MOVE_RIGHT_FLAG], R0

	MOV R1, [POSITION]
	INC R1
	MOV [POSITION], R1 ; update position

	MOV R2, LINE
	CALL paint_pixel   ; draw pixel in new position

	move_right_ret:
		POP R2
		POP R1
		POP R0
		RET

paint_pixel:
	; R1 - pixel column
	; R2 - pixel line
	PUSH R0

	MOV R0, 0
	MOV [PIXEL_CLEAR], R0

	MOV [PIXEL_XX], R1
	MOV [PIXEL_YY], R2

	MOV R0, 0F0FFH
	MOV [PIXEL_COLOR], R0

	POP R0
	RET
;
; TODO: Rotinas de Interrupção
;

move_right_interrupt:
	PUSH R0

	MOV R0, 1
	MOV [MOVE_RIGHT_FLAG], R0

	POP R0
	RFE