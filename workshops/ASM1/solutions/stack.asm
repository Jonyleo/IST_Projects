VEC_SIZE EQU 6

PLACE 1000H

VEC1: WORD 10, 1, 77, 410, 99, 3
VEC2: WORD 7, 10, 53, 3, 4, 0
VEC3: WORD 0, 4, 1, 5, 7, 13

STACK 100H
	SP_init:

PLACE 0

; Calcular produto vetorial de VEC1 * VEC2 + VEC2 * VEC3 e colocar o resultado no R0
; Resultado: 1723H
; Utilizar uma rotina

MOV SP, SP_init

MOV R1, VEC1
MOV R2, VEC2
MOV R3, VEC_SIZE * 2

CALL produto_vetorial

MOV R4, R0

MOV R1, VEC2
MOV R2, VEC3

CALL produto_vetorial

ADD R0, R4

end:
	JMP end

produto_vetorial:
	; R1 - Endereco do primeiro vetor
	; R2 - Endereco do segundo vetor
	; R3 - Tamanho do vetor em bytes

	PUSH R4
	PUSH R5
	PUSH R6

	MOV R0, 0 ; Acumulador
	MOV R4, 0 ; Contador de bytes processados (2 em 2)

	produto_vetorial_loop:
		CMP R4, R3 	; Se forem iguais chegamos ao fim
		JZ produto_vetorial_ret

		MOV R5, [R1 + R4]
		MOV R6, [R2 + R4]

		MUL R5, R6
		ADD R0, R5 	; acumula multiplicações

		ADD R4, 2 	; proximo numero do vetor está 2 bytes a seguir
		JMP produto_vetorial_loop

	produto_vetorial_ret:
		POP R6
		POP R5
		POP R4
		RET