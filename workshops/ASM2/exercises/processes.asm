PIXEL_YY EQU 600AH
PIXEL_XX EQU 600CH
PIXEL_COLOR EQU 6012H
PIXEL_CLEAR EQU 6000H

LINE EQU 10

PLACE 1000H

STACK 100H
SP_INIT:

;
; TODO: Declarar stacks dos processos
;

BTE_START:
	WORD move_down_interrupt
	WORD move_right_interrupt
	WORD 0
	WORD 0

MOV_DOWN: WORD 0
MOV_RIGHT: WORD 0

POSITION_XX: WORD 0
POSITION_YY: WORD 0

PLACE 0

MOV SP, SP_INIT
MOV BTE, BTE_START

EI0
EI1
EI

MOV R1, [POSITION_XX]
MOV R2, [POSITION_YY]

CALL paint_pixel

;
; TODO: change main program
;

main:
	CALL move_down
	CALL move_right
	
	JMP main

;
; TODO: tranforma move_down e move_right em processos, cada um com o seu ciclo infinito
;

move_down:
	PUSH R0
	PUSH R1
	PUSH R2

	MOV R0, [MOV_DOWN]
	CMP R0, 0
	JZ move_down_ret   ; check if time to move down

	MOV R0, 0
	MOV [MOV_DOWN], R0 ; reset flag

	MOV R2, [POSITION_YY]
	INC R2
	MOV [POSITION_YY], R2 ; update position

	MOV R1, [POSITION_XX]
	CALL paint_pixel   ; draw pixel in new position
	
	move_down_ret:
		POP R2
		POP R1
		POP R0
		RET

move_right:
	PUSH R0
	PUSH R1
	PUSH R2

	MOV R0, [MOV_RIGHT]
	CMP R0, 0
	JZ move_right_ret   ; check if time to move right

	MOV R0, 0
	MOV [MOV_RIGHT], R0 ; reset flag

	MOV R1, [POSITION_XX]
	INC R1
	MOV [POSITION_XX], R1 ; update position

	MOV R2, [POSITION_YY]
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
; Interrupts
;

move_down_interrupt:
	PUSH R0

	MOV R0, 1
	MOV [MOV_DOWN], R0

	POP R0
	RFE


move_right_interrupt:
	PUSH R0

	MOV R0, 1
	MOV [MOV_RIGHT], R0

	POP R0
	RFE