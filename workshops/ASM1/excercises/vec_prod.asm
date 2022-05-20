PLACE 1000H
VEC1: WORD 10, 1, 77, 410, 99, 3
VEC2: WORD 7, 10, 53, 3, 4, 0


PLACE 0H

; Calcular produto vetorial de VEC1 * VEC2 e colocar o resultado no R0
; Resultado: 169BH


MOV R3, VEC1
MOV R4, VEC2
MOV R0, 0
MOV R5, 6

produto_vetorial_loop:
	MOV R1, [R3]
	MOV R2, [R4]

	ADD R3, 2
	ADD R4, 2

	MUL R1, R2

	ADD R0, R1

	DEC R5
	JNZ produto_vetorial_loop

end:
	JMP end