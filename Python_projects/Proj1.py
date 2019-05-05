def eh_tabuleiro(tab):
    """eh_tabuleiro: universal -> booleano
    verifica se o argumento é um tabuleiro valido em termos de formatação"""
    tab_Length = (3,3,2) 
    tab_Valid = (-1,0,1)
    #verificação do tabuleiro
    if not isinstance(tab,tuple) or len(tab) != 3:
        return False
    #verificação individual de cada sub-tuplo
    for x in range(len(tab)):
        if not isinstance(tab[x],tuple) or len(tab[x]) != tab_Length[x]:
            return False
        #verificação de cada elemento de cada sub-tuplo
        for y in range(len(tab[x])):
            if tab[x][y] not in tab_Valid:
                return False
    return True

def tabuleiro_str(tab):
    """tabuleiro_str: tabuleiro -> cad. caracteres
    retorna um string formatado como tabuleiro de jogo a partir do argumento"""
    tab_String = "+-------+\n"
    tab_Line_Length = (1,2,3,2)
    
    if not eh_tabuleiro(tab):
        raise ValueError("tabuleiro_str: argumento invalido")
    #construtor das linha
    for x in range(4):
        tab_String_Line = "|"+"." * (4-tab_Line_Length[x])
        for y in range(tab_Line_Length[x]):
            tab_String_Line_toAdd = tab[y+(x//3)][y-tab_Line_Length[x]-(x//3)]
            if tab_String_Line_toAdd == -1:
                tab_String_Line += "x."
            else:
                tab_String_Line += str(tab_String_Line_toAdd) +"."
        tab_String += tab_String_Line + "."*(3-tab_Line_Length[x]) +"|\n"
        tab_String_Line = ""
    tab_String += "+-------+"
    return tab_String
        
def tabuleiros_iguais(tab1,tab2):
    """tabuleiros_iguais: tabuleiro x tabuleiro -> booleano
    verifica se os tabuleiros são iguais"""
    #eh_tabuleiro verifica os argumentos desta função
    if not eh_tabuleiro(tab1) or not eh_tabuleiro(tab2):
        raise ValueError("tabuleiros_iguais: um dos argumentos nao e tabuleiro")
    return tab1 == tab2

def inverteE(t):
    """inverteE: Inverte os valores de 0 e 1 dum tuplo"""
    #cada linha invertida no lado esquerdo corresponde à inversao do primeiro ou segundo tuplo
    tab_NewLine = ()
    for x in t:
        if x == -1:
            tab_NewLine += (-1,)
        else:
            tab_NewLine += (int(not x),)
    return tab_NewLine

def inverteD(tab,porta):
    """inverteD: Inverte o lado direito das portas x e z (1->0, 0->1)
    quando porta é respetivamente -2, para x e -1 para z"""
    newTab = ()
    for x in tab:
        newLine = ()
        for y in range(-1,-len(x)-1,-1):
            #inverte os elementos cujo indice em cada tuplo é porta
            #ou seja, o "mais ha direita no tabuleiro" ou "mais à esquerda" dos dois
            if y == porta:
                if x[y] == -1:
                    newLine = (-1,) + newLine
                else:
                    # int(not(x[y])) torna um 1 em 0 e vice versa
                    newLine = (int(not x[y]),) + newLine
            else:
                newLine = (x[y],) + newLine
        newTab += (newLine,)
    return newTab
            
def porta_x(tab,op):
    """porta_x: tabuleiro x {“E”, “D”} -> tabuleiro
    porta_x inverte a linha do tabuleiro de dentro, da esquerda(E) ou direita(D)"""
    #eh_tabuleiro verifica o argumento tab(tabuleiro) desta função
    #verifica se o argumento op(operação) é um dos possiveis
    if not eh_tabuleiro(tab) or op not in ("E", "D"):
        raise ValueError("porta_x: um dos argumentos e invalido")
    if op == "E":
        return tab[0],inverteE(tab[1]),tab[2]
    else:
        return inverteD(tab,-2)
        
def porta_z(tab,op):
    """porta_z: tabuleiro x {“E”, “D”} -> tabuleiro
    porta_x inverte a linha do tabuleiro de jogo, da esquerda(E) ou direita(D)"""
    #eh_tabuleiro verifica o argumento tab(tabuleiro) desta função
    #verifica se o argumento op(operação) é um dos possiveis
    if not eh_tabuleiro(tab) or op not in ("E", "D"):
        raise ValueError("porta_z: um dos argumentos e invalido")
    if op == "E":
        return inverteE(tab[0]),tab[1],tab[2]
    else:
        return inverteD(tab,-1)

def porta_h(tab, op):
    """porta_h: tabuleiro x {“E”, “D”} -> tabuleiro
    inverte as linhas exteriores do tabuleiro de jogo esquerda(E) ou direita(D)"""
    #eh_tabuleiro verifica o argumento tab(tabuleiro) desta função
    #verifica se o argumento op(operação) é um dos possiveis
    if not eh_tabuleiro(tab) or op not in ("E", "D"):
        raise ValueError("porta_h: um dos argumentos e invalido")
    if op == "E":
        #inverte os dois tuplos da esquerda do tabuleiro
        return (tab[1],tab[0],tab[2])
    else:
        #inverte os ultimos dois elementos de cada tuplo entre eles
        #ou seja, as duas "linhas da direita" do tabuleiro de jogo
        return ((tab[0][0],tab[0][2],tab[0][1]),(tab[1][0],tab[1][2],tab[1][1]),(tab[2][1], tab[2][0]))
