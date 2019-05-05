%-----------------------------------------------------------------------------
% Projeto LP
% Joao Oliveira 
% tg93728
%-----------------------------------------------------------------------------

%-----------------------------------------------------------------------------
%  		----MODULES----
%-----------------------------------------------------------------------------
:-[codigo_comum].

%-----------------------------------------------------------------------------
%  		----UTILITIES----
%-----------------------------------------------------------------------------

%-----------------------------------------------------------------------------
% castList(Lin, Lout). copia todas os elementos de Lin para Lout
% sem perder a ordem de modo a que as variaveis em Lin nao unifiquem 
% em conjunto com as de Lout
%-----------------------------------------------------------------------------
castList([],[]).

castList([H|T],[H|Res]):-
	nonvar(H), !,
	castList(T,Res).

castList([H|T],[_|Res]):-
	var(H),
	castList(T,Res).

%-----------------------------------------------------------------------------
% fillList(Lin,Lout,E) e verdadeiro se Lout for o resultado de trocar
% todas as variaveis em Lin por E
%-----------------------------------------------------------------------------
fillList([],[],_).

fillList([H|T],[H|Res],E):-
	nonvar(H), !,
	fillList(T,Res,E).

fillList([H|T],[E|Res],E):-
	var(H),
	fillList(T,Res,E).

%-----------------------------------------------------------------------------
% ocurList(Lin,N,E). e verdadeiro se N_1 tiver o numero de ocurrencias
% do elemento E em Lin
%-----------------------------------------------------------------------------
ocurList([],0,_).

ocurList([H|T],N,E):-
	H \== E, !,
	ocurList(T,N,E).

ocurList([H|T],N,E):-
	H == E,
	ocurList(T,N1,E),
	N is N1 + 1.

%-----------------------------------------------------------------------------
% partitionList(Lin,N,V). e verdadeiro se N contem soma de 
% todas as constantes em Lin e V o numero de variaveis em Lin
%-----------------------------------------------------------------------------
partitionList([],0,0).

partitionList([H|T],N,V):-
	nonvar(H), !,
	partitionList(T,N1,V),
	N is N1+H.

partitionList([H|T],N,V):-
	var(H),
	partitionList(T,N,V1),
	V is V1+1.

%-----------------------------------------------------------------------------
% diff(Lin,Lcheck,Lout). e verdadeiro se Lout contem as posicoes em que
% Lin difere de Lcheck, comecando da posicao 1
%-----------------------------------------------------------------------------
diff(Lin,Lcheck,Lout):-
	diff(Lin,Lcheck,Lout,1).

diff([],[],[],_).

diff([H1|T1],[H2|T2],Lout,N):-
	H1 == H2, !,
	N1 is N + 1,
	diff(T1,T2,Lout,N1).

diff([H1|T1],[H2|T2],[N|Res],N):-
	H1 \== H2,
	N1 is N + 1,
	diff(T1,T2,Res,N1).
	
%-----------------------------------------------------------------------------
% memberCustom(Lin,Lcheck). e verdadeiro se Lin pertence a Lcheck, 
% nao unificando
%-----------------------------------------------------------------------------
memberCustom(L,[H|_]):-
	L == H, !.

memberCustom(L,[_|T]):-
	memberCustom(L,T).


%-----------------------------------------------------------------------------
%  		----REGRA 1----
%-----------------------------------------------------------------------------

%-----------------------------------------------------------------------------
% inverte(E1,E2). e verdadeiro se E1 for o inverso de E2 (0 <-> 1)
%-----------------------------------------------------------------------------
inverte(1,0).
inverte(0,1).

%-----------------------------------------------------------------------------
% triploValido(Lin). e verdadeiro se Lin respeita a regra 1 (Unifica)
%-----------------------------------------------------------------------------
triploValido([E,E,R]):- inverte(E,R), !.
triploValido([E,R,E]):- inverte(E,R), !.
triploValido([R,E,E]):- inverte(E,R), !.

%-----------------------------------------------------------------------------
% aplica_R1_triplo(Lin,Lout). e verdadeiro se Lout representar a aplicacao
% da regra 1 a Lin
%-----------------------------------------------------------------------------
aplica_R1_triplo(Lin,Lout):-
	partitionList(Lin,N,V),
	(V >= 2 -> Lout = Lin
		;
		(V == 0 -> triploValido(Lin), Lout = Lin
			;
			(N == 1 -> Lout = Lin % Sobra V == 1
				; 
				castList(Lin,Lout), triploValido(Lout)))).

%-----------------------------------------------------------------------------
% aplica_R1_fila_aux(Lin,Lout). e verdadeiro se Lout for o resultado de
% aplicar a regra 1 a Lin (1 passagem)
%-----------------------------------------------------------------------------
aplica_R1_fila_aux([E1,E2],[E1,E2]).

aplica_R1_fila_aux([E1,E2,E3|T1],[O1|T2]):-
	aplica_R1_triplo([E1,E2,E3],[O1,O2,O3]),
	aplica_R1_fila_aux([O2,O3|T1],T2).

%-----------------------------------------------------------------------------
% aplica_R1_fila(Lin,Lout). e verdadeiro se Lout for o resultado de aplicar
% a regra 1 ate esta nao surtir efeito
%-----------------------------------------------------------------------------
aplica_R1_fila(Lin, Lout):-
	aplica_R1_fila_aux(Lin, Ltmp),
	(Lin \== Ltmp -> aplica_R1_fila(Ltmp,Lout)
		;
		Lout = Lin).


%-----------------------------------------------------------------------------
%  		----REGRA 2----
%-----------------------------------------------------------------------------

%-----------------------------------------------------------------------------
% aplica_R2_fila(Lin,Lout). e verdadeiro se Lout for o resultado de aplicar
% a regra 2 em Lin
%-----------------------------------------------------------------------------
aplica_R2_fila(Lin,Lout):-
	ocurList(Lin,N_0,0),
	ocurList(Lin,N_1,1),
	length(Lin,N_t),
	N_max is div(N_t,2),
	(N_0 == N_max -> fillList(Lin,Lout,1)
		;
		(N_1 == N_max -> fillList(Lin,Lout,0)
			;
			(N_0 < N_max, N_1 < N_max -> Lout = Lin))).
	

%-----------------------------------------------------------------------------
% aplica_R1_R2_fila(Lin,Lout). e verdadeiro se Lout for o resultado de aplicar
% a regras 1 e 2 a Lin.
%-----------------------------------------------------------------------------
aplica_R1_R2_fila(Lin,Lout):-
	aplica_R1_fila(Lin,Ltmp),
	aplica_R2_fila(Ltmp,Lout).

%-----------------------------------------------------------------------------
% aplica_R1_R2_linhas(PuzIn,PuzOut). e verdadeiro se PuzOut for o resultado de 
% aplicar as regras 1 e 2 as linhas de PuzIn
%-----------------------------------------------------------------------------
aplica_R1_R2_linhas([],[]).

aplica_R1_R2_linhas([Lin|T],[Lout|Res]):-
	aplica_R1_R2_fila(Lin,Lout),
	aplica_R1_R2_linhas(T,Res). 

%-----------------------------------------------------------------------------
% aplica_R1_R2_puzzle(PuzIn,PuzOut). e verdadeiro se PuzOut for o resultado de aplicar
% as regras 1 e 2 a todas as linhas e colunas de PuzIn
%-----------------------------------------------------------------------------
aplica_R1_R2_puzzle(PuzIn,PuzOut):-
	aplica_R1_R2_linhas(PuzIn,PuzTmp1),
	mat_transposta(PuzTmp1,PuzTmp2),
	aplica_R1_R2_linhas(PuzTmp2,PuzTmp3),
	mat_transposta(PuzTmp3,PuzOut).


%-----------------------------------------------------------------------------
%  		----REGRA 3----
%-----------------------------------------------------------------------------

%-----------------------------------------------------------------------------
% verifica_R3_Listas(PuzIn). e verdadeiro se as linhas de PuzIn respeitarem a regra 3
%-----------------------------------------------------------------------------
verifica_R3_Listas([_]):- !.

verifica_R3_Listas([Lin|T]):- 
	\+ memberCustom(Lin,T),
	verifica_R3_Listas(T).

%-----------------------------------------------------------------------------
% verifica_R3(PuzIn). e verdadeiro se PuzIn respeita a regra 3, tanto as colunas
% como as linhas
%-----------------------------------------------------------------------------
verifica_R3(Puz):-
	verifica_R3_Listas(Puz),
	mat_transposta(Puz,PuzTmp),
	verifica_R3_Listas(PuzTmp).


%-----------------------------------------------------------------------------
%  		----SOLUCAO----
%-----------------------------------------------------------------------------

%-----------------------------------------------------------------------------
% inicializa(PuzIn,PuzOut). e verdadeiro se PuzOut for o resultado de aplicar
% as regras 1 e 2 a todas as linhas e colunas de PuzIn ate nao surtir efeito
%-----------------------------------------------------------------------------
inicializa(PuzIn,PuzOut):-
	aplica_R1_R2_puzzle(PuzIn,PuzTmp),
	(PuzIn \== PuzTmp -> inicializa(PuzTmp,PuzOut)
		;
		PuzOut = PuzTmp).


%-----------------------------------------------------------------------------
% propaga_linha(PuzIn,L,PuzOut,PosDif). e verdadeiro se PuzOut for o resuldado
% de propagar a linha L de PuzIn, e PosDif seja uma lista de posicoes onde
% houve unificacao de variaveis
%-----------------------------------------------------------------------------
propaga_linha(PuzIn,L,PuzOut,PosDif):-
	nth1(L,PuzIn,Lin),
	aplica_R1_R2_fila(Lin,Lout),
	mat_muda_linha(PuzIn,L,Lout,PuzOut),

	diff(Lin,Lout,Ind),
	findall((L,C),(member(C,Ind)),PosDif).
	
%-----------------------------------------------------------------------------
% propaga_coluna(PuzIn,C,PuzOut,PosDif). e verdadeiro se PuzOut for o resuldado
% de propagar a coluna C de PuzIn, e PosDif seja uma lista de posicoes onde
% houve unificacao de variaveis
%-----------------------------------------------------------------------------
propaga_coluna(PuzIn,C,PuzOut,PosDif):- 
	mat_elementos_coluna(PuzIn,C,Cin),
	aplica_R1_R2_fila(Cin,Cout),
	mat_muda_coluna(PuzIn,C,Cout,PuzOut),

	diff(Cin,Cout,Ind),
	findall((L,C),(member(L,Ind)),PosDif).

%-----------------------------------------------------------------------------
% getVar(Puz,Pos). utiliza o polimorfismo do prolog para que Pos seja a 
% primeiro variavel em Puz
%-----------------------------------------------------------------------------
getVar(Puz,Pos):-
	mat_ref(Puz,Pos,E),
	var(E), !.

%-----------------------------------------------------------------------------
% propaga_posicoes(Lin,PuzIn,PuzOut). e verdadeiro se PuzOut for o resultado
% de propagar as regras 1 e 2 em todas as posicoes em Lin, propagando
% todas as coordenadas onde houver unificacao a seguir
%-----------------------------------------------------------------------------
propaga_posicoes([],PuzIn,PuzIn).
propaga_posicoes([(L,C)|Res],PuzIn,PuzOut):-
	propaga_linha(PuzIn,L,PuzTmp,Pos1),
	propaga_coluna(PuzTmp,C,PuzTmp1,Pos2),
	append(Pos1,Pos2,Pos3),
	append(Pos3,Res,Pos_All),
	propaga_posicoes(Pos_All,PuzTmp1,PuzOut).
	
%-----------------------------------------------------------------------------
% resolve_testa(PuzIn,PuzOut). e verdadeiro se PuzOut for o resultado de
% aplicar propagar_posicoes a PuzIn em que o elemento em Pos foi trocado
% para 1 ou 0.
%-----------------------------------------------------------------------------
resolve_testa(PuzIn,PuzOut,Pos):-
	mat_muda_posicao(PuzIn,Pos,0,PuzTmp),
	propaga_posicoes([Pos],PuzTmp,PuzOut).

resolve_testa(PuzIn,PuzOut,Pos):-
	mat_muda_posicao(PuzIn,Pos,1,PuzTmp),
	propaga_posicoes([Pos],PuzTmp,PuzOut).

%-----------------------------------------------------------------------------
% resolve(Puz,Sol). e verdadeiro se Sol for solucao de Puz
%-----------------------------------------------------------------------------
resolve(Puz,Sol):-
	inicializa(Puz,PuzInit),
	resolve_aux(PuzInit,Sol).

resolve_aux(Puz,Sol):-
	(getVar(Puz,Pos) -> resolve_testa(Puz,PuzTmp,Pos), resolve_aux(PuzTmp,Sol)
			; Sol = Puz).