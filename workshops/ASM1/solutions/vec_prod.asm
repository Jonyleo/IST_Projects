PLACE 1000H

VEC1: WORD 10, 1, 77, 410, 99, 3
VEC2: WORD 7, 10, 53, 3, 4, 0

PLACE 0

; Calcular produto vetorial de VEC1 * VEC2 e colocar o resultado no R0
; Resultado: 169BH


MOV R0, 0
MOV R1, VEC1
MOV R2, VEC2

MOV R3, 0
MOV R4, 12


vec_prod:
	CMP R3, R4
	JZ end

	MOV R5, [R1 + R3]
	MOV R6, [R2 + R3]

	MUL R5, R6
	ADD R0, R5

	ADD R3, 2

	JMP vec_prod


end:
	JMP end