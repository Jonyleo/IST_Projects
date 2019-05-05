# Joao Oliveira - 93728

#Funcoes Celulas

def cria_celula(valor):
    """
    cria_celula: valor --> celula
    Retorna uma celula criada a partir de um valor 1,0 ou -1

    Erros: levanta um ValueError caso o argumento nao seja inteiro
    ou um valor possivel de uma celula

    Representacao interna: Uma celula consiste numa lista com um unico elemento
    que corresponde ao valor, ou seja, [1], [0] ou [-1]
    """
    if type(valor) != int or valor not in (1,0,-1):
        raise ValueError("cria_celula: argumento invalido.")
    return [valor]
    
def obter_valor(celula):
    """
    obter_valor: celula --> valor
    Retorna o valor da celula recebida
    """
    return celula[0]

def eh_celula(objeto):
    """
    eh_celula: universal --> booleano
    Retorna True se o argumento e uma celula e False se nao 
    """
    return type(objeto) == list and len(objeto) == 1 \
           and  type(objeto[0]) == int and objeto[0] in (1,0,-1)

def inverte_estado(celula):
    """
    inverte_estado: celula --> celula
    Retorna uma nova celula que tem o seu valor invertido

    Altera o valor da celula passada como argumento
    """
    celula[0] = (obter_valor(celula)+1) % 2 if obter_valor(celula) >= 0 else -1
    return celula

def celulas_iguais(c1,c2):
    """
    celulas_iguais: celula x celula --> booleano
    Retorna True se os argumentos sao celulas e sao a mesma celula
    e False se nao forem celulas ou se forem diferentes
    """
    return eh_celula(c1) and eh_celula(c2) \
           and obter_valor(c1) == obter_valor(c2)

def celula_para_str(celula):
    """
    celula_para_str: celula --> string
    Retorna a representacao em string de uma celula
    """
    return str(obter_valor(celula)) if obter_valor(celula) >= 0 else "x"

#Funcoes Coordenadas

def cria_coordenada(linha,coluna):
    """
    cria_coordenada: int x int --> tuple
    Recebe a linha e coluna e retorna uma coordenada com esses valores

    Erros: levanta um ValueError se a linha ou coluna recebida
    nao forem valores possiveis

    Representacao Interna: Uma coordenada consiste num tuplo de dois elementos
    a linha e a coluna respetivamente, por exemplo, (0,1)
    """
    if type(linha) != int or type(coluna) != int or \
       linha not in (0,1,2) or coluna not in (0,1,2):
        raise ValueError("cria_coordenada: argumentos invalidos.")
    return (linha,coluna)
    
def coordenada_linha(coordenada):
    """
    coordenada_linha: coordenada --> int
    Recebe uma coordenada e devolve o valor da linha
    """
    return coordenada[0]

def coordenada_coluna(coordenada):
    """
    coordenada_coluna: coordenada --> int
    Recebe uma coordenada e devolve o valor da coluna
    """
    return coordenada[1]

def eh_coordenada(objeto):
    """
    eh_coordenada: universal --> booleano
    Retorna True se o argumento e uma coordenada e False se nao 
    """
    return type(objeto) == tuple and len(objeto) == 2 and \
           objeto[0] in (0,1,2) and objeto[1] in (0,1,2)

def coordenadas_iguais(c1,c2):
    """
    coordenadas_iguais: coordenada x coordenada --> booleano
    Retorna True se os argumentos sao coordenadas e sao a mesma coordenada
    e False se nao forem coordenadas ou se forem diferentes
    """
    return eh_coordenada(c1) and eh_coordenada(c2) and \
           coordenada_linha(c1) == coordenada_linha(c2) and \
           coordenada_coluna(c1) == coordenada_coluna(c2)

def coordenada_para_str(coordenada):
    """
    coordenada_para_str: coordenada --> string
    Retorna a representacao em string de uma coordenada
    """
    return "({}, {})".format(coordenada_linha(coordenada), \
                             coordenada_coluna(coordenada))

#Funcoes Tabuleiros

def coordenada_lista(mode = "c"):
    """
    coordenada_lista: () --> lista
    Retorna uma lista com todas as coordenadas possiveis de um tabuleiro
    
    Modos:
        c- Retorna uma lista de coordenadas
        s- Retorna uma lista de coordenadas em strings
    """
    func_dict = {"c": cria_coordenada, \
                 "s": lambda x,y: coordenada_para_str(cria_coordenada(x,y))}
    return [func_dict[mode](x, y) for x in range(3) for y in range(x//2, 3)]

def tuple_to_tab(tuple_tab):
    """
    tuple_to_tab: tuplo --> tabuleiro
    Recebe um tuplo que representa um tabuleiro e retorna um tabuleiro
    """
    return dict(zip(coordenada_lista("s"),(cria_celula(y) for x in tuple_tab for y in x)))

def tabuleiro_inicial():
    """
    tabuleiro_inicial: () --> tabuleiro
    Retorna o tabuleiro inical do jogo Hello Quantum
    """
    return str_para_tabuleiro("((-1,-1,-1),(0,0,-1),(0,-1))")

def str_para_tabuleiro(string):
    """
    str_para_tabuleiro: string --> tabuleiro
    Recebe um string e converte-o num tabuleiro

    Erros: levanta ValueError se o argumento nao for um string,
    se o interior nao e um tuplo com 3 tuplos

    Representacao Interna: Um tabuleiro representa-se com um dicionario com
    8 pares chave-elemento,1 para cada celula do tabuleiro em que as chaves
    sao a coordenada transformada em string pela funcao coordenada_para_str()
    e os valores sao celulas
    """
    if type(string) != str:
        raise ValueError("str_para_tabuleiro: argumento invalido.")
    tuplo_tab = eval(string)
    if type(tuplo_tab) != tuple or len(tuplo_tab) != 3 or \
       any(len(tuplo_tab[x]) != (3,3,2)[x] or type(tuplo_tab[x]) != tuple for x in range(3)) or \
       any(type(y) != int or y not in (0,1,-1) for x in tuplo_tab for y in x):
        raise ValueError("str_para_tabuleiro: argumento invalido.")       
    return tuple_to_tab(tuplo_tab)

def tabuleiro_dimensao(tab):
    """
    tabuleiro_dimensao: tabuleiro --> int
    Retorna a dimensao de um tabuleiro, 3
    """
    return 3

def tabuleiro_celula(tab,coordenada):
    """
    tabuleiro_celula: tabuleiro x coordenada --> celula
    Retorna a celula de um tabuleiro nas coordenadas recebidas
    """
    return tab[coordenada_para_str(coordenada)]
    
def eh_tabuleiro(objeto):
    """
    eh_tabuleiro: universal --> booleano
    Retorna True se o argumento for um tabuleiro, se nao retorna False    
    """
    return type(objeto) == dict and len(objeto) == 8 \
           and all(y in coordenada_lista("s") for y in objeto) \
           and all(eh_celula(tabuleiro_celula(objeto,x)) for x in coordenada_lista()) 
    
def tabuleiros_iguais(tab1,tab2):
    """
    tabuleiros_iguais: tabuleiro x tabuleiro --> booleano
    Retorna True se ambos os argumentos sejam tabuleiros e sejam iguais
    e False no caso contrario
    """
    return eh_tabuleiro(tab1) and eh_tabuleiro(tab2) and \
           all(celulas_iguais(tabuleiro_celula(tab1,x),tabuleiro_celula(tab2,x)) for x in coordenada_lista())

def tabuleiro_para_str(tab):
    """
    tabuleiro_str: tabuleiro --> string
    Retorna um string formatado como tabuleiro de jogo a partir do argumento
    """
    return "+-------+\n|...{2}...|\n|..{1}.{5}..|\n|.{0}.{4}.{7}.|\n|..{3}.{6}..|\n+-------+" \
           .format(*[celula_para_str(tabuleiro_celula(tab,x)) for x in coordenada_lista()])

def tabuleiro_substitui_celula(tab,celula,coordenada):
    """
    tabuleiro_substitui_celula: tabuleiro x celula x coordenada --> tabuleiro
    Retorna um tabuleiro em que a celula na coordenada
    recebida mudou para a celula recebida

    Altera o tabuleiro passado como argumento
    """
    if not eh_tabuleiro(tab) or not eh_celula(celula) or not eh_coordenada(coordenada):
        raise ValueError("tabuleiro_substitui_celula: argumentos invalidos.")
    tab[coordenada_para_str(coordenada)] = celula
    return tab

def tabuleiro_inverte_estado(tab,coordenada):
    """
    tabuleiro_inverte_estado: tabuleiro x coordenada --> tabuleiro
    Retorna um tabuleiro em que o estado da celula
    na coordenada recebida foi invertido
    
    Altera o tabuleiro passado como argumento
    """
    if not eh_tabuleiro(tab) or not eh_coordenada(coordenada):
        raise ValueError("tabuleiro_inverte_estado: argumentos invalidos.")
    inverte_estado(tabuleiro_celula(tab,coordenada))
    return tab

#Funcoes de alto nivel

#Portas

def porta_aux(tab, op, porta):
    """
    porta_aux: tabuleiro x string x int --> tabuleiro
    Recebe um tabuleiro, uma operacao ("E" ou "D") e uma porta,
    o valor estatico de cada operacao em relacao aos valores das coordenadas
    """
    return [tabuleiro_inverte_estado(tab,cria_coordenada(x if op == "D" else porta, x if op == "E" else porta)) for x in range(3)][-1]

def porta_x(tab,op):
    """
    porta_x: tabuleiro x string --> tabuleiro
    Retorna o tabuleiro resultante da aplicacao da porta x no lado recebido

    Erros: levanta um ValueError se os argumentos nao forem um tabuleiro
    ou uma operacao valida respetivamente
    """
    if not eh_tabuleiro(tab) or op not in ("E","D"):
        raise ValueError("porta_x: argumentos invalidos.")
    return porta_aux(tab,op,1)

def porta_z(tab,op):
    """
    porta_z: tabuleiro x string --> tabuleiro
    Retorna o tabuleiro resultante da aplicacao da porta z no lado recebido

    Erros: levanta um ValueError se os argumentos nao forem um tabuleiro
    ou uma operacao valida respetivamente
    """
    if not eh_tabuleiro(tab) or op not in ("E","D"):
        raise ValueError("porta_z: argumentos invalidos.")
    return porta_aux(tab,op,0 if op == "E" else 2)

def porta_h(tab,op):
    """
    porta_h: tabuleiro x string --> tabuleiro
    Retorna o tabuleiro resultante da aplicacao da porta h no lado recebido

    Erros: levanta um ValueError se os argumentos nao forem um tabuleiro
    ou uma operacao valida respetivamente
    """
    def tabuleiro_substitui_celulas(tab,coordenada1,coordenada2):
        """
        tabuleiro_substitui_celulas: tabuleiro x coordenada x coordenada --> tabuleiro
        Retorna um tabuleiro em que as celulas nas coordenadas recebidas estao trocadas uma pela outra

        Altera o tabuleiro passado como argumento
        """
        celula1 = tabuleiro_celula(tab,coordenada1)
        tabuleiro_substitui_celula(tab,tabuleiro_celula(tab,coordenada2),coordenada1)
        tabuleiro_substitui_celula(tab,celula1,coordenada2)
        return tab

    if not eh_tabuleiro(tab) or op not in ("E","D"):
        raise ValueError("porta_h: argumentos invalidos.")
    if op == "E":
        [tabuleiro_substitui_celulas(tab,cria_coordenada(0,x),cria_coordenada(1,x)) for x in range(3)]
    else:
        [tabuleiro_substitui_celulas(tab,cria_coordenada(x,1),cria_coordenada(x,2)) for x in range(3)]
    return tab

#Jogo

def hello_quantum(string):
    """
    hello_quantum: string --> booleano
    Loop principal do jogo
    Este loop controla o jogo ao perguntar as jogadas que este quer fazer e
    imprimir o resultado dessas jogadas
    """
    instrucoes_dict = {"X": porta_x,"Z": porta_z,"H": porta_h}
    tab_inicial = tabuleiro_inicial()
    tab_final, jogadas_possiveis = (lambda x,y: (str_para_tabuleiro(x), int(y)))(*string.split(":"))
    jogadas_atuais = 0
    
    print("Bem-vindo ao Hello Quantum!\nO seu objetivo e chegar ao tabuleiro:\n" \
          "{}\nComecando com o tabuleiro que se segue:\n{}" \
          .format(tabuleiro_para_str(tab_final),tabuleiro_para_str(tab_inicial)))

    while not tabuleiros_iguais(tab_final,tab_inicial):
        if jogadas_atuais == jogadas_possiveis:
            return False
        
        jogadas_atuais += 1
        
        porta = input("Escolha uma porta para aplicar (X, Z ou H): ")
        lado = input("Escolha um qubit para analisar (E ou D): ")
        print(tabuleiro_para_str(instrucoes_dict[porta](tab_inicial,lado)))

    print("Parabens, conseguiu converter o tabuleiro em {} jogadas!" \
          .format(jogadas_possiveis))
    return True
