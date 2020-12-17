#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <values.h>

/*Macros para ajudar o accesso a eventos*/
#define Sala(I) salas_array[I-1]
#define Evento(I,E) Sala(I).eventos[E]

/*Template da representacao de eventos*/
#define EV_REPR 				"%s %02d%02d%04d %02d%02d %d Sala%d %s\n*"
/*Templates de erros*/
#define ER_SALAOCUPADA      	"Impossivel agendar evento %s. Sala%d ocupada.\n"
#define ER_PESSOAOCUPADA		"Impossivel agendar evento %s. Participante %s tem um evento sobreposto.\n"
#define ER_INEXESTENTE      	"Evento %s inexistente.\n"
#define ER_CHEIO  				"Impossivel adicionar participante. Evento %s ja tem %d participantes.\n"
#define ER_SOBREPOSTO       	"Impossivel adicionar participante. Participante %s tem um evento sobreposto.\n"
#define ER_VAZIA   				"Impossivel remover participante. Participante %s e o unico participante no evento %s.\n"

#define STR_MAXSIZE 64 				/*Tamanho maximo de um string*/
#define PART_MAX 4					/*Numero de participantes + responsavel*/
#define TOKENS_MAX 5+PART_MAX 		/*Numero de tokens*/

#define SALAS_MAX 10 				/*Numero de salas*/
#define EVENTOS_MAX 100 			/*Numero de eventos por sala*/

#define PARAM_DELIM_FIRST " \n"     /*Delimitadores do strtok para o comando*/
#define PARAM_DELIM_ALL ":\n" 		/*Delimitadores do strtok a partir da descricao*/
#define PARAM_MAX 400 				/*Tamanho maximo de um comando, apontado para cima*/

typedef struct {
	char desc[STR_MAXSIZE], part[PART_MAX][STR_MAXSIZE];
	int  dur, sala;
	long epoch;
} Evento;

typedef struct {
	Evento eventos[EVENTOS_MAX];
	int num, curr;
} Sala;

/*Array de Salas, global para facil acesso a partir de qualquer funcao*/
Sala salas_array[SALAS_MAX]; 		

/* int --> void
 * Recebe um numero de 1 a 10 e inicializa uma sala na posicao correta da array de salas
*/
void criarSala(int n) {
	Sala tmp;
	tmp.num = n;
	tmp.curr = 0;
	Sala(n) = tmp;
}

/* void --> void
 * Inicializa todas as salas
*/
void initSalas() {
	int x;
	for(x=1; x<=SALAS_MAX; x++)
		criarSala(x);
}

/* Evento --> void
 * Recebe um evento e coloca-o na posicao certa de modo a preservar
 * a ordem crescente dos incios dos eventos
*/
void adicionarEvento(Evento evento) {
	int x, s = evento.sala;
	for(x=Sala(s).curr; x>0; x--)
		if(Evento(s,x-1).epoch < evento.epoch)
			break;
		else
			Evento(s,x) = Evento(s,x-1);
	Evento(s,x) = evento;	
	Sala(s).curr++; 
}

/* long --> int*
 * Recebe um numero com os minutos correspondentes ao dia e hora
 * e converte para uma array de inteiros, retorna o pointer para essa array
*/
int *epochToDate(long tmp) {
	static int tmpArray[5];   	/*Array com cada unidade de tempo*/
	tmpArray[0] = tmp % 60; 	/*Minutos*/
	tmp /= 60;
	tmpArray[1] = tmp % 24; 	/*Horas*/
	tmp /= 24;
	tmpArray[2] = tmp % 31 +1;  /*Dias*/
	tmp /= 31;
	tmpArray[3] = tmp % 12 +1;  /*Meses*/
	tmpArray[4] = tmp / 12; 	/*Anos*/
	return tmpArray;
} 

/* char[], char[] --> long
 * Recebe dois strings, uma data e hora e retorna um long que corresponde
 * aos minutos que passaram desde 01012019
 * Assume que cada mes tem 31 dias porque nao afeta o objetivo de comparar inicios
 * e facilita as contas 
*/ 
long dateToEpoch(char str_d[], char str_t[]) {
	long int date = strtol(str_d,NULL,10), time = strtol(str_t,NULL,10), tmp = 0;
	tmp += time % 100; 							/*Minutos*/
	tmp += (time / 100) * 60; 					/*Horas*/
	tmp += (date / 1000000 -1) * 1440;			/*Dias*/
	tmp += ((date / 10000) % 100 -1) * 44640;	/*Meses*/
	tmp += (date % 10000) * 535680; 			/*Anos*/
	return tmp;
}

/* Evento --> void
 * Representacao de um evento
*/
void eventoRepr(Evento evento) {
	int x, *tmpArray = epochToDate(evento.epoch);
	printf(EV_REPR,evento.desc, tmpArray[2], tmpArray[3], tmpArray[4], 
			       tmpArray[1], tmpArray[0], 
		       	   evento.dur, evento.sala, evento.part[0]);
	/*Printa todos os participantes que existem neste evento*/ 
	for(x=1; x<PART_MAX && evento.part[x][0] != '\0'; x++)
		printf(" %s",evento.part[x]);
	printf("\n");
}

/* Evento, Evento --> int
 * Testa se os eventos que recebe fazem overlap um com o outro, 
 * ou seja, se acontecem ao mesmo tempo
 * 1- Ha colisao
 * 0- Nao ha colisao
*/
int eventoTempoColisao(Evento e_1, Evento e_2) {
	int e1_ini = e_1.epoch, e1_fim = e1_ini + e_1.dur;
	int e2_ini = e_2.epoch, e2_fim = e2_ini + e_2.dur;
	if(e1_fim > e2_ini && e1_fim <= e2_fim)
		return 1;
	if(e1_ini >= e2_ini && e1_ini < e2_fim)
		return 1;
	if(e1_ini <= e2_ini && e1_fim >= e2_fim)
		return 1;
	return 0;
}

/* Evento, int --> int
 * Recebe um evento e um numero de uma sala
 * Testa se algum evento na sala s faz overlap com o evento dado 
 * 1- Ha colisao
 * 0- Nao ha colisao
*/
int salaTempoColisao(Evento evento, int s) {
	int x,e = Sala(s).curr;
	for(x=0;x<e;x++)
		if(eventoTempoColisao(evento, Evento(s,x)))
			return 1;
	return 0;
}

/* Evento, int --> int
 * Recebe um evento e um numero de uma sala
 * Calcula se o evento pode ser adicionado, em termos de tempo na sala s
 * 1- Pode ser colocado
 * 0- Nao pode ser colocado
*/
int salaVaga(Evento evento, int s) {
	if(s == 0)
		s = evento.sala;
	if(salaTempoColisao(evento, s)) {
		printf(ER_SALAOCUPADA, evento.desc, s);
		return 0;
	}
	return 1;
}

/* Evento, char[] --> int
 * Recebe um evento e uma array com o nome duma pessoa
 * Testa se a pessoa participa no evento dado
 * 1- Esta no evento
 * 0- Nao esta no evento
*/
int pessoaNesteEvento(Evento evento, char pess[STR_MAXSIZE]) {
	int x;
	for(x=0; x<PART_MAX; x++)
		if(!strcmp(pess, evento.part[x]))
			return 1;
	return 0; 
}

/* Evento, char[], {t} --> int
 * Recebe um evento e uma pessoa e um t(tipo)
 * t determina o tipo da mensagem de erro caso uma pessoa esteja ocupada
 * Calcula se uma pessoa pode ser adicionada a este evento, ou seja,
 * se esta em mais algum evento na mesma hora
 * 1- Pessoa livre
 * 0- Pessoa ocupada
*/
int pessoaLivre(Evento evento, char pess[], int t) {
	int x, y;
	for(x=1; x<=SALAS_MAX; x++) {
		for(y=0; y<Sala(x).curr; y++) {
			if(!eventoTempoColisao(evento, Evento(x,y)))
				continue;
			if(pessoaNesteEvento(Evento(x,y), pess)) {
				if(t)
					printf(ER_PESSOAOCUPADA, evento.desc, pess);
				else
				 	printf(ER_SOBREPOSTO, pess);
				return 0;
			}
		}
	}
	return 1;
}

/* int[] --> int
 * Recebe um array com 10 inteiros correspondentes a posicao no array de eventos da sala
 * Procura o evento mais pequeno entre os 10 com estas posicoes e retorna o inidice
 * Retorna -1 caso nao hajam mais eventos 
*/
int getMin(int indexEv[SALAS_MAX]) {
	int x, min=-1;
	Evento tmp;
	tmp.epoch = MAXLONG;
	/*Procura o primeiro evento do array que exista*/
	for(x=0; x<SALAS_MAX; x++)
		if(Sala(x+1).curr > indexEv[x] && tmp.epoch > Evento(x+1,indexEv[x]).epoch)
			tmp = Evento(x+1,indexEv[min = x]);
	return min;
}

/* char[] --> int*
 * Recebe uma string com a descricao de um evento e procura a sala e indice
 * que lhe correspondem, coloca num array estatico e retorna o pointer
 * Caso o evento nao exista coloca a sala(indice 0) a 0
*/
int *getIndice(char desc[]) {
	static int index[2]; 				/*Contem a sala e indice do evento em cada posicao dps de chamada a funcao*/
	for(index[0]=1; index[0]<=SALAS_MAX; index[0]++)
		for(index[1]=0; index[1]<Sala(index[0]).curr; index[1]++)
			if(!strcmp(desc,Evento(index[0],index[1]).desc))
				return index;
	printf(ER_INEXESTENTE, desc);
	index[0] = 0;
	return index;
}

/* int, int --> void
 * Recebe uma sala e indice do evento, e retira-o do array de eventos da sala
 * Fazendo um left shift de tudo na direita to evento a remover
*/
void removerEvento(int s, int e) {
	int x;
	for(x=e; x<Sala(s).curr-1;x++)
		Evento(s,x) = Evento(s,x+1);
	Sala(s).curr--;
}

/* Evento, int --> void
 * Recebe um evento e um inteiro
 * O inteiro determina o erro da pessoa ocupada
 * Testa se o evento pode ser adicionado na respetiva sala
 * Testanto compatibilidade de horas e pessoas
 * 1- Pode adicionar
 * 0- Nao pode adicionar
*/
int completeAvailable(Evento evento, int type) {
	int x, terminate = 0;
	if(!salaVaga(evento,0)) 
		return 0;
	for(x=0; x<PART_MAX && evento.part[x][0] != '\0'; x++)
		if(!pessoaLivre(evento, evento.part[x], type))
			terminate = 1;
	if(terminate) 
		return 0;
	return 1;
}

/* Evento, Evento, int, int, int --> void
 * Recebe 2 eventos, um para adicionar, 
 * e outro backup caso nao se possa adicionar o primeiro 
*/
void removerAddBackup(Evento e_novo, Evento e_backup, int s, int e, int t) {
	removerEvento(s,e); 						/*Remove o evento antigo na lista de eventos para nao haver colisao*/
	if(completeAvailable(e_novo, t)) 			/*Se houver disponibilidade para o novo evento, adiciona-se*/
		adicionarEvento(e_novo);
	else 										/*Se nao, restaura-se o backup*/
		adicionarEvento(e_backup);
}

/* char[] char*[] --> void
 * Recebe um char e um array de tokens
 * Le os parametros de um comando para o array e calcula os tokens
 * para cada parametro
*/
void lerParametros(char parametros[], char *tokens[]) {
	int x;
	fgets(parametros, PARAM_MAX, stdin);
	strtok(parametros,PARAM_DELIM_FIRST);
	for(x=0; x<TOKENS_MAX; x++) {
		tokens[x] = strtok(NULL, PARAM_DELIM_ALL);
	}
}

/* char*[] --> void
 * Recebe um array de pointers para cada parametro
 * Se possivel, adiciona um Evento na sua respetiva sala
*/
void procComando_a(char *tokens[]) {
	int x;
	Evento evento;
	evento.epoch = dateToEpoch(tokens[1], tokens[2]);
	evento.dur = atoi(tokens[3]);
	evento.sala = atoi(tokens[4]);
	strcpy(evento.desc,tokens[0]);
	for(x=0; x<PART_MAX; x++)
		if(tokens[x+5] == NULL)
			evento.part[x][0] = '\0';		/*Participante == '\0' ==> Participante nao existe*/
		else
			strcpy(evento.part[x],tokens[x+5]);
	if(completeAvailable(evento, 1))
		adicionarEvento(evento); 			/*adicionarEvento() assegura que os eventos estao por ordem cronologica na sala*/
}

/* void --> void
 * Printa, ordenado por tempo de inicio, todos os eventos
*/
void procComando_l() {
	int x, curr;
	int indexEv[SALAS_MAX];
	/*Preenche a lista de inteiros com a posicao primeiro evento de cada sala*/
	for(x=0; x<SALAS_MAX; x++)
		indexEv[x] = 0;
	/*Imprime o evento mais pequeno em termos de inicio ate nao haver mais eventos*/
	while((curr = getMin(indexEv)) != -1)
		eventoRepr(Evento(curr+1,indexEv[curr]++));
}

/* char*[] --> void
 * Imprime todos os eventos numa sala
*/
void procComando_s(char *tokens[]) {
	int s = atoi(tokens[0]), x;
	for(x=0;x<Sala(s).curr;x++)
		eventoRepr(Evento(s,x));
}

/* char*[] --> void
 * Remove o evento dado, se nao existir, nao faz nada
*/
void procComando_r(char *tokens[]) {
	int *i = getIndice(tokens[0]);
	if(i[0]==0)
		return;
	removerEvento(i[0],i[1]);
}

/* char*[] --> void
 * Altera a hora de inicio de um evento
*/
void procComando_i(char *tokens[]) {
	int *i = getIndice(tokens[0]), s = i[0], e = i[1];
	int h, m;
	Evento tmp, tmp_backup;
	if(s==0) 
		return;
	tmp = Evento(s,e);
	tmp_backup = tmp; 			/*Backup para mais tarde*/
	h = tmp.epoch % 1440; 		/*Calcula a hora de inicio antiga*/
	tmp.epoch -= h; 			/*Retira a hora de inicio antiga*/
	h = atoi(tokens[1]);
	m = h % 100;
	h /= 100; 					/*Calcula a nova hora de inicio*/
	tmp.epoch += h*60 + m; 		/*Adiciona a nova hora de inicio*/

	removerAddBackup(tmp,tmp_backup,s,e, 1);
}

/* char*[] --> void
 * Altera a duracao de um evento
*/
void procComando_t(char *tokens[]) {
	int *i = getIndice(tokens[0]), s = i[0], e = i[1];
	int dur = atoi(tokens[1]);
	Evento tmp, tmp_backup;
	if(s==0)
		return;
	tmp = Evento(s,e);
	tmp_backup = tmp;
	tmp.dur = dur;
	removerAddBackup(tmp,tmp_backup,s,e, 1);
}

/* char*[] --> void
 * Altera a sala de um evento
*/
void procComando_m(char *tokens[]) {
	int *i = getIndice(tokens[0]), s = i[0], e = i[1];
	int sala = atoi(tokens[1]);
	Evento tmp;
	if(s==0 || s == sala)
		return;
	tmp = Evento(s,e);
	tmp.sala = sala;
	
	if(!salaVaga(tmp,0))
		return;
	removerEvento(s,e);
	adicionarEvento(tmp);
}

/* char*[] --> void
 * Adiciona um participante a um evento
*/
void procComando_A(char *tokens[]) {
	int x, *i = getIndice(tokens[0]), s = i[0], e = i[1];
	if(s==0)
		return;
	if(pessoaNesteEvento(Evento(s,e), tokens[1])) 		/*Se a pessoa esta no evento, nao ha efeito*/
		return;
	if(Evento(s,e).part[PART_MAX-1][0] != '\0') { 		/*Se o ultimo slot esta preenchido ent o evento esta cheio*/
		printf(ER_CHEIO,tokens[0],PART_MAX-1);
		return;
	}
	if(!pessoaLivre(Evento(s,e), tokens[1], 0)) 		/*Se a pessoa nao esta livre nao se adiciona*/
		return;
	for(x=2; x<PART_MAX; x++)
		if(Evento(s,e).part[x][0] == '\0')  			/*Obtem o indice do primeiro espaco livre para*/
			break; 										/*Por um participante novo*/

	strcpy(Evento(s,e).part[x],tokens[1]);
}

/* char*[] --> void
 * Remove um participante de um evento
*/
void procComando_R(char *tokens[]) {
	int x, *i = getIndice(tokens[0]), s = i[0], e = i[1];
	if(s==0)
		return;
	if(!strcmp(Evento(s,e).part[0],tokens[1]))      /*Se a pessoa é o responsavel, nao ha efeito*/
		return;
	if(!pessoaNesteEvento(Evento(s,e), tokens[1])) 	/*Se a pessoa nao esta no evento, nao ha efeito*/
		return;
	if(Evento(s,e).part[2][0] == '\0') { 		 	/*Se nao existe segundo participante, entao so ha 1*/
		printf(ER_VAZIA, tokens[1], tokens[0]);
		return;
	}
	for(x=1; x<PART_MAX; x++) 						/*Calcula o indice do participante a remover*/
		if(!strcmp(Evento(s,e).part[x],tokens[1])) 
			break;

	for(; x<PART_MAX-1; x++)						/*Left shift dos participantes*/
		strcpy(Evento(s,e).part[x], Evento(s,e).part[x+1]); 						
	Evento(s,e).part[x][0] = '\0';	 			    /*Limpeza do slot mais à direita deixado pelo participante removido*/
}

/* Main
 * Loop principal que processa o comando e chama a respetiva funcao 
*/
int main() {
	int run = 1;
	char parametros[PARAM_MAX], *tokens[TOKENS_MAX]; 		
	initSalas();									 			/*Inicializa salas a 0*/
	while(run) {
		lerParametros(parametros,tokens);
		switch(parametros[0]) {
			case 'a':
				procComando_a(tokens);
				break;
			case 'l':
				procComando_l();
				break;
			case 's':
				procComando_s(tokens);		
				break;
			case 'r':
				procComando_r(tokens);
				break;
			case 'i':
				procComando_i(tokens);
				break;
			case 't':
				procComando_t(tokens);
				break;
			case 'm':
				procComando_m(tokens);
				break;
			case 'A':
				procComando_A(tokens);
				break;
			case 'R':
				procComando_R(tokens);
				break;
			case 'x':
				run = 0;
				break;
		}
	}
	return 0;
} 
