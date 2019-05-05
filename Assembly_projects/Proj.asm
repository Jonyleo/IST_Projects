;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Definicao de constantes numericas
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

PIX_SCR         EQU 08000H  ; endereco do pixel screen
DISPLAYS        EQU 0A000H  ; endereco dos displays de 7 segmentos
TEC_LIN         EQU 0C000H  ; endereco das linhas do teclado
TEC_COL    	    EQU 0E000H  ; endereco das colunas do teclado
LINE_MASC      	EQU 0010H   ; mascara que determina a linha a testar

PDr_COL_MUL 	EQU 0002H   ; auxilia no calculo do modificador de coluna
PDr_LIN_MUL 	EQU 0004H   ; auxilia no calculo do modificador de linha
PDr_COL_MODULE  EQU 0010H   ; auxilia no calculo da posicao do bit na word
PDr_WORD_MASC   EQU 8000H   ; mascara para calcular a word a enviar para o pixel screen
IDr_WORD_MASC   EQU 8000H   ; mascara para chamar pixel_Draw em image_Draw

SCORE_INCREASE  EQU 0003H   ; incrementacao da pontuacao
SCORE_MAX       EQU 0063H	; pontuacao maxima em hexadecimal	

METEOR_RIGHT    EQU 0200H 	; desvio dos meteoritos para a direita
METEOR_LEFT		EQU 0FE00H  ; desvio dos meteoritos para a esquerda
GOOD            EQU 0001H   ; estado bom do meteorito
METEOR_GOODTOBAD_RATIO EQU 0004H ; aumentar este valor aumenta a probabilidade de um meteorito ser MAU, a probabilidade de ser BOM é 1/METEOR_GOODTOBAD_RATIO

TRUE            EQU 0001H 	; verdadeiro ;
FALSE           EQU 0000H 	; falso 	 ; Estes EQUs ajudam numa melhor leitura do codigo, em comparacoes e atribuicoes de variaveis
NULL            EQU 0000H 	; nulo 		 ;
LIST_END        EQU 0FFFFH  ; valor que determina fim de listas
NEXT_WORD       EQU 0002H   ; incremento para o proximo endereco par(WORD) na memoria

HEXTODEC_MODULE    EQU 000AH	; variavel auxiliar na conversao de hexadecimal para decimal

GAME_STATE      EQU 0000H 	; estado de jogo
MENU_STATE      EQU 0001H 	; estado de menu
PAUSE_STATE     EQU 0002H 	; estado de pausa
GAMEOVER_STATE      EQU 0003H 	; estado de gameover

MISSILE_1_ST    EQU 081BH 	; coordenada inicial do missil da esquerda 
MISSILE_2_ST    EQU 171BH 	; coordenada inicial do missil da direita
MISSILE_MAXHEIGHT   EQU 000FH  ; altura maxima do missil

METEOR_ST       EQU 1000H 	; coordenada inical dos meteoros

PLACE 2000H

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Descricao das tabelas de imagens
; A primeira WORD dita as coordenadas X e Y do pixel do canto superior Direito da imagem 
; A segunda WORD dita a largura e altura do quadrado de pixeis em que a imagem se insere
; A seguinte informação dita os pixeis a pintar, 1 por bit
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

MENU: WORD 0000H 				; ecra do menu
	  WORD 2020H

STRING 00H, 00H, 00H, 00H, 08H, 0BBH, 0BBH, 0B8H
STRING 0DH, 0A1H, 22H, 0A8H, 0AH, 0B9H, 3AH, 0B8H
STRING 08H, 0A1H, 22H, 0B0H, 08H, 0B9H, 3BH, 0A8H
STRING 00H, 00H,  00H, 00H, 0EH, 0AEH, 8BH, 0B8H
STRING 08H, 0AAH, 8AH, 28H, 0EH, 0EAH, 0ABH, 0B8H
STRING 02H, 0AAH, 0DAH, 30H, 0EH, 0AEH, 8BH, 0A8H
STRING 00H, 00H, 00H, 00H, 00H, 00H, 00H, 00H
STRING 00H, 00H, 80H, 00H, 00H, 00H, 00H, 00H
STRING 00H, 06H, 30H, 00H, 01H, 46H, 30H, 00H
STRING 02H, 0A0H, 00H, 00H, 01H, 40H, 04H, 40H
STRING 02H, 0A0H, 0A2H, 80H, 01H, 40H, 41H, 00H
STRING 00H, 00H, 0A2H, 80H, 00H, 80H, 04H, 40H
STRING 00H, 00H, 00H, 00H, 00H, 00H, 01H, 00H
STRING 00H, 00H,  00H, 00H, 0EH, 0BFH, 0FDH, 70H
STRING 12H, 0A0H, 05H, 48H, 23H, 60H, 06H, 0C4H
STRING 40H, 83H, 0C1H, 02H, 80H, 00H, 00H, 01H



GAMEOVER: WORD 0000H 			; ecra do gameover
		  WORD 2020H
STRING 00H, 00H, 00H, 00H, 00H, 00H, 00H, 00H
STRING 00H, 00H, 00H, 00H, 00H, 07H, 0F0H, 00H
STRING 00H, 18H, 08H, 00H, 00H, 20H, 04H, 00H
STRING 00H, 40H, 02H, 00H, 00H, 80H, 01H, 00H
STRING 01H, 00H, 00H, 80H, 02H, 00H, 00H, 40H
STRING 02H, 00H, 00H, 40H, 02H, 00H, 00H, 40H
STRING 02H, 70H, 07H, 40H, 02H, 70H, 07H, 40H
STRING 02H, 7CH, 1FH, 40H, 01H, 3FH, 7EH, 40H
STRING 00H, 80H, 80H, 80H, 00H, 81H, 40H, 40H
STRING 01H, 09H, 40H, 40H, 01H, 38H, 0EH, 40H
STRING 01H, 0CAH, 0A9H, 0C0H, 00H, 0AH, 0A8H, 00H
STRING 00H, 0AH, 0A8H, 00H, 00H, 0AH, 0A8H, 00H
STRING 00H, 0AH, 0A8H, 00H, 00H, 0AH, 0A8H, 00H
STRING 00H, 0AH, 0A8H, 00H, 00H, 05H, 50H, 00H
STRING 00H, 00H, 00H, 00H, 00H, 00H, 00H, 00H
STRING 00H, 00H, 00H, 00H, 00H, 00H, 00H, 00H

SHIP: WORD 001BH        		; nave
	  WORD 2005H
	STRING 0EH, 0BFH, 0FDH, 70H, 12H, 0A0H, 05H, 48H
	STRING 23H, 60H, 06H, 0C4H, 40H, 80H, 01H, 02H
	STRING 80H, 00H, 00H, 01H
	
STEER_STOP: WORD 0E1EH 			; volante quanto a nave vai em frente
	        WORD 0401H
	WORD 0F000H

STEER_LEFT: WORD 0E1DH      	; volante enquanto a nave vira para a esquerda
	  		WORD 0403H
	WORD 1000H
	WORD 6000H
	WORD 8000H
	
STEER_RIGHT: WORD 0E1DH 		; volante enquanto a nave vira para a direita
			 WORD 0403H
	WORD 8000H
	WORD 6000H
	WORD 1000H


METEOR_SMALL: WORD 0000H 		; meteorito pequeno
			  WORD 0101H
	WORD 8000H

METEOR_MEDIUM: WORD 0000H 		; meteorito medio
			   WORD 0202H
	WORD 0C000H
	WORD 0C000H

BAD_METEOR_LARGE: WORD 0000H 	; meteorito grande (mau)
				  WORD 0303H
	WORD 0E000H
	WORD 0A000H
	WORD 0E000H

BAD_METEOR_HUGE: WORD 0000H 	; meteorito enorme (mau)
				 WORD 0404H
	WORD 6000H
	WORD 9000H
	WORD 9000H
	WORD 6000H

BAD_METEOR_GIANT: WORD 0000H 	; meteorito gigante (mau)
				  WORD 0505H
	WORD 7000H
	WORD 8800H
	WORD 8800H
	WORD 8800H
	WORD 7000H

METEOR_EXPLODE: WORD 0000H 		; meteorito explodido
				WORD 0505H
	WORD 5000H
	WORD 0A800H
	WORD 5000H
	WORD 0A800H
	WORD 5000H

GOOD_METEOR_LARGE: WORD 0000H 	; meteorito grande (bom)
				   WORD 0303H
	WORD 0A000H
	WORD 04000H
	WORD 0A000H

GOOD_METEOR_HUGE:  WORD 0000H 	; meteorito enorme (bom)
				   WORD 0404H
	WORD 9000H
	WORD 6000H
	WORD 6000H
	WORD 9000H

GOOD_METEOR_GIANT: WORD 0000H 	; meteorito gigante (bom)
				   WORD 0505H
	WORD 8800H
	WORD 5000H
	WORD 2000H
	WORD 5000H 	
	WORD 8800H

MISSILE_DRAW: WORD 0000H
		 WORD 0101H
		 WORD 8000H

MISSILE_1: WORD NULL		; direcao
		   WORD MISSILE_1_ST; coordenadas no Pixel Screen

MISSILE_2: WORD NULL 		; direcao
		   WORD MISSILE_2_ST; coordenadas no Pixel Screen

MISSILE_LIST:
	WORD MISSILE_1
	WORD MISSILE_2
	WORD LIST_END

METEOR_1: WORD NULL 			; direcao
		  WORD METEOR_ST 		; coordenadas no Pixel Screen
		  WORD METEOR_SMALL 	; estado
		  WORD 0000H     		; tipo
		 	  
METEOR_2: WORD NULL 			; direcao
		  WORD METEOR_ST 		; coordenadas no Pixel Screen
		  WORD METEOR_SMALL 	; estado
		  WORD 0000H 			; tipo

METEOR_3: WORD NULL 			; direcao
		  WORD METEOR_ST 		; coordenadas no Pixel Screen
		  WORD METEOR_SMALL 	; estado
		  WORD 0000H			; tipo

METEOR_4: WORD NULL 			; direcao
		  WORD METEOR_ST 		; coordenadas no Pixel Screen
		  WORD METEOR_SMALL 	; estado
 		  WORD 0000H 			; tipo

METEOR_LIST: 
	WORD METEOR_1
	WORD METEOR_2
	WORD METEOR_3
	WORD METEOR_4
	WORD LIST_END

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Inicializacao do Stack pointer
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

PLACE 1000H

pile_init:				
	TABLE 100H
 SP_inicial:

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Inicializacao da tabela de interrupcões
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

int_init:
	WORD  int_MoveMeteor
	WORD  int_MoveMissile

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Declaracao de variaveis
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SCORE_HEXA:    WORD NULL
SCORE_UPDATE:  WORD FALSE
RANDOM_NUMBER: WORD NULL
RANDOM_RENEW:  WORD 0003H

KEY_PRESSED:  WORD NULL
KEY_PRESSING: WORD NULL
KEY_UPDATE:   WORD FALSE

METEOR_MOD:   	 WORD NULL
PROGRAM_STATE:   WORD MENU_STATE
STEER_STATE:     WORD NULL

MOVE_METEOR:    WORD FALSE
MOVE_MISSILE:   WORD FALSE
CREATE_MISSILE: WORD FALSE
NEXT_MISSILE:   WORD 0001H


VAR_LIST:
	WORD SCORE_UPDATE
	WORD KEY_PRESSED
	WORD KEY_PRESSED
	WORD KEY_UPDATE
	WORD METEOR_MOD
	WORD STEER_STATE
	WORD MOVE_MISSILE
	WORD MOVE_METEOR
	WORD CREATE_MISSILE
	WORD LIST_END

KEY_LIST:
	WORD key_Action_Null
	WORD key_Action_PlaceHolder 
	WORD key_Action_PlaceHolder
	WORD key_Action_PlaceHolder
	WORD key_Action_PlaceHolder
	WORD key_Action_4
	WORD key_Action_5
	WORD key_Action_6
	WORD key_Action_PlaceHolder
	WORD key_Action_PlaceHolder
	WORD key_Action_PlaceHolder
	WORD key_Action_PlaceHolder
	WORD key_Action_PlaceHolder
	WORD key_Action_C
	WORD key_Action_D
	WORD key_Action_PlaceHolder
	WORD key_Action_PlaceHolder


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Código
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

PLACE 0000H

program_Init:					; inicializa o programa 
	MOV  SP, SP_inicial 		; inicializa o stack pointer
	MOV  BTE, int_init			; inicializa as interrupcões 
	CALL display_Reset
	CALL menu_Init
	EI 							
	EI0
	EI1

program_Main: 					; loop principal do programa
	CALL random_Gen
	CALL score_Update
	CALL key_Proccess
	CALL meteor_Proccess
	CALL missile_Proccess

	JMP  program_Main

;Game States - Inicializações, processos e reset

menu_Init: 						; inicializa o menu
	PUSH R0
	PUSH R2

	CALL game_Reset 	 		; reinicializa o jogo (exceto a pontuação)
	CALL menu_Draw 				; pinta o menu

	MOV  R0, PROGRAM_STATE
	MOV  R2, MENU_STATE
	MOV  [R0], R2 				; muda o estado do programa para menu

	POP R0
	POP R2
	RET

game_Init: 						; inicializa o jogo
	PUSH R0
	PUSH R2

	MOV  R0, PROGRAM_STATE
	MOV  R1, [R0]
	CMP  R1, PAUSE_STATE
	JZ   game_Init_fromPause 	; verifica se estamos na pausa, se sim, inicializar o jogo e apenas mudar o estado

	CALL score_Reset 			; inicia a pontuação
	CALL game_Reset 			; reinicia o jogo
	CALL cockpit_Draw 			; desenha o cockpit com volante parado
	
 game_Init_fromPause: 			; muda o estado para jogo
	MOV  R0, PROGRAM_STATE
	MOV  R2, GAME_STATE 
	MOV  [R0], R2

	POP  R2
	POP  R0
	RET

pause_Init: 					; inicializa a pausa
	PUSH R0
	PUSH R1

	MOV  R0, PROGRAM_STATE
	MOV  R1, PAUSE_STATE
	MOV  [R0], R1 				; muda o estado para pausa

	POP  R1
	POP  R0
	RET

gameOver_Init: 					; inicializa o game over
	PUSH R0
	PUSH R2

	CALL game_Reset 			; reinicia as variaveis do jogo
	MOV  R2, GAMEOVER 	
	CALL image_Draw 			; pinta o ecrã do game over

	MOV  R0, PROGRAM_STATE
	MOV  R2, GAMEOVER_STATE 	
	MOV  [R0], R2 				; muda os estado para game over

	POP  R0
	POP  R2
	RET

key_Proccess: 					; ler teclado e processar tecla
	CALL key_Sweep 	 	 		; varrer as teclas		
	CALL key_Convert			; converter a tecla
	CALL key_CheckUpdate 		; verificar se ha mudanca de tecla
	CALL key_Action	 			; processar a tecla
	RET 				  

missile_Proccess: 				; processar o movimento dos misseis
	PUSH R0
	PUSH R1

	MOV  R0, PROGRAM_STATE
	MOV  R1, [R0]
	CMP  R1, GAME_STATE
	JNZ  missile_Proccess_Return; verifica se estamos em jogo

	MOV  R0, MOVE_MISSILE
	MOV  R1, [R0]
	CMP  R1, FALSE
	JZ   missile_Proccess_Return; verifica se a flag para mover os misseis esta ativa

	CALL missile_Update 		; apaga os misseis
	CALL missile_FullColision 	; verifica se ha colisao com meteoritos
	CALL missile_OutOffBounds 	; verifica se chegaram ao limite de 12 pixeis
	CALL missile_Renew 			; cria misseis 
	CALL missile_Move 			; move para as novas coordenadas
	CALL missile_Update 		; pinta os misseis nas novas coordenadas

	MOV  R0, MOVE_MISSILE
	MOV  R1, FALSE
	MOV  [R0], R1 				; consome a flag colocando-a a 0(FALSE)

 missile_Proccess_Return:
	POP  R1
	POP  R0
	RET

meteor_Proccess: 				; processar o movimento dos meteoros
	PUSH R0
	PUSH R1

	MOV  R0, PROGRAM_STATE
	MOV  R1, [R0]
	CMP  R1, GAME_STATE
	JNZ  meteor_Proccess_Return ; verifica se o estado e jogo

	MOV  R0, MOVE_METEOR
	MOV  R1, [R0]
	CMP  R1, FALSE
	JZ   meteor_Proccess_Return ; verifica se a flag para mover os meteoritos esta ativa (houve interrupcao)
	
	CALL meteor_Update  		; apaga os meteoritos
	CALL meteor_Exploded 		
	CALL meteor_Renew 		 	; renova meteoritos
 	CALL meteor_Move 			; move meteoritos
 	CALL meteor_OutOffBounds 	; verifica se estao fora do pixel Screen
 	CALL meteor_Colision 		; verifica se algum meteorito bateu na nave
	CALL meteor_Change 			; muda o desenho dos meteoritos consoante o Y
	CALL meteor_Update 			; pinta os meteoritos

	MOV  R0, MOVE_METEOR
	MOV  R1, FALSE
	MOV  [R0], R1 				; consome a flag colocando-a a 0(FALSE)

 meteor_Proccess_Return:
 	POP  R1
 	POP  R0
	RET

game_Reset: 					; reinicializa todas as variaveis 
	PUSH R0

	CALL var_Reset 				; reinicia as variaveis da lista 
	CALL meteor_FullReset 		; reinicia os meteoritoss
	CALL missile_FullReset 		; reinicia os misseis
 	MOV  R0, 1FH 				
 	CALL pixelScreen_Reset 		; limpa o pixel screen
 	
 	POP  R0
	RET

;Rotinas do teclado

key_Sweep:						; regista a tecla pressionada(ou 0000H em caso de nenhuma tecla pressionada) na memoria
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R3

	MOV  R0, TEC_LIN			; endereco das linhas
	MOV  R1, TEC_COL 			; endereco das coluna
	MOV  R2, LINE_MASC 			; mascara para testar linhas
	MOV  R3, NULL

 key_Sweep_Main: 				; faz o varrimento das quatro linhas do teclado

	SHR  R2, 1         		
	JZ   key_Sweep_Return   	; se passado 4 ciclos nao ha tecla pressionada, reseta R9 para 0

	MOVB [R0], R2 				; testa linha em R2
	MOV  R1, TEC_COL
	MOVB R3, [R1] 				; coloca coluna em R3
	MOV  R1, 000FH 				
	AND  R3, R1 				; limpa o byte para obter apenas o nibble low (coluna)
	CMP  R3, NULL
	JZ   key_Sweep_Main     	; se houver tecla pressionada continua 

	SHL  R2, 8
	OR   R3, R2 				; coloca o par Linha-Coluna atual(deste ciclo) em R3

 key_Sweep_Return:
 	MOV  R0, KEY_PRESSING 		; coloca o padrao tecla(atual) numa variavel em memoria para uso futuro
	MOV  [R0], R3
 
 	POP  R3
	POP  R2
	POP  R1
	POP  R0
	RET

key_CheckUpdate:				; verifica se ha uma nova tecla a ser pressionada e atualiza a respetiva variavel em memoria
	PUSH R0
	PUSH R1
	PUSH R2

	MOV  R0, KEY_PRESSED  	
	MOV  R1, [R0]
	MOV  R0, KEY_PRESSING
	MOV  R2, [R0]
	CMP  R1, R2 				; verifica se a tecla pressionada entre o ultimo ciclo e o atual e diferente
	JZ   key_CheckUpdate_Return

	MOV  R0, KEY_UPDATE 		; coloca a flag KEY_UPDATE a 1(TRUE)
	MOV  R1, TRUE
	MOV  [R0], R1

 key_CheckUpdate_Return:
 	POP  R2
 	POP  R1
 	POP  R0
	RET  		 

key_Action:						; processa a tecla que esta a ser premida e chama a respetiva rotina
	PUSH R0
	PUSH R1   				
	PUSH R2		

	MOV  R0, KEY_UPDATE
	MOV  R1, [R0]
	CMP  R1, FALSE
	JZ   key_Action_Return 		; verifica se a flag KEY_UPDATE está a 0(FALSE) ou seja, nao ha nova tecla

	MOV  R0, KEY_PRESSING
	MOV  R1, [R0]

	MOV  R0, 2
	MUL  R1, R0 				; calcula a posicao relativa da rotina da tecla em relacao a lista de teclas

	MOV  R0, KEY_LIST
	ADD  R0, R1 				; calcula a posicao da rotina na memoria

	MOV  R1, [R0]
	CALL R1 					; chama a rotina respetiva de cada tecla 
  
 key_Action_Return:
 	MOV  R0, KEY_UPDATE 		; consome KEY_UPDATE, restaurando a 0(FALSE)
 	MOV  R1, FALSE
 	MOV  [R0], R1

 	MOV  R0, KEY_PRESSED
 	MOV  R1, KEY_PRESSING
 	MOV  R2, [R1] 				
 	MOV  [R0], R2 				; atualiza a tecla premida no ultimo ciclo para a do ciclo atual

 	POP  R2 
	POP  R1
	POP  R0
	RET 


 key_Action_PlaceHolder: 		; rotina que ocupa o lugar de teclas sem acao na lista de teclas
 	RET
 
key_Action_4: 					; virar a nave a esquerda
 	PUSH R0
 	PUSH R2

 	MOV  R0, PROGRAM_STATE
 	MOV  R2, [R0]
 	CMP  R2, GAME_STATE
 	JNZ  key_Action_4_Return 	; verifica se estamos no jogo

 	MOV  R2, STEER_LEFT
 	CALL steer_Draw 			; pinta volante para a esquerda

 	MOV  R0, METEOR_MOD 		
 	MOV  R2, METEOR_RIGHT
 	MOV  [R0], R2				; muda o modificador relativo dos meteoritos e misseis
 key_Action_4_Return:
 	POP  R2
 	POP  R0
 	RET

key_Action_5: 					; disparar missil
	PUSH R0
	PUSH R1

	MOV  R0, PROGRAM_STATE
 	MOV  R1, [R0]
 	CMP  R1, GAME_STATE
 	JNZ  key_Action_5_Return 	; verifica se estamos no jogo

 	MOV  R0, CREATE_MISSILE
 	MOV  R1, TRUE
 	MOV  [R0], R1 				; coloca a flag CREATE_MISSILE a 1(TRUE)  para no resto do ciclo ser criado um missil

 key_Action_5_Return:
 	POP  R1
 	POP  R0
  	RET

key_Action_6: 					; virar a nave a direita
 	PUSH R0
 	PUSH R2

 	MOV  R0, PROGRAM_STATE
 	MOV  R2, [R0]
 	CMP  R2, GAME_STATE
 	JNZ  key_Action_6_Return 	; verifica se estamos no jogo
 

 	MOV  R2, STEER_RIGHT
 	CALL steer_Draw 			; pinta o volante para a direita

 	MOV  R0, METEOR_MOD
 	MOV  R2, METEOR_LEFT 
 	MOV  [R0], R2 				; atualiza o modificador do movimento dos meteoros e misseis
 key_Action_6_Return:
 	POP  R2
 	POP  R0
 	RET

key_Action_C: 					; tira e põe o jogo em pausa, inicia o jogo a partir do menu ou do game over
 	PUSH R0
 	PUSH R1

 	MOV  R0, PROGRAM_STATE
 	MOV  R1, [R0]

 	CMP  R1, PAUSE_STATE
 	JZ   key_Action_Pause_C 	; verifica se estamos na pausa

	CMP  R1, MENU_STATE
 	JZ   key_Action_Menu_C 		; verifica se estamos no menu

 	CMP  R1, GAMEOVER_STATE
 	JZ   key_Action_Menu_C 		; verifica se estamos no game over

 	CALL pause_Init 			

 	JMP  key_Action_C_Return

 key_Action_Menu_C:
 	CALL game_Init

 	JMP key_Action_C_Return

 key_Action_Pause_C:
 	CALL game_Init

 key_Action_C_Return:
 	POP  R1
 	POP  R0
 	RET

key_Action_D: 					; acaba o jogo (para o menu) quando na pausa ou no jogo
 	PUSH R0
 	PUSH R1

 	MOV  R0, PROGRAM_STATE
 	MOV  R1, [R0]
 	CMP  R1, MENU_STATE
 	JZ   key_Action_D_Return 	; se estivermos no menu nao ha jogo para acabar

 	CMP  R1, GAMEOVER_STATE
 	JZ   key_Action_D_Return 	; se estivermos no game over nao ha jogo para acabar

 	CALL menu_Init 	
 key_Action_D_Return:
 	POP  R1
 	POP  R0
 	RET

key_Action_Null: 		 	 	; reseta o voltante
 	PUSH R0
 	PUSH R2

 	MOV  R0, PROGRAM_STATE
 	MOV  R1, [R0]
 	CMP  R1, GAME_STATE
 	JNZ  key_Action_Null_Return; verifica se estamos em jogo, se nao a tecla nula nao tem efeito

 	MOV  R0, STEER_STATE
 	MOV  R2, [R0]

 	MOV  R1, STEER_RIGHT
 	CMP  R2, R1
 	JZ   key_Action_Null_Draw

 	MOV  R1, STEER_LEFT
 	CMP  R2, R1
 	JZ   key_Action_Null_Draw 	; verifica se o volante estava virado para a esquerda ou direita

 	JMP  key_Action_Null_Return

 key_Action_Null_Draw:
 	MOV  R2, STEER_STOP 		
 	CALL steer_Draw 			; pinta o volante parado
 	MOV  R0, METEOR_MOD
 	MOV  R2, 0
 	MOV  [R0], R2 				; atualiza o modificador do movimento

 key_Action_Null_Return:
 	POP  R2
 	POP  R0
 	RET	 

key_Convert: 					; converte o padrao linha coluna para um numero hexadecimal
	PUSH R0 					; 0 corresponde a tecla nula(nenhuma tecla pressionada) e as restantes teclas vão de 1 a 10H
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH R4

	MOV  R0, KEY_PRESSING
	MOVB R1, [R0] 				; coloca padrão linha em R1
	ADD  R0, 1 
	MOVB R2, [R0] 				; coloca padrão coluna em R2
	MOV  R3, 0
	MOV  R4, 0

	CMP  R1, 0
	JZ   key_Convert_Return

	ADD  R4, 1 					; soma um à tecla final para contar com a tecla nula
 key_Convert_Line: 				; calcula a posicao da tecla na linha
 	SHR  R1, 1
 	JZ   key_Convert_Column
 	ADD  R3, 1
 	JMP  key_Convert_Line

 key_Convert_Column: 			; calcula a posicao da tecla na coluna
 	SHR  R2, 1
 	JZ   key_Convert_Return
 	ADD  R4, 1
 	JMP  key_Convert_Column

 key_Convert_Return: 	 		
 	MOV  R0, 4
 	MUL  R3, R0
 	ADD  R4, R3 				; calcula o valor da tecla de 0 a 10H (0 = nenhuma tecla, 1-10H = teclas 0 a F)

 	MOV  R0, KEY_PRESSING 	
 	MOV  [R0], R4 				; guarda a tecla convertida

 	POP  R4
 	POP  R3
 	POP  R2
 	POP  R1
 	POP  R0
 	RET
	
;Rotinas Miscellanious

score_Update:					; adiciona 3 pontos ao score, converte para decimal, e envia para os displays
	PUSH R0
	PUSH R1
	PUSH R2

	MOV  R0, SCORE_UPDATE 		
	MOV  R1, [R0]
	CMP  R1, TRUE
	JNZ  score_Update_Return	; verifica se a flag SCORE_UPDATE esta a 1(TRUE)

	MOV  R0, SCORE_HEXA
	MOV  R1, [R0]
	MOV  R2, SCORE_MAX
	CMP  R1, R2
	JGE  score_Update_Return    ; verifica se o score chegou ao limite da pontuacao(default = 99D ou 63H)

	ADD  R1, SCORE_INCREASE 	; aumenta o score por SCORE_HEXA pontos (default = 3)
	MOV  [R0], R1

	CALL hexToDec_Convert 		; converte para digitos decimais

	MOV  R0, DISPLAYS  			
	MOVB [R0], R1				; coloca a pontuacao em decimal nos displays

 score_Update_Return:
 	MOV  R0, SCORE_UPDATE
 	MOV  R1, FALSE
 	MOV  [R0], R1 				; consome a flag 

	POP  R2
	POP  R1
	POP  R0
	RET 			 

score_Reset: 					; reinicia o score
	PUSH R0
	PUSH R1
	MOV  R0, SCORE_HEXA
	MOV  R1, 0
	MOV  [R0], R1 				
	CALL display_Reset 			
	POP  R1
	POP  R0
	RET

display_Reset: 					; reinica os displays a 0
	PUSH R0
	PUSH R1
	MOV  R0, DISPLAYS
	MOV  R1, 0
	MOV  [R0], R1
	POP  R1
	POP  R0
	RET

var_Reset: 						; reinicia todas as variaveis do jogo
	PUSH R0
	PUSH R1
	PUSH R2

	MOV  R0, VAR_LIST 
 var_Reset_NextVar: 			; cicla pelas variaveis em VAR_LIST e coloca-as a 0(NULL or FALSE)
 	MOV  R1, [R0]
 	ADD  R0, 2 
 	MOV  R2, LIST_END
 	CMP  R2, R1 				
 	JZ   var_Reset_Return 		; se chegou ao fim da lista salta para fora do loop
 	MOV  R2, 0
 	MOV  [R1], R2
 	JMP  var_Reset_NextVar

 var_Reset_Return:
 	POP  R2
 	POP  R1
 	POP  R0
 	RET

random_Gen: 					; incrementa uma unidade numa variavel em memoria para criar um numero pseudo aleatorio
	PUSH R0
	PUSH R1

	MOV  R0, RANDOM_NUMBER
	MOV  R1, [R0]
	ADD  R1, 1
	MOV  [R0], R1

	POP  R1
	POP  R0
	RET 			

hexToDec_Convert: 				; converto numeros hexadecimais (até 63H) para decimal
	PUSH R0 					; converte o numero em R1, e deixa-o em R1
	PUSH R2

	MOV  R0, HEXTODEC_MODULE
	MOV  R2, R1
	DIV  R1, R0 				; coloca o algarismo das dezenas em decimal em R1
	MOD  R2, R0 				; coloca o algarismo das unidades em decimal em R2

	SHL  R1, 4
	OR   R1, R2					; coloca o numero em decimal em R1

	POP  R2
	POP  R0
	RET

;Rotinas do PixelScreen

image_Draw:                 	; pinta uma imagem no pixel screen
	PUSH R0 					; pinta a imagem cujo endereco na memoria esta em R2
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH R4
	PUSH R5
	PUSH R6
	PUSH R7
	PUSH R8
	PUSH R9

	MOVB R0, [R2] 				; coloca coord X do objeto em R0

	MOV  R1, 80H
	AND  R1, R0
	JZ   image_Draw_NoExtend
	MOV  R1, 0FF00H
	OR   R0, R1 				; estende o sinal do X

 image_Draw_NoExtend:
 	SUB  R0, 1
 	MOV  R7, R0 				; faz um backup de X em R7
	ADD  R2, 1

	MOVB R1, [R2] 				; coloca coord Y do objeto em R1
	ADD  R2, 1

	MOVB R3, [R2]			 	; coloca largura do objeto em R3 (em colunas)
	MOV  R8, R3 				; faz um backup da largura do objeto em R8
	ADD  R2, 1

	MOVB R4, [R2]				; coloca altura do objeto em R4 (em linhas)
	SUB  R2, 1

 image_Draw_NextLine:
 	CMP  R3, 0
 	JP  image_Draw_NextWord

 	MOV  R0, R7 				; reseta X inicial
 	MOV  R3, R8 				; reseta largura da imagem

 	ADD  R1, 1				 	; proxima linha
 	SUB  R4, 1				 	; contador das linhas
 	JZ   image_Draw_Return

 image_Draw_NextWord:
 	ADD  R2, 2 				 	; proxima word a desenhar
 	MOV  R9, [R2] 			
 	MOV  R6, IDr_WORD_MASC 	 	; reset da mascara

 image_Draw_Line:
 	MOV  R5, R9
 	CMP  R6, 0
 	JZ   image_Draw_NextLine

 	ADD  R0, 1 				 	; proxima coluna
 	AND  R5, R6 			 	; verificacao do bit(pixel), on or off
 	SHR  R6, 1 				 	; proximo bit a verificar
 	SUB  R3, 1				 	; contador das colunas
 	JN   image_Draw_NextLine
 	CMP  R5, 0 				    ; se bit esta a zero ent nao se pinta o respetivo pixel
 	JZ   image_Draw_Line
 	CALL pixel_Draw 		 	; pinta pixel em (R0, R1)
 	JMP  image_Draw_Line
	
 image_Draw_Return:
 	POP  R9
 	POP  R8
 	POP  R7
 	POP  R6
	POP  R5
	POP  R4
	POP  R3
	POP  R2
	POP  R1
	POP  R0
	RET

pixel_Draw:						; liga/desliga o pixel com coordenadas de 0 a 1F 
	PUSH R0 					; R0 - X /// R1 - Y
	PUSH R1                     ; se estiver ligado, desliga, e vice-versa
	PUSH R2 				
	PUSH R3

	MOV  R3, 80H 				; 
	AND  R3, R0 				;
	JNZ  pixel_Draw_Return 		;
 								; verifica se os pixeis a ser desenhados estao confinados ao pixel screen
	MOV  R3, 20H 				;
	CMP  R0, R3 				;
	JGE  pixel_Draw_Return      ;

	MOV  R2, R0
	MOV  R3, PDr_COL_MODULE
	MOD  R2, R3					; coloca posicao do bit na word em R2
	DIV  R0, R3             	; coloca coluna (0 ou 1) para word em R0

	MOV  R3, PDr_COL_MUL
	MUL  R0, R3					; calcula modificador da coluna 0 ou 2
	MOV  R3, PDr_LIN_MUL
	MUL  R1, R3   				; calcula modificador da linha (4 por linha)

	MOV  R3, PDr_WORD_MASC   	; inicializa a mascara

 pixel_Draw_Word:				; produz a word em R3 necessaria a colocar na celula
	CMP  R2, 0					; para ligar um pixel na posicao 0 a 15
	JZ   pixel_Draw_Fill
	SUB  R2, 1 	 				; passa para a proxima posicao na word
	SHR  R3, 1 				
	JMP  pixel_Draw_Word

 pixel_Draw_Fill:	
	MOV  R2, PIX_SCR 
	ADD  R0, R1
	ADD  R0, R2  				; calcula o endereco da celula a afetar para ligar o pixel pedido

	MOV  R2, [R0]          	 	; 
	XOR  R3, R2 				; Ligam o pixel sem desligar os adjacentes
	MOV  [R0], R3           	;

 pixel_Draw_Return:	
	POP  R3
	POP  R2
	POP  R1
	POP  R0
	RET

steer_Draw:                 	; limpa e pinta o volante de acordo com o valor de R2
	PUSH R0
	PUSH R1 
	PUSH R2 				
 		
 	MOV  R0, STEER_STATE
 	MOV  R1, [R0]
 	CMP  R1, 0
 	JZ   steer_Draw_Return 		; se o ultimo volante era 0 entao nao havia nenhum volante pintado

 	MOV  R0, R2
 	MOV  R2, R1
 	CALL image_Draw 			; apaga ultimo volante

 	MOV  R2, R0
 steer_Draw_Return:
 	MOV  R0, STEER_STATE
 	MOV  [R0], R2 				; atualiza ultimo volante

 	CALL image_Draw 			; pinta novo volante
 	POP  R2
	POP  R1
	POP  R0
	RET 

menu_Draw: 						; pinta o menu
	PUSH R2
	MOV  R2, MENU
	CALL image_Draw
	POP  R2
	RET

cockpit_Draw: 					; pinta o cockpit 
	PUSH R2
	MOV  R2, STEER_STOP
	CALL steer_Draw
	MOV  R2, SHIP
	CALL image_Draw
	POP  R2
	RET

pixelScreen_Reset:          	; limpa o ecra desde o topo ate a linha em R0
	PUSH R0
	PUSH R1
	PUSH R2

	MOV  R1, PIX_SCR
	MOV  R2, 0
 pixelScreen_Reset_Loop: 		; corre por todos os endereços do pixelscreen até à linha pretendida 
 	MOV  [R1], R2
 	ADD  R1, 2
 	MOV  [R1], R2
 	ADD  R1, 2 					; duas words limpas por linha para limpar os 32 pixeis(bits) de uma linha

 	CMP  R0, 0
 	JZ   pixelScreen_Reset_Return
 	SUB  R0, 1

 	JMP  pixelScreen_Reset_Loop
	
 pixelScreen_Reset_Return:
 	POP  R2
 	POP  R1
 	POP  R0
	RET 

;Rotinas dos meteoritos

meteor_Renew: 					; renova um meteorito em intrevalos aleatorios
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R3

	MOV  R0, RANDOM_RENEW 		;
	MOV  R1, [R0] 				; 
	CMP  R1, 0 					; 
	JZ   meteor_Renew_True 		; este bloco assegura que ha um intervalo pseudo aleatorio
	SUB  R1, 1 					; entre a criacao de novos meteoritos
	MOV  [R0], R1 				;
	JMP  meteor_Renew_Return 	;


 meteor_Renew_True:
	MOV  R0, METEOR_LIST

 meteor_Renew_Next: 			; procura um meteorito na lista que nao esteja inicializado para o renovar
 	MOV  R1, [R0]
 	ADD  R0, NEXT_WORD 			; proximo elemento da lista

 	MOV  R2, LIST_END		
 	CMP  R1, R2 				; testa pelo fim da lista
 	JZ   meteor_Renew_Return
 	MOV  R2, [R1] 				
 	CMP  R2, 0  				; verifica se o meteorito esta "desativado" (direcao a zero)
 	JNZ  meteor_Renew_Next

 	MOV  R0, RANDOM_NUMBER 		
 	MOV  R2, [R0]
 	MOV  R3, 3
 	MOD  R2, R3
 	SUB  R2, 1 					; gera direcao horizontal aleatoria entre -1, 0 ou 1

 	
 	SHL  R2, 8 					; coloca direcao horizontal no byte high
 	MOV  R3, 0001H
 	OR   R2, R3 				; direcao vertical no byte low
 	MOV  [R1], R2 				; coloca uma direcao aleatoria no meteorito

 	ADD  R1, 6 					; passa para a propriedade estado do meteorito
 	MOV  R2, [R0]
 	MOV  R3, METEOR_GOODTOBAD_RATIO
 	MOD  R2, R3
 	ADD  R2, 1
 	MOV  [R1], R2  				; coloca um estado, bom ou mau ao calhas no meteorito


 	MOV  R1, [R0] 				
 	MOV  R2, 3
 	MOD  R1, R2
 	ADD  R1, 5
 	MOV  R0, RANDOM_RENEW
 	MOV  [R0], R1 				; cria um novo intervalo para a renovacao do proximo meteorito entre 5 e 7
 
 meteor_Renew_Return:
	POP  R3
	POP  R2
	POP  R1
	POP  R0
	RET

meteor_Move: 					; atualiza a coordenada de cada meteorito na lista
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH R4

	MOV  R0, METEOR_MOD 		; modificador correspondente ao movimentoda nave (esquerda ou direita)
	MOV  R4, [R0]

	MOV  R1, METEOR_LIST    	; inicializa a lista de meteoritos

 meteor_Move_Next:
	MOV  R0, [R1] 				; coloca endereco do elemento (meteorito) em R0
	MOV  R2, LIST_END
	CMP  R0, R2 				; verifica se a lista chegou ao fim
	JZ   meteor_Move_Return
	ADD  R1, NEXT_WORD			; proximo elemento da lista

 	MOV  R2, [R0] 				; coloca direcao do meteorito em R2
 	CMP  R2, FALSE
 	JZ   meteor_Move_Next		; se a direcao é 0, o meteorito nao é movido
 	ADD  R0, 2
 	MOV  R3, [R0]				; coloca coordenada do meteorito em R3
 	ADD  R2, R3 				; soma direcao a coordenada
 	ADD  R2, R4					; acrescenta o modificador do movimento da nave
 	MOV  [R0], R2 				; regista na memoria do meteorito a nova coordenada
 
 	JMP  meteor_Move_Next

 meteor_Move_Return:
 	POP  R4
 	POP  R3	
	POP  R2
	POP  R1
	POP  R0
	RET

meteor_OutOffBounds: 			; verifica se um meteorito saiu dos limites do Pixel Screen
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R3

	MOV  R1, METEOR_LIST  		; inicializa a lista de meteoritos

 meteor_OutOffBounds_Next:
	MOV  R0, [R1] 				; coloca elemento da lista em R0
	MOV  R2, LIST_END 		
	CMP  R0, R2 				; verifica se a lista chegou fim
	JZ   meteor_OutOffBounds_Return
	ADD  R1, NEXT_WORD	 		; proximo elemento na lista

	ADD  R0, 2 			
	MOVB R2, [R0]				; coloca coordenada x do meteorito em R2

	MOV  R3, R2 				; backup do x para modificar
	ADD  R3, 5  				; soma da largura de um meteorito grande para calcular o x do pixel mais a direita do objeto
	MOV  R0, 80H
	AND  R3, R0					; sendo que as coordenadas sao guardadas em bytes, e o PEPE funciona em complemento para dois com 16bits
	JNZ meteor_OutOffBounds_Off ; temos de verificar se o "bit sinal" do byte esta a 1 para saber se a sua coordenada X e negativa

	MOV  R3, 0F0H
	CMP  R2, R3
	JGE  meteor_OutOffBounds_Next ; sendo que o x é guardado num byte, quando o objeto esta paracialmente fora do pixel screen para a esquerda a coordenada e maior que 32 em complemento para 2 com 16 bit
 								  ; logo temos de testar isso antes da proxima verificacao
	MOV  R3, 20H         		
	CMP  R2, R3 				; verifica se esta fora do pixel screen para a direita
	JGE  meteor_OutOffBounds_Off

	JMP  meteor_OutOffBounds_Next

 meteor_OutOffBounds_Off: 		; reinicia meteoritos fora do pixel screen
 	CALL meteor_Reset
 	JMP  meteor_OutOffBounds_Next

 meteor_OutOffBounds_Return:
	POP  R3
	POP  R2
	POP  R1
	POP  R0
	RET

meteor_Colision: 				; testa a colisao dos meteoritos com a nave
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH R4

	MOV  R1, METEOR_LIST

 meteor_Colision_Next:
	MOV  R0, [R1]
	MOV  R2, LIST_END 		 	; testa pelo fim da lista
	CMP  R0, R2
	JZ   meteor_Colision_Return
	ADD  R1, NEXT_WORD
 	
	ADD  R0, 3
	MOVB R2, [R0] 				; coloca oordenada Y do meteorito em R2
	ADD  R0, 1             
	MOV  R3, [R0] 			 	; coloca endereco do estado do meteorito em R3
	ADD  R3, 3
	MOVB R4, [R3]				; coloca altura do meteorito em R0
	
	ADD  R4, R2 	
	MOV  R3, 1BH
	CMP  R4, R3 				; verifica se o meteorito atingiu o painel de controlos da nave
	JP   meteor_Colision_Trigger

	JMP  meteor_Colision_Next

 meteor_Colision_Trigger:       ; houve colisao logo acabou o jogo ou ganhou-se 3 pontos
 	ADD R0, NEXT_WORD
 	MOV R2, [R0]
 	CMP R2, GOOD
 	JNZ  meteor_Colision_TriggerReset ; se nao for bom entao acabou o jogo

 	MOV R0, SCORE_UPDATE
 	MOV R2, TRUE
 	MOV [R0], R2 				; se for mau ent ganhamos 3 pontos (o main trata disso a partir desta flag)

 	CALL meteor_Reset 			; o meteorito e reiniciado
 	JMP  meteor_Colision_Return 

 meteor_Colision_TriggerReset:
 	CALL gameOver_Init 			; inicializa o gameover, o main trata do resto

 meteor_Colision_Return:
 	POP  R4
	POP  R3
	POP  R2
	POP  R1
	POP  R0
	RET

meteor_Change: 					; atualiza o estado(tamanho) dos meteoritos
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R3
	
	MOV  R1, METEOR_LIST 		

 meteor_Change_Next: 			; corre pela lista de meteoritos 
	MOV  R0, [R1]
	MOV  R2, LIST_END
	CMP  R0, R2
	JZ   meteor_Change_Return
	ADD  R1, NEXT_WORD

	ADD  R0, 3
	MOVB R3, [R0] 				; coloca coordenada Y do meteorito em R3
	ADD  R0, 3 					; prepara R0 para aceder a propriedade que contem o desenho

	
	MOV  R2, 02H 				; *verifica se a coordenada Y chegou a um certo valor para atualizar o tamanho
	CMP  R3, R2
	JZ   meteor_Change_toMedium

	MOV  R2, 05H 				; *
	CMP  R3, R2
	JZ   meteor_Change_toLarge
 	
	MOV  R2, 09H  				; *
	CMP  R3, R2
	JZ   meteor_Change_toHuge

	MOV  R2, 0EH  				; *
	CMP  R3, R2
	JZ   meteor_Change_toGiant

	JMP  meteor_Change_Next

 ; estes jumps escolhem os desenho consoante seja bom ou mau, a logica repete-se para todos a partir de toLarge

 meteor_Change_toMedium:
 	MOV R3, METEOR_MEDIUM
 	SUB R0, 2
 	MOV [R0], R3
 	JMP meteor_Change_Next


 meteor_Change_toLarge: 
 	MOV  R2, [R0]
 	SUB  R0, 2
 	CMP  R2, GOOD
 	JZ   meteor_Change_toLarge_Good ; verifica se e bom

  	MOV R3, BAD_METEOR_LARGE
 	MOV [R0], R3 				; se for mau colocamos o desenho mau na memoria
 	JMP meteor_Change_Next 

 meteor_Change_toLarge_Good: 	; se for bom colocamos o desenho bom na memoria
 	MOV R3, GOOD_METEOR_LARGE
 	MOV [R0], R3
 	JMP meteor_Change_Next

 
 meteor_Change_toHuge:
 	MOV  R2, [R0]
 	SUB  R0, 2
 	CMP  R2, GOOD
 	JZ   meteor_Change_toHuge_Good

 	MOV R3, BAD_METEOR_HUGE
 	MOV [R0], R3
 	JMP meteor_Change_Next

 meteor_Change_toHuge_Good:
 	MOV R3, GOOD_METEOR_HUGE
 	MOV [R0], R3
 	JMP meteor_Change_Next

 
 meteor_Change_toGiant:
 	MOV  R2, [R0]
 	SUB  R0, 2
 	CMP  R2, GOOD
 	JZ   meteor_Change_toGiant_Good

 	MOV R3, BAD_METEOR_GIANT
 	MOV [R0], R3
 	JMP meteor_Change_Next

 meteor_Change_toGiant_Good:
 	MOV R3, GOOD_METEOR_GIANT
 	MOV [R0], R3
 	JMP meteor_Change_Next


 meteor_Change_Return:
 	POP  R3
	POP  R2
	POP  R1
	POP  R0
	RET

meteor_Update: 					; atualiza no Pixel Screen todos os meteoritos da lista
	PUSH R0
	PUSH R1
	PUSH R2

	MOV  R1, METEOR_LIST

 meteor_Update_Next: 			; cicla por todos os meteoritos
	MOV  R0, [R1]
	ADD  R1, NEXT_WORD
	MOV  R2, LIST_END
	CMP  R0, R2
	JZ   meteor_Update_Return 	; fim da lista	 
	MOV  R2, [R0]
	CMP  R2, NULL 				; verifica se a direcao e nula, ou seja se o meteorito esta OFF
	JZ   meteor_Update_Next
	CALL meteor_SingleUpdate 	; pinta o meteorito
	JMP  meteor_Update_Next

 meteor_Update_Return:
	POP  R2
	POP  R1
	POP  R0
	RET

meteor_SingleUpdate: 			; atualiza do Pixel Screen um meteorito cujo endereco esta em R0
	PUSH R0
	PUSH R1
	PUSH R2

	ADD  R0, NEXT_WORD 			
	MOV  R1, [R0] 				; poe coordenada em R1
	ADD  R0, NEXT_WORD
	MOV  R2, [R0] 				; poe desenho em R2
	MOV  [R2], R1 				; injeta coordenada no desenho
	CALL image_Draw 			; pinta meteorito

 meteor_SingleUpdate_Return:
	POP  R2
	POP  R1
	POP  R0
	RET

meteor_Reset: 			 		; reinicia o meteorito cujo endereco corresponde ao endereco na lista de meteoritos em R1
	PUSH R0
	PUSH R1
	
	SUB  R1, NEXT_WORD
	MOV  R0, [R1] 				; põe em R0 e endereco do meteorito a ser reiniciado
	MOV  R1, NULL
 	MOV  [R0], R1 				; reinicia a direcao
 	
 	ADD  R0, NEXT_WORD
 	MOV  R1, METEOR_ST			; reinicia a coordenada inicial
 	MOV  [R0], R1

 	ADD  R0, NEXT_WORD			; reinicia o estado
 	MOV  R1, METEOR_SMALL
 	MOV  [R0], R1

 meteor_Reset_Return:
	POP  R1
	POP  R0
	RET

meteor_FullReset: 				; reinicia todos os meteoritos
	PUSH R0
	PUSH R1
	PUSH R2

 	MOV  R1, METEOR_LIST 
 meteor_FullReset_Next: 		; cicla pelos meteoritos em METEOR_LIST e reinicia-os
	MOV  R0, [R1]
	ADD  R1, NEXT_WORD 			; proximo elemento
	MOV  R2, LIST_END
	CMP  R0, R2 				; fim da lista
	JZ   meteor_FullReset_Return
	CALL meteor_Reset 			; reseta o meteorito
	JMP  meteor_FullReset_Next

 meteor_FullReset_Return:
 	POP  R2
 	POP  R1
 	POP  R0
 	RET

meteor_Exploded: 				; limpa o ecra de meteoritos explodidos e reseta-os, logo eles só se manteem no ecrã por um instante (1 interrupção)
	PUSH R0
	PUSH R1
	PUSH R2

	MOV  R1, METEOR_LIST
 meteor_Exploded_Next: 			; cicla por todos os meteoritos
 	MOV  R0, [R1]
 	ADD  R1, NEXT_WORD
 	MOV  R2, LIST_END
 	CMP  R0, R2 				; fim da lista
 	JZ   meteor_Exploded_Return

 	MOV  R2, [R0]
 	CMP  R2, NULL
 	JNZ  meteor_Exploded_Next 	; verifica se a direcao é zero

 	ADD  R0, 4
 	MOV  R2, [R0]
 	MOV  R3, METEOR_EXPLODE
 	CMP  R2, R3 
 	JNZ  meteor_Exploded_Next 	; verifica se o meteorito está explodido, ou seja, a sua imagem é METEOR_EXPLODE

 	SUB  R0, 4

 	CALL meteor_SingleUpdate 	; apaga o meteorito
 	CALL meteor_Reset 			; reinicia o meteorito

 	JMP  meteor_Exploded_Next 	; proximo meteorito

 meteor_Exploded_Return:
	POP  R2
	POP  R1
	POP  R0
	RET


;Rotinas dos Misseis

missile_Move: 					; atualiza as coordenadas dos misseis ativos na memoria
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH R4

	MOV  R1, MISSILE_LIST 		; inicializa a lista de misseis
 missile_Move_Next:
	MOV  R0, [R1] 				; busca endereço do missil
	ADD  R1, NEXT_WORD
	MOV  R2, LIST_END
	CMP  R0, R2 				; verifica se a lista chegou ao fim
	JZ   missile_Move_Return

	MOV  R2, [R0]
	CMP  R2, FALSE 				; verifica se a direcao é 0, ou seja, "nao existe"
	JZ   missile_Move_Next

	ADD  R0, NEXT_WORD 			
	MOV  R3, [R0]
	ADD  R2, R3 				; soma direção e coordenada obtendo nova coordenada e atualiza-a na memoria
	MOV  R3, METEOR_MOD
	MOV  R4, [R3]
	ADD  R2, R4 				; soma nova coordenada ao movimento relativo da nave
	MOV  [R0], R2 				; atualiza coordenada na memoria

	JMP  missile_Move_Next

 missile_Move_Return:

 	POP  R4
	POP  R3
	POP  R2
	POP  R1
	POP  R0
	RET

missile_OutOffBounds: 			; verifica se o missil atingiu a sua altura maxima, e reinicia-o
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R3

	MOV  R2, MISSILE_LIST 		; inicializa a lista de misseis
 missile_OutOffBounds_Next:
	MOV  R0, [R2] 				; busca endereço do missil
	ADD  R2, NEXT_WORD 
	MOV  R1, LIST_END 			
	CMP  R0, R1 				; verifica se a lista chegou ao fim
	JZ   missile_OutOffBounds_Return
	MOV  R1, [R0]
	CMP  R1, FALSE 				; verifica se o missil tem direção 0 ou seja, "nao existe"
	JZ   missile_OutOffBounds_Next

	ADD  R0, 3
	MOVB R1, [R0] 				; coloca coordenada Y do missil em R1

	MOV  R3, MISSILE_MAXHEIGHT
	CMP  R1, R3
	JGT  missile_OutOffBounds_Next ; verifica se o missil atingiu o limite de 12 pixeis
	
	CALL missile_Reset

	JMP  missile_OutOffBounds_Next

 missile_OutOffBounds_Return:
 	POP  R3
 	POP  R2
 	POP  R1
 	POP  R0
 	RET

missile_Renew: 					; renova um missil de acordo com o valor de CREATE_MISSILE e NEXT_MISSILE
	PUSH R0
	PUSH R1

	MOV  R0, CREATE_MISSILE
	MOV  R1, [R0]
	CMP  R1, FALSE
	JZ missile_Renew_Return 	; verifica se a flag CREATE_MISSILE esta a 0(FALSE)
 
	MOV  R0, NEXT_MISSILE
	MOV  R1, [R0] 				

	ADD  R1, 1
	MOV  R0, 2
	MUL  R1, R0
	MOV  R0, MISSILE_1
	ADD  R1, R0 				; calcula endereço do missil a renovar baseado no valor de NEXT_MISSILE, -1 para missil da esquerda, 1 para missil da direita

	MOV  R0, [R1]
	CMP  R0, FALSE
	JNZ  missile_Renew_FlagOff

	MOV  R0, 0FFFFH
	MOV  [R1], R0				; coloca a direcao do missil a -1

	ADD  R0, 3
	MOV  R1, 1BH
	MOV  [R0], R1  				; coloca a coordenada Y do missil 0

	MOV  R0, NEXT_MISSILE
	MOV  R1, [R0]
	MOV  R0, 0FFFFH
	MUL  R1, R0
	MOV  R0, NEXT_MISSILE
	MOV  [R0], R1 				; troca a variavel NEXT_MISSILE de -1 para 1 e vice-versa

 missile_Renew_FlagOff:
	MOV  R0, CREATE_MISSILE
	MOV  R1, FALSE
	MOV  [R0], R1 				; coloca a flag CREATE_MISSILE a 0(FALSE)

 missile_Renew_Return:
	POP  R1
	POP  R0
	RET

missile_FullColision: 			; testa as colisoes de ambos os misseis com todos os meteoritos
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH R4
	PUSH R8

	
	MOV  R2, MISSILE_LIST
 missile_FullColision_NextMissile: 	; inicializa a lista de misseis
	MOV  R3, [R2]
	ADD  R2, NEXT_WORD
	MOV  R4, LIST_END
	CMP  R3, R4
	JZ   missile_FullColision_Return

	MOV  R1, METEOR_LIST
 missile_FullColision_NextMeteor: 	; inicializa a lista de meteoritos
 	MOV  R0, [R1]
 	ADD  R1, NEXT_WORD
 	MOV  R4, LIST_END
 	CMP  R0, R4
 	JZ   missile_FullColision_NextMissile 	; proximo missil

 	CALL missile_Colision 			; testa se o par meteorito missil estão a colidir

 	CMP  R8, TRUE 
 	JNZ  missile_FullColision_NextMeteor 

 	CALL meteor_SingleUpdate 		; se sim então apaga o meteorito
 	CALL missile_ExplodeMeteor 		; chama rotina responsavel por iniciar o processo da degradação dos meteoritos
 	CALL missile_Reset 				; reinicia o missil
 	
 	JMP  missile_FullColision_NextMeteor ; proximo meteoro

 missile_FullColision_Return:
	POP  R8
	POP  R4
	POP  R3
	POP  R2
	POP  R1
	POP  R0
	RET

missile_Update: 				; pinta no pixel screen os misseis ativos
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R3

	MOV  R1, MISSILE_LIST 		; inicializa a lista de meteoritos
 missile_Update_Next:
 	MOV  R0, [R1] 				; coloca endereço do missil em R0
 	ADD  R1, NEXT_WORD
 	MOV  R2, LIST_END
 	CMP  R0, R2 
 	JZ   missile_Update_Return  ; verifica se chegou ao fim da lista

 	MOV  R2, [R0]
 	CMP  R2, NULL
 	JZ   missile_Update_Next 	; verifica se a direção é nula, ou seja, nao existe

 	ADD  R0, NEXT_WORD
 	MOV  R3, [R0]
 	MOV  R2, MISSILE_DRAW
 	MOV  [R2], R3
 	CALL image_Draw 			; pinta o missil nas suas coordenadas 

 	JMP  missile_Update_Next

 missile_Update_Return:
	POP  R3
	POP  R2
	POP  R1
	POP  R0
	RET

missile_Reset: 					; reinicia o missil cujo endereço na lista de misseis está em R2
	PUSH R0
	PUSH R2

	SUB  R2, NEXT_WORD
	MOV  R0, [R2]
	MOV  R2, NULL
	MOV  [R0], R2 				; nulifica a direcao do missil, ou seja, ja nao existe

	MOV  R2, MISSILE_1
	CMP  R0, R2
	JNZ  missile_Reset_2 		; verifica qual missil estamos a reiniciar pois as coordenadas iniciais sao diferentes
 
	MOV  R2, MISSILE_1_ST
	JMP  missile_Reset_Coord 	

 missile_Reset_2:
  	MOV  R2, MISSILE_2_ST

 missile_Reset_Coord:
	ADD  R0, NEXT_WORD
	MOV  [R0], R2 				; retoma a altura inicial

 missile_Reset_Return:
 	POP  R2
	POP  R0
	RET

missile_FullReset: 				; reinicia todos os misseis
	PUSH R0
	PUSH R1
	PUSH R2

 	MOV  R2, MISSILE_LIST

 missile_FullReset_Next: 		; corre pela lista de misseis
 	MOV  R0, [R2]
 	ADD  R2, NEXT_WORD
 	MOV  R1, LIST_END
 	CMP  R0, R1 				; fim da lista
 	JZ   missile_FullReset_Return
 	CALL missile_Reset 			; reseta missil a missil pela lista
 	JMP  missile_FullReset_Next
 missile_FullReset_Return:
 	POP  R2
 	POP  R1
 	POP  R0
 	RET

missile_Colision: 				; R0 endereco do meteorito - R3 endereco do missil 
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH R4
	PUSH R5
	PUSH R6
	PUSH R7

	MOV  R8, 0

	ADD  R0, NEXT_WORD
	MOVB R1, [R0] 				; coordenada x do meteorito
	ADD  R0, 1
	MOVB R2, [R0] 				; coordenada y do meteorito
	ADD  R0, 1
	MOV  R4, [R0] 				; desenho do meteorito
	ADD  R4, NEXT_WORD

	MOVB R5, [R4] 				; largura do meteorito
	ADD  R5, R1
	SUB  R5, 1 					; coordenada x mais a esquerda do meteorito

	ADD  R4, 1
	MOVB R6, [R4] 				; altura do meteorito
	ADD  R6, R2
	SUB  R6, 1 					; coordenada y mais a baixo do meteorito

	ADD  R3, NEXT_WORD
	MOVB R4, [R3] 				; coordenada x do missil
	ADD  R3, 1
	MOVB R7, [R3] 				; coordenada y do missil

	CMP  R4, R1
	JLT  missile_Colision_Return

	CMP  R4, R5
	JGT  missile_Colision_Return

	CMP  R7, R2
	JLT  missile_Colision_Return

	CMP  R7, R6 	
	JGT  missile_Colision_Return 	; testam se o missil esta dentro da "caixa" em que o meteorito se insere

	MOV  R8, 1

 missile_Colision_Return:
 	POP  R7
 	POP  R6
 	POP  R5
 	POP  R4
	POP  R3
	POP  R2
	POP  R1
	POP  R0
	RET

missile_ExplodeMeteor: 			; sendo R0 o endereço de um meteorito, prepara-o para o ciclo de degradaçao
	PUSH R0
	PUSH R1
	PUSH R2
 
 	MOV  R1, 0
 	MOV  [R0], R1 				; direcao a zero para nao andar mais

 	ADD  R0, 4
 	MOV  R1, METEOR_EXPLODE
 	MOV  [R0], R1 				; muda o desenho para a imagem de um meteorito explodido

 	ADD  R0, NEXT_WORD
 	MOV  R1, 0
 	MOV  [R0], R1 				; muda o estado (arbitrario) para 0
 	
 	SUB  R0, 6
 	CALL meteor_SingleUpdate 	; pinta o meteorito com as novas propriedades
 missile_ExplodeMeteor_Return:
	POP  R2
	POP  R1
	POP  R0
	RET


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Rotinas de interrupcao
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

int_MoveMeteor: 				; poe a flag MOVE_METEOR a 1(TRUE)
	PUSH R0
	PUSH R1
	MOV  R0, MOVE_METEOR
	MOV  R1, TRUE
	MOV  [R0], R1
	POP  R1
	POP  R0
	RFE

int_MoveMissile: 				; poe a flag MOVE_MISSILE a 1(TRUE)
	PUSH R0
	PUSH R1
	MOV  R0, MOVE_MISSILE
	MOV  R1, TRUE
	MOV  [R0], R1
	POP  R1
	POP  R0
	RFE 
