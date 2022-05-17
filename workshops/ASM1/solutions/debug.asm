N_PRIMES EQU 200H

PLACE 1000H

primes:
TABLE N_PRIMES

PLACE 0

; Calcular N_PRIMES e guarda-os na tabela primes
; ** Encontra o bug **


MOV R0, 0 
MOV R4, N_PRIMES * 2
MOV R5, primes
MOV R1, 2 


loop:
	CMP R0, R4
	JZ end

	MOV R2, 2

	loop2:
		CMP R1, R2
		;JNZ found 
		JZ found

		MOV R3, R1
		MOD R3, R2

		JZ step

		INC R2
		JMP loop2

	found:
		MOV [R5 + R0], R1
		ADD R0, 2

	step:
		
		INC R1
		JMP loop

end:
	JMP end