#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LINE_LENGTH 255

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~* ESTRUTURAS *~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*/

typedef struct tipoToken{
    char* lexema;
    char* Simbolo;
}tipoToken;

typedef struct posfixa {
    char valor[10];
    int precedencia;
    struct posfixa *prev;
}psPilha;

typedef struct posfixaTopo{
    psPilha *topo;
}psTopo;

typedef struct tabelaSimbolo{
    char lexema[30];
    char tipo[30];
    int nivel;
    void* mem;
    struct tabelaSimbolo *prev;
}tabelaSimbolos;

typedef struct pilha{
    tabelaSimbolos *topo;
}Topo;

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~* CABEÇALHOS *~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*/

tipoToken *createToken(char* lex, char* sim);

void tratarIdentificador();

void tratarDigito();

void tratarOperador();

void lexico();

int analisador();

void analisa_bloco();

int et_analisa_var();

int analisa_var();

void analisa_tipo();

void analisa_comandos();

void analisa_comando_simples();

void analisa_atrib_chprocedimento();

void analisa_atribuicao();

void analisa_leia();

void analisa_escreva();

void analisa_enquanto();

void analisa_se();

int analisa_expressao();

int analisa_expressao_simples();

int analisa_termo();

int analisa_fator();

void analisa_chamada_procedimento();

void analisa_chamada_funcao();

void analisa_subrotinas();

void analisa_declaracao_procedimento();

void analisa_declaracao_funcao();


void iniciaPilha();

void insereTabela(char lexema[], char nometabela[], int nivel, void* mem);

void Desempilha();

bool buscaDuplicado(char lexema[], int nivel);

bool buscaVarDuplicado(char lexema[], int nivel);

int buscaVarDeclarado(char lexema[]);

void printPilha();

void defineTipoVar(char* tipo);

int pesquisa_procedimento(char *id);

int pesquisa_funcao(char *id);

char* confereTipo(char* lexema);

void gera(char *rotulo, char *comando, char *param1, char *param2);
/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~* VARIAVEIS *~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*/

char text[2048];

FILE *arquivo;

FILE *codigo;

tipoToken* token = NULL;

tabelaSimbolos* TS;

Topo *inicio;

psPilha* PS;

psTopo *psInicio;

int _index_ = 0;

int escopo_global = 0;

int rotulo = 0;

int mem = 1;

int funcEnd= 0;

int lineCounter = 1;


/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~* MAIN *~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*/

int main(){
    char line[LINE_LENGTH];
    char linemarker[2] = {13,0};
    int resultado;
    //O endereço deve ser alterado para o adequado SEMPRE
    arquivo=fopen("C:/Users/nucle/OneDrive/Documentos/GitHub/Compilador-em-C/compelado/gera3.txt","r");
    //arquivo=fopen("C:/Users/nucle/Documents/GitHub/Compilador-em-C/compelado/gera1.txt","r");
    //arquivo=fopen("/home/luckytods/CLionProjects/Compilador-em-C/compelado/gera1.txt","r");
    //arquivo=fopen("C:/Users/19088582/Downloads/Compilador-em-C-main/compelado/gera2.txt","r");
    if(arquivo == NULL) {
        printf("ERRO");
        exit(1);
    }

     //Inicializa a pilha
    iniciaPilha();
    iniciaPosFixa();
    //Insere a primeira marca na pilha que deve estar lá. As marcas també representam a troca de nivel/escopo

    insereTabela("final", "final", -1, NULL);
    insereTabela("#", "marca", escopo_global, NULL);


    token = createToken("", "");
    while(fgets(line,LINE_LENGTH,arquivo)){
        strcat(text, line);
        strcat(text,linemarker);
        memset(line, 0, sizeof(line));
    }
    codigo = fopen("codigo.txt", "w");
    resultado = analisador();
    printf("\nPilha final: \n");
    printPilha();
    if (resultado == 0)
        printf("\nanalise concluida\n");
    else printf("\nerro na analise\n");
    fclose(arquivo);
    fclose(codigo);
    return 0;

}

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~* FUNÇÕES *~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*/

tipoToken *createToken(char* lex, char* sim){

    tipoToken *newToken = malloc(sizeof(tipoToken));
    newToken->lexema = malloc(strlen(lex));
    strcpy(newToken->lexema, lex);
    newToken->Simbolo = malloc(strlen(sim));
    strcpy(newToken->Simbolo,sim);
    return newToken;
}

void freeToken()
{
    free(token->lexema);
        free(token->Simbolo);
        free(token);
}

void tratarIdentificador(){
    char ID[30] = {0};
    char palavrasReservadas[][20] = { "programa",
                                    "se",
                                    "entao",
                                    "senao",
                                    "enquanto",
                                    "faca",
                                    "inicio",
                                    "fim",
                                    "escreva",
                                    "leia",
                                    "var",
                                    "inteiro",
                                    "booleano",
                                    "verdadeiro",
                                    "falso",
                                    "procedimento",
                                    "funcao",
                                    "div",
                                    "e",
                                    "ou",
                                    "nao"};
    char s[20] = {0};
    s[1] = '\0';
    s[0] = 's';
    char cs[2] = {0};
    cs[1] = '\0';
    cs [0] = text[_index_];
    do{

        strcat( ID, cs);
        _index_++;
        cs[0] = text[_index_];
    }while ((cs[0] >= 65 && cs[0] <=90) || (cs[0]>= 97 && cs[0] <= 122) || (cs[0] >=48 && cs[0] <= 57) || cs[0] == '_');

    for (int I = 0; I < 21; I++)
    {
        if ( strcmp(ID,palavrasReservadas[I]) == 0)
        {
            strcat(s, ID);

            //token = createToken( ID, s);
            strcpy(token->lexema, ID);
            strcpy(token->Simbolo, s);

            return;
        }
    }

    //token = createToken( ID, "sidentificador");
    strcpy(token->lexema, ID);
    strcpy(token->Simbolo, "sidentificador");
}

void tratarDigito(){
    char ID[30]={0};
    char cs[2];
    cs[1] = '\0';
    cs [0] = text[_index_];
    do{

        strcat( ID, cs);
        _index_++;
        cs[0] = text[_index_];
    }
    while(cs[0] >=48 && cs[0] <= 57);
    //token = createToken( ID, "snumero");
    strcpy(token->lexema, ID);
    strcpy(token->Simbolo, "snumero");
}

void tratarOperador(){
    char ID[30] = {0};
    char c[2];
    c[1] = '\0';
    c[0] = text[_index_];
    _index_++;
    switch(c[0]){
        case '+':
                    strcpy(token->lexema, c);
                    strcpy(token->Simbolo, "smais"); break;
        case '-':
                    strcpy(token->lexema, c);
                    strcpy(token->Simbolo, "smenos"); break;
        case '*':
                    strcpy(token->lexema, c);
                    strcpy(token->Simbolo, "smult"); break;
        case ';':
                    strcpy(token->lexema, c);
                    strcpy(token->Simbolo, "sponto_virgula");break;
        case ',':
                    strcpy(token->lexema, c);
                    strcpy(token->Simbolo, "svirgula"); break;
        case '.':
                    strcpy(token->lexema, c);
                    strcpy(token->Simbolo, "sponto"); break;
        case '(':
                    strcpy(token->lexema, c);
                    strcpy(token->Simbolo, "sabre_parenteses"); break;
        case ')':
                    strcpy(token->lexema, c);
                    strcpy(token->Simbolo, "sfecha_parenteses"); break;
        case '=':
                    strcpy(token->lexema, c);
                    strcpy(token->Simbolo, "sig"); break;
        case '!':

            if (text[_index_] != '=')
                printf("\terro linha %d, caractere %s nao esperado, esperava \"=\"\n", lineCounter, text[_index_]);
            else
                {
                    _index_++;

                    strcpy(token->lexema,"!=");
                    strcpy(token->Simbolo, "sdif");
                }
            break;
        case ':':
            if (text[_index_] != '=')
            {

                strcpy(token->lexema,c);
                strcpy(token->Simbolo, "sdoispontos");
            }
            else
                {
                    _index_++;

                    strcpy(token->lexema,":=");
                    strcpy(token->Simbolo, "satribuicao");

                }
            break;
        case '<':
            if (text[_index_] != '=')
            {

                strcpy(token->lexema,c);
                    strcpy(token->Simbolo, "smenor");
            }
            else
                {
                    _index_++;

                    strcpy(token->lexema,"<=");
                    strcpy(token->Simbolo, "smenorig");
                }
            break;
        case '>':
            if (text[_index_] != '=')
            {

                strcpy(token->lexema,c);
                strcpy(token->Simbolo, "smaior");
            }
            else
            {
                _index_++;

                strcpy(token->lexema,">=");
                strcpy(token->Simbolo, "smaiorig");
            }
            break;
        default: printf("erro linha %d, Operador %s nao identificado \n", lineCounter, c); break;
    }

}

void lexico(){
    char c = text[_index_];



        if (c == '{')
        {
            while (c!= '}' && c != 0)
            {
                _index_++;
                c = text[_index_];
                if (c == 13)
                    lineCounter++;
                else if (c == 0)
                    printf("erro linha %d, comentario sem fim\n", lineCounter);
            }
            _index_++;
            lexico();
        } else if ((c >= 65 && c <=90)|| (c >= 97 && c <= 122))
        {


            tratarIdentificador();
        }
        else if (c >=48 && c <= 57)
        {

            tratarDigito();
        }

        else if (c == 32 || c == 10 || c == 13 || c == 9)
        {
            if (c == 13)
                lineCounter++;
            _index_++;
            lexico();
        }
        else if (c == 0)
        {
            token = NULL;
        }
        else
        {

            tratarOperador();
        }
        c = text[_index_];

}

int analisador(){
    printf("\n[analise]\n");
    lexico();
    char memString[5];
    char memString2[5];
    if (strcmp(token->Simbolo, "sprograma") == 0)
        {lexico(); printf("\n %s ", token->lexema);}
    else {printf("\terro linha %d, \"programa\" esperado, %s recebido\n", lineCounter, token->Simbolo);return;}//\terro não tem programa

    if (strcmp(token->Simbolo, "sidentificador") == 0){

        insereTabela(token->lexema,"nomedeprograma", escopo_global, rotulo);
        gera("","START", "", "");
        gera ("", "ALLOC", "0", "1");
        rotulo++;
        lexico(); printf("\n %s ", token->lexema);
        }
    else {printf("\terro linha %d, identificador esperado, %s recebido\n", lineCounter, token->Simbolo);return;}//\terro falta identificador
    if (strcmp(token->Simbolo, "sponto_virgula") == 0)
        analisa_bloco();
    else  {printf("\terro linha %d, \";\" esperado\n", lineCounter);return;}//\terro ponto_virg
    if (strcmp(token->Simbolo, "sponto") == 0)
    {
        lexico();
        if (token == NULL)
        {
            sprintf(memString, "%d", mem-1);
            sprintf(memString2, "%d", mem);
            gera("", "DALLOC", memString, memString2);
            gera("", "HLT", "", "");
            mem = 1;
            return 0; // sucesso
        }
        else {printf("\terro linha %d, final de codigo esperado\n", lineCounter);return;}// \terro
    } else {printf("\terro em linha %d, \".\" esperado\n", lineCounter);return;} //\terro
}

void analisa_bloco(){
    printf("\n[analisa_bloco]\n");

    lexico();
    printf("\n %s ", token->lexema);
    int memLocal = et_analisa_var();
    char memString[5], memLocalString[5];

    if ( memLocal >0)
    {
        sprintf(memString, "%d", mem);
        sprintf(memLocalString, "%d", memLocal);
        gera("", "ALLOC", memString, memLocalString);
        mem = mem + memLocal;
    //allocs
    }

    sprintf(memString, "%d", mem-memLocal);

    analisa_subrotinas();
    funcEnd = 0;
    analisa_comandos();

    if ( memLocal >0)
    {
    gera ("", "DALLOC", memString, memLocalString);
    mem = mem-memLocal;
    //dallocs
    }

    printf("\n[analisa_bloco end]\n");
}


int et_analisa_var(){
    printf("\n[et_analisa_var]\n");
    int varCount=0;
    if(strcmp(token->Simbolo,"svar")== 0)
    {lexico(); printf("\n %s ", token->lexema);
        if(strcmp(token->Simbolo,"sidentificador")== 0)
        {
            while(strcmp(token->Simbolo,"sidentificador")== 0)
            {
                varCount = varCount + analisa_var();

                if (strcmp(token->Simbolo,"sponto_virgula")== 0)
                    {
                        lexico(); printf("\n %s ", token->lexema);}
                else  {printf("\terro linha %d, \";\" esperado\n", lineCounter);return;} //\terro ; esperado
            }
        }else {printf("\terro linha %d, identificador esperado\n", lineCounter);return;}// \terro falta identificador

    }
    printf("\n[et_analisa_var end]\n");
    return varCount;
}

int analisa_var(){
    printf("\n[analisa_var]\n");
    int varCount = 0;
    do{

        if(strcmp(token->Simbolo,"sidentificador")== 0){
            if (!(buscaVarDuplicado(token->lexema, escopo_global))) {

                insereTabela(token->lexema, "variavel", escopo_global, mem+varCount);
                varCount++;
                lexico();
                printf("\n %s ", token->lexema);
            }
            else{
                printf("\terro linha %d, nome de variavel ja utilizado\n", lineCounter);
                return;
            }
        }
        else {printf("\terro linha %d, identificador esperado\n", lineCounter);return;}

        if(strcmp(token->Simbolo,"svirgula")== 0 || strcmp(token->Simbolo,"sdoispontos")== 0)
        {
            if (strcmp(token->Simbolo,"svirgula")== 0)
            {
                lexico();
                printf("\n %s ", token->lexema);
                if (strcmp(token->Simbolo,"sdoispontos")== 0)
                    {printf("\terro linha %d, identificador esperado\n", lineCounter);return;}//\terro, identificador espserado
            }
        } else {printf("\terro linha %d, \",\" ou \":\" esperado\n", lineCounter);return;} //\terro , ou : esperado
    }while  (strcmp(token->Simbolo,"sdoispontos") != 0);

    lexico();
    printf("\n %s ", token->lexema);
    analisa_tipo();

    printf("\n[analisa_var end]\n");
    return varCount;
}

void analisa_tipo(){
    printf("\n[analisa_tipo]\n");
    if (strcmp(token->Simbolo,"sinteiro")!=0 && strcmp(token->Simbolo,"sbooleano")!=0){
        printf("\terro linha %d, tipo nao reconhecido\n", lineCounter);
        return;
    }

    defineTipoVar(token->lexema);

    lexico();
    printf("\n %s ", token->lexema);
    printf("\n[analisa_tipo end]\n");
}

void analisa_comandos(){
    printf("\n[analisa_comandos]\n");
    if (strcmp(token->Simbolo, "sinicio") != 0)
     {printf("\terro linha %d, inicio esperado\n", lineCounter);return;} //\terro, inicio esperado
    lexico();
    printf("\n %s ", token->lexema);
    analisa_comando_simples();
    while(funcEnd == 0 &&strcmp(token->Simbolo,"sfim") != 0)
    {
        if (strcmp(token->Simbolo, "sponto_virgula") == 0)
        {
            lexico();
            printf("\n %s ", token->lexema);
            if (strcmp(token->Simbolo,"sfim") != 0)
                analisa_comando_simples();
        } else {printf("\terro linha %d, \";\" esperado\n", lineCounter);return;} //\terro ; esperado
    }
    lexico();
    printf("\n %s ", token->lexema);
    printf("\n[analisa_comandos end]\n");
}

void analisa_comando_simples(){
    printf("\n[analisa_comando_simples]\n");
    if (strcmp(token->Simbolo, "sidentificador") == 0)
        analisa_atrib_chprocedimento();
    else if (strcmp(token->Simbolo, "sse") == 0)
        analisa_se();
    else if (strcmp(token->Simbolo, "senquanto") == 0)
        analisa_enquanto();
    else if (strcmp(token->Simbolo, "sleia") == 0)
        analisa_leia();
    else if (strcmp(token->Simbolo, "sescreva") == 0)
        analisa_escreva();
    else analisa_comandos();
    printf("\n[analisa_comando_simples end]\n");
}

void analisa_atrib_chprocedimento(){
    printf("\n[analisa_atrib_chprocedimento]\n");
    int memProc = pesquisa_procedimento(token->lexema);
    char memString[5];
    if (memProc == -1)
        analisa_atribuicao();
    else
    {

    sprintf(memString, "%d", memProc);
    gera("", "CALL", memString, "");

    lexico();
    printf("\n %s ", token->lexema);
    }


    printf("\n[analisa_atrib_chprocedimento end]\n");
}

void analisa_atribuicao(){
    printf("\n[analisa_atribuicao]\n");
    //if (buscaVarDeclarado(token->lexema) == -1 && pesquisa_funcao(token->lexema) != 1)
    char memString[5];
    int mem = buscaVarDeclarado(token->lexema);
    int tipoFunc;
    char *tipoVar;
    if (mem == -1)
    {
        tipoFunc = pesquisa_atrib_funcao(token->lexema);
        printf( " testing yo %d ", tipoFunc);
        if (tipoFunc == -1)
        {
            printf("\terro semantico linha %d, variavel nao encontrada \n", lineCounter);return;// erro, identificador inválido
        }
    } else tipoVar = confereTipo(token->lexema);


    lexico();
    printf("\n %s ", token->lexema);
    if (strcmp(token->Simbolo, "satribuicao") != 0)
       { printf("\terro linha %d, \":=\" esperado\n", lineCounter);return;} //erro, ':=' esperado

    lexico();
    printf("\n %s ", token->lexema);
    int exp_tipo = analisa_expressao();
    popAll();



    if (exp_tipo == 2)
        { printf("\terro semantico linha %d, expressao invalida \n", lineCounter);return;}
    //if (((strcmp(tipo, "variavel booleano")  == 0|| strcmp(tipo, "funcao booleana")  == 0) && exp_tipo == -1)||((strcmp(tipo, "variavel inteiro") == 0 || strcmp(tipo, "funcao inteiro")  == 0) && exp_tipo == 0))

    if(mem == -1)
    {

        if (tipoFunc == exp_tipo)
        {
            gera("", "STR", "0", "");
            funcEnd = 1;
            lexico();
            printf("\n %s ", token->lexema);
            printf("\n[analisa_atribuicao end]\n");
            return; // sem erro
        }
    }
    else if ((strcmp(tipoVar, "variavel booleano")  == 0 && exp_tipo == -1)||(strcmp(tipoVar, "variavel inteiro") == 0  && exp_tipo == 0))
    {
        sprintf(memString, "%d", mem);
        gera("", "STR", memString, "");
        printf("\n[analisa_atribuicao end]\n");
        return; // sem erro
    }

    printf("\terro semantico linha %d, tipos incompativeis %d\n", lineCounter, exp_tipo);
    return; //expressão incompatível com váriavel
}

void analisa_leia(){
    printf("\n[analisa_leia]\n");
    lexico();
    int mem;
    char memString[5];
    printf("\n %s ", token->lexema);
    if (strcmp(token->Simbolo, "sabre_parenteses") == 0)
        {lexico(); printf("\n %s ", token->lexema);}
    else {printf("\terro linha %d, \"(\" esperado\n", lineCounter);return;}//\terro
    if (strcmp(token->Simbolo, "sidentificador") == 0){
        mem = buscaVarDeclarado(token->lexema);
        if(mem != -1) {
            if (strcmp(confereTipo(token->lexema), "variavel inteiro") == 0)
            {
                sprintf(memString,"%d",mem);
                gera("","RD", "", "");
                gera("","STR",memString, "");

                lexico();
                printf("\n %s ", token->lexema);
            } else {printf("\terro semantico linha %d, tipo de variavel invalido\n", lineCounter);return;} //erro, variável inválida
        }
        else{
            printf("erro linha %d, variavel não foi encontrada", lineCounter);
            return;
        }
    }
    else {printf("\terro linha %d, identificador esperado\n", lineCounter);return;}//\terro
    if (strcmp(token->Simbolo, "sfecha_parenteses") == 0)
        {lexico(); printf("\n %s ", token->lexema);}
    else {printf("\terro linha %d, \")\" esperado\n", lineCounter);return;}//\terro
    printf("\n[analisa_leia end]\n");
}

void analisa_escreva(){
    printf("\n[analisa_escreva]\n");
    int mem;
    char memString[5];

    lexico();
    printf("\n %s ", token->lexema);
    if (strcmp(token->Simbolo, "sabre_parenteses") == 0)
        {lexico(); printf("\n %s ", token->lexema);}
    else {printf("\terro linha %d, \"(\" esperado\n", lineCounter);return;}//\terro
    if (strcmp(token->Simbolo, "sidentificador") == 0){
        mem = buscaVarDeclarado(token->lexema);
        //if(buscaVarDeclarado(token->lexema) != -1 || pesquisa_funcao(token->lexema) == 1)
        if(mem != -1) {
           //if (strcmp(confereTipo(token->lexema), "variavel inteiro") == 0 || strcmp(confereTipo(token->lexema), "funcao inteiro") == 0)
            if (strcmp(confereTipo(token->lexema), "variavel inteiro") == 0)
            {
                sprintf(memString,"%d",mem);
                gera("","LDV",memString, "");
                gera("","PRN","","");

                lexico();
                printf("\n %s ", token->lexema);
            } else   {printf("\terro semantico linha %d, tipo invalido\n", lineCounter);return;} //erro, variável inválida
        }
        else{
            printf("erro linha %d, variavel ou funcao não encontrada", lineCounter);
            return;
        }
    }
    else {printf("\terro linha %d, identificador esperado\n", lineCounter);return;}//\terro

    if (strcmp(token->Simbolo, "sfecha_parenteses") == 0)
        {lexico(); printf("\n %s ", token->lexema);}
    else {printf("\terro linha %d, \")\" esperado\n",lineCounter);return;}//\terro
    printf("\n[analisa_escreva end]\n");
}

void analisa_enquanto(){
    printf("\n[analisa_enquanto]\n");
    int auxrot1 = rotulo, auxrot2;
    char rotString[5];

    sprintf(rotString, "%d", rotulo);
    gera(rotString, "NULL", " ", " ");
    rotulo++;
    lexico();
    printf("\n %s ", token->lexema);
    if (analisa_expressao() != 1)
        {printf("\terro semantico linha %d, expressao invalida\n", lineCounter);return;} //erro, não é bool
    popAll();
    if (strcmp(token->Simbolo, "sfaca") == 0)
    {
        auxrot2 = rotulo;
        sprintf(rotString, "%d", rotulo);
        gera(" ", "JMPF", rotString, " ");
        rotulo++;
        lexico();
        printf("\n %s ", token->lexema);
        analisa_comando_simples();

        sprintf(rotString, "%d", auxrot1);
        gera(" ", "JMP", rotString, " ");
        sprintf(rotString, "%d", auxrot2);
        gera(rotString, "NULL", " ", " ");
    }
    else {printf("\terro linha %d, faca esperado\n", lineCounter);return;} //\terro

    printf("\n[analisa_enquanto end]\n");
}

void analisa_se(){
    printf("\n[analisa_se]\n");

    int auxrot, auxrot2;
    int flag = 0;
    char rotString[7];

    lexico();
    printf("\n %s ", token->lexema);
    if (analisa_expressao() != 1)
        {printf("\terro semantico linha %d, expressao invalida\n", lineCounter);return;} //erro, não é bool
    popAll();

    auxrot = rotulo;
    sprintf(rotString, "%d", rotulo);

    gera(" ", "JMPF", rotString, " ");
    rotulo++;

    if (strcmp(token->Simbolo, "sentao") == 0)
        {lexico(); printf("\n %s ", token->lexema);}
    else {printf("\terro linha %d, entao esperado\n", lineCounter);return;} //\terro
    analisa_comando_simples();





    if (strcmp(token->Simbolo, "ssenao") == 0)
    {

        auxrot2 = rotulo;
        sprintf(rotString, "%d", rotulo);
        gera(" ", "JMP", rotString, " ");// ssenão
        rotulo++;
        flag = 1;

        lexico();
        printf("\n %s ", token->lexema);
        analisa_comando_simples();
    }

    sprintf(rotString, "%d", auxrot);
    gera(rotString, "NULL", " ", " ");


    if (flag == 1)
    {
       sprintf(rotString, "%d", auxrot2); //ssenão
        gera(rotString, "NULL", " ", " ");
    }


    printf("\n[analisa_se end]\n");
}

int analisa_expressao(){
    printf("\n[analisa_expressao]\n");
    char  operadores[6][10]= {"smaior","smaiorig","sig", "smenor", "smenorig","sdif"};
    int I;



    int res = analisa_expressao_simples();                // 0 = int, 1 = bool
    for (I = 0; I<6 ;I++)
    {
       if( strcmp(token->Simbolo, operadores[I]) == 0)
           break;
    }
    if (I!=6)
    {
        if (res != 0)
            return 2; //erro
        switch(I){
        case 0: insereOperador("CMA"); break;
        case 1: insereOperador("CMAQ"); break;
        case 2: insereOperador("CEQ"); break;
        case 3: insereOperador("CME"); break;
        case 4: insereOperador("CMEQ"); break;
        case 5: insereOperador("CDIF"); break;
        default: printf ("\n WTF erro\n"); break;
        }

        lexico();
        printf("\n %s ", token->lexema);
        if (analisa_expressao_simples() != 0)
            return 2; //erro
        res = 1;
    }
    printf("\n[analisa_expressao end]\n");
    return res;
}

int analisa_expressao_simples(){
    printf("\n[analisa_expressao_simples]\n");
    int aux = 1;
    int tipo;
    if(strcmp(token->Simbolo, "smais") == 0 || strcmp(token->Simbolo, "smenos") == 0)
        {
            if(strcmp(token->Simbolo, "smenos") == 0)
                insereOperador("INV");
            aux = 0;
            lexico();
            printf("\n %s ", token->lexema);
        }
    tipo = analisa_termo();
    if (tipo != 0 && aux == 0)
         {printf("\nerro linha %d, tipo  incompativel\n", lineCounter);return 2;} //erro, tipo incompativel

    while(strcmp(token->Simbolo, "smais") == 0 || strcmp(token->Simbolo, "smenos") == 0 || strcmp(token->Simbolo, "sou") == 0)
    {
        if (strcmp(token->Simbolo, "sou") == 0)
        {
           aux = 1;
           insereOperador("OR");
        } else if(strcmp(token->Simbolo, "smais") == 0)
        {
            aux = 0;
            insereOperador("ADD");

        }
        else {aux = 0;insereOperador("SUB");}


        if (aux != tipo)
            return 2; //erro
        lexico();
        printf("\n %s ", token->lexema);
        if (analisa_termo() != tipo)
            {printf("\nerro linha %d, tipo  incompativel\n", lineCounter);return 2;} // erro
    }
    printf("\n[analisa_expressao_simples end]\n");
    return tipo;
}

int analisa_termo(){
    printf("\n[analisa_termo]\n");
    int tipo = analisa_fator();
    while(strcmp(token->Simbolo, "smult") == 0 || strcmp(token->Simbolo, "sdiv") == 0 || strcmp(token->Simbolo, "se") == 0)
    {

        if(strcmp(token->Simbolo, "se") == 0)
        {
            //if (tipo == 0)
                 //{printf("\nerro teste em %s, tipo  incompativel\n", token->lexema);return 2;}
            insereOperador("AND");
        } else if(strcmp(token->Simbolo, "smult") == 0)
        {
            if (tipo == 1)
                 {printf("\nerro linha %d, tipo  incompativel\n", lineCounter);return 2;}
            insereOperador("MULT");
        } else {
            if (tipo == 1)
                 {printf("\nerro linha %d, tipo  incompativel\n", lineCounter);return 2;}
            insereOperador("DIVI");
        }

       // if ((tipo == 0 && strcmp(token->Simbolo, "se") == 0) || ((strcmp(token->Simbolo, "smult") == 0 || strcmp(token->Simbolo, "sdiv") == 0)&& tipo == -1))
        //    return 2; //erro, tipos incompativeis
        lexico();
        printf("\n %s ", token->lexema);
        if (analisa_fator() != tipo)
             {printf("\nerro linha %d, tipo  incompativel\n", lineCounter);return 2;}  //erro tipos não compatíveis
    }
    printf("\n[analisa_termo end]\n");
    return tipo;
}

int  analisa_fator(){
    printf("\n[analisa_fator]\n");
    int tipo = 0; // 0 = int, 1 = bool;
    int mem;
    char memString[5];
    if (strcmp(token->Simbolo, "sidentificador") == 0) {
        mem = pesquisa_funcao(token->lexema);
        if (mem != -1)
        {
            if (strcmp(confereTipo(token->lexema), "funcao booleana") == 0)
                tipo = 1;

            sprintf(memString,"%d", mem);
            gera("", "CALL", memString, "");
            gera("", "LDV", "0", "");

            lexico();
            printf("\n %s ", token->lexema);

        } else
        {
            mem = buscaVarDeclarado(token->lexema);
            if (mem != -1)
            {
                if (strcmp(confereTipo(token->lexema), "variavel booleano") == 0)
                    tipo = 1;

                sprintf(memString,"%d", mem);
                gera("","LDV", memString, "");

                lexico();
                printf("\n %s ", token->lexema);
            } else {printf("\terro semantico linha %d, identificador nao declarado\n", lineCounter);return 2;} //erro
        }
    }else if (strcmp(token->Simbolo, "snumero") == 0)
    {
        gera("", "LDC",token->lexema, "");
        lexico();
        printf("\n %s ", token->lexema);
    } else if (strcmp(token->Simbolo, "snao") == 0)
    {
        lexico();
        printf("\n %s ", token->lexema);
        tipo = analisa_fator();
        if (tipo != 1)
            return 2; // erro
    } else if (strcmp(token->Simbolo, "sabre_parenteses") == 0)
    {
        insereOperador("(");
        lexico();
        printf("\n %s ", token->lexema);
        tipo = analisa_expressao();
        if (strcmp(token->Simbolo, "sfecha_parenteses") == 0)
            {

                popParanteses();
                lexico();
                printf("\n %s ", token->lexema);}
        else {printf("\terro linha %d, \")\" esperado\n", lineCounter);return;} //\terro
    } else if (strcmp(token->Simbolo, "sverdadeiro") == 0 || strcmp(token->Simbolo, "sfalso") == 0)
        {
            if (strcmp(token->Simbolo, "sverdadeiro") == 0)
                gera("", "LDC", "1", "");
            else gera("", "LDC", "0", "");
            tipo = 1;
            lexico();
            printf("\n %s ", token->lexema);
        }
    else {printf("\terro linha %d, fator esperado\n", lineCounter);return;}//\terro
    printf("\n[analisa_fator end]\n");

    return tipo;
}

void analisa_subrotinas()
{
    printf("\n[analisa_subrotinas]\n");
    int auxrot, flag;
    char rotString[5];

    if (strcmp(token->Simbolo,"sprocedimento")==0|| strcmp(token->Simbolo,"sfuncao")==0)
    {

        auxrot = rotulo;
        sprintf(rotString, "%d", rotulo);
        gera(" ", "JMP", rotString, " ");
        rotulo++;
        flag = 1;

    }

    while(strcmp(token->Simbolo,"sprocedimento")==0|| strcmp(token->Simbolo,"sfuncao")==0)
    {
        if(strcmp(token->Simbolo,"sprocedimento")==0)
            analisa_declaracao_procedimento();
        else analisa_declaracao_funcao();
        if(strcmp(token->Simbolo,"sponto_virgula")==0)
            {lexico(); printf("\n %s ", token->lexema);}
        else {printf("\terro linha %d, \";\" esperado\n", lineCounter);return;}//\terro, ; esperado
    }
    if (flag== 1)
    {
        sprintf(rotString, "%d", auxrot);
        gera(rotString, "NULL", " ", " ");
    }

    printf("\n[analisa_subrotinas end]\n");
}

void analisa_declaracao_procedimento(){
    printf("\n[analisa_declaracao_procedimento]\n");
    char rotString[5];

    lexico();

    printf("\n %s ", token->lexema);
    if(strcmp(token->Simbolo,"sidentificador")==0){
        if(!buscaDuplicado(token->lexema,escopo_global)) {
            escopo_global++;
            insereTabela(token->lexema, "procedimento", escopo_global, rotulo);
            insereTabela("#", "marca", escopo_global, NULL);


            sprintf(rotString, "%d", rotulo);
            gera(rotString, "NULL", " ", " ");
            rotulo++;

            lexico();
            printf("\n %s ", token->lexema);
            if (strcmp(token->Simbolo, "sponto_virgula") == 0)
                analisa_bloco();
            else {
                printf("\terro linha %d, \";\" esperado\n", lineCounter);
                return;
            }

            Desempilha();
            gera("", "RETURN", "", "");
            escopo_global--;
        }
        else{
            printf("erro linha %d, procedimentos com nomes iguais", lineCounter);
            return;
        }
    }
    else {printf("\terro linha %d, identificador esperado\n", lineCounter);return;}//\terro

    printf("\n[analisa_declaracao_procedimento end]\n");

}
void analisa_declaracao_funcao(){
    printf("\n[analisa_declaracao_funcao]\n");
    char rotString[5];

    lexico();
    printf("\n %s ", token->lexema);
    if(strcmp(token->Simbolo,"sidentificador")==0){
        if(!(buscaDuplicado(token->lexema,escopo_global))) {

            escopo_global++;
            insereTabela(token->lexema, "", escopo_global, rotulo);


            sprintf(rotString, "%d", rotulo);
            gera(rotString, "NULL", " ", " ");
            rotulo++;

            lexico();
            printf("\n %s ", token->lexema);
            if (strcmp(token->Simbolo, "sdoispontos") == 0) {
                lexico();
                printf("\n %s ", token->lexema);
                if (strcmp(token->Simbolo, "sinteiro") == 0 || strcmp(token->Simbolo, "sbooleano") == 0) {
                    if(strcmp(token->Simbolo, "sinteiro") == 0)
                        strcpy(inicio->topo->tipo, "funcao inteiro"); //TS->tipo = "funcao inteiro";
                    else
                        strcpy(inicio->topo->tipo, "funcao booleana"); //TS->tipo = "funcao booleana";
                    insereTabela("#", "marca", escopo_global, NULL);

                    lexico();
                    printf("\n %s ", token->lexema);
                    if (strcmp(token->Simbolo, "sponto_virgula") == 0)
                        analisa_bloco();
                    else {
                        printf("\terro linha %d, \";\" esperado\n", lineCounter);
                        return;
                        }
                } else {
                    printf("\terro linha %d, tipo nao reconhecido\n", lineCounter);
                    return;
                }//\terro
                Desempilha();
                gera("", "RETURN", "", "");
                escopo_global--;
            } else {
                printf("\terro linha %d, \":\" esperado\n", lineCounter);
                return;
            }//\terro
        }
        else{
            printf("ERRO, identificador já utilizado");
        }
    }else {printf("\terro linha %d, identificador esperado\n", lineCounter);return;}//\terro
    printf("\n[analisa_declaracao_funcao end]\n");

}

//=======================================================================================================================================
// semantico
//=======================================================================================================================================

//Função que inicia a pilha
void iniciaPilha(){
    Topo *aux = (Topo*)malloc(sizeof (Topo));
    inicio = aux;
    inicio->topo = NULL;
    printf("\n\n%s",inicio->topo);
}

//função que insere na tabela de simbolos(não consigo explicar ao certo ainda o que a variavel mem, prorem vou ver isso com o daniel dps, só sei que tem a ver com a VM
void insereTabela(char lexema[], char nometabela[], int nivel, void* mem){
    tabelaSimbolos *aux = (tabelaSimbolos*)malloc(sizeof (tabelaSimbolos));
    printf ("\n  == testing yo %s  %d==  \n", lexema, strlen(aux->lexema));
    strcpy(aux->lexema,lexema);
    strcpy(aux->tipo,nometabela);

    aux->nivel = nivel;
    aux->mem = mem;
    aux->prev = inicio->topo;
    inicio->topo = aux;
    printPilha();


}
//função que ira retirar os simbolos da pilha ate encontrar uma marca
void Desempilha(){
    tabelaSimbolos *aux = inicio->topo;
    tabelaSimbolos *aux1;


    while(strcmp(aux->lexema, "#")){
        aux1 = aux->prev;
        free(aux);
        aux = aux1;

    }

    if(!strcmp(aux->lexema, "#")){
        inicio->topo = aux->prev;
        free(aux);
    }
    else
        inicio->topo = aux;
}
//função que buscará por duplicantes. Retorna: false caso não haja duplicantes/ true no caso de duplicantes no mesmo ou mais baixo escopo
bool buscaDuplicado(char lexema[], int nivel){
    tabelaSimbolos *aux = inicio->topo;
    do{
        if(!strcmp(lexema, aux->lexema)){
            if(aux->nivel <= nivel)
                return true;
        }
        aux = aux->prev;
    } while (aux->nivel != -1);

    return false;
}
//função que buscará por duplicantes. Retorna: false caso não haja duplicantes/ true no caso de duplicantes variáveis no mesmo nível ou algo além de váriavel em nível mais baixo
bool buscaVarDuplicado(char lexema[], int nivel){
    tabelaSimbolos *aux = inicio->topo;

    do{
        if(strcmp(aux->lexema, lexema) == 0)
        {
            if ((strcmp(aux->tipo, "variavel inteiro" ) == 0 || strcmp(aux->tipo, "variavel booleano" ) == 0 || strcmp(aux->tipo, "variavel" ) == 0))
            {
                if(aux->nivel == nivel)
                    return true;
            } else if ( aux->nivel <= nivel)
                    return true;
        }

        aux = aux->prev;

    } while (strcmp(aux->tipo,"final") != 0);

    printf (" testing yo duplic: %s  ", lexema);
    return false;
}

int buscaVarDeclarado(char lexema[]){
    tabelaSimbolos *aux = inicio->topo;

    do{
        if(strcmp(aux->lexema, lexema) == 0 && (strcmp(aux->tipo, "variavel inteiro" ) == 0 || strcmp(aux->tipo, "variavel booleano" ) == 0))
            return aux->mem;
        aux = aux->prev;
    }while(aux->nivel != -1);
    return -1;
}

void defineTipoVar(char* tipo){
    tabelaSimbolos *aux = (tabelaSimbolos*)malloc(sizeof (tabelaSimbolos));
    tabelaSimbolos *aux2 = (tabelaSimbolos*)malloc(sizeof (tabelaSimbolos));
    char tipoFull[20] = "variavel ";
    strcat(tipoFull,tipo);
    int c =0;
    do{

        aux2 = aux;
        aux = (tabelaSimbolos*)malloc(sizeof (tabelaSimbolos));
        strcpy(aux->lexema, inicio->topo->lexema);
        aux->nivel = inicio->topo->nivel;
        aux->mem = inicio->topo->mem;
        aux->prev = aux2;

        inicio->topo = inicio->topo->prev;
        c++;

    }while(strcmp(inicio->topo->tipo,"variavel") == 0);

    do {
       insereTabela(aux->lexema, tipoFull,aux->nivel, aux->mem);
       aux= aux->prev;
       c--;
    } while (c>0);
}

int pesquisa_procedimento(char *id){
    tabelaSimbolos *aux = inicio->topo;

    do{
        if(strcmp(aux->lexema, id) == 0 && strcmp(aux->tipo, "procedimento") == 0)
            return aux->mem;
      aux = aux->prev;
    }while(aux != NULL);
    return -1;
}

int pesquisa_funcao(char *id){
    tabelaSimbolos *aux = inicio->topo;

    do{
        if(strcmp(aux->lexema, id) == 0 && (strcmp(aux->tipo, "funcao inteiro") == 0 || strcmp(aux->tipo, "funcao booleana") == 0))
            return aux->mem;
      aux = aux->prev;
    }while(aux != NULL);

    return -1;
}

int pesquisa_atrib_funcao(char *id){
    tabelaSimbolos *aux = inicio->topo;

    do{
        if(strcmp(aux->lexema, id) == 0 && aux->nivel == escopo_global)
        {

            if (strcmp(aux->tipo, "funcao inteiro") == 0)
                return 0;
            else if (strcmp(aux->tipo, "funcao booleana") == 0)
                return 1;
        }
      aux = aux->prev;
    }while(aux != NULL);

    return -1;
}


char* confereTipo(char* lexema){
    tabelaSimbolos *aux = inicio->topo;
    char resp[20];
    do{
        if(strcmp(aux->lexema, lexema) == 0){

            //strcpy(resp, aux->tipo);
            //break;
            return aux->tipo;

        }
        aux = aux->prev;
    }while(aux != NULL);
    return resp;

}

//printa a tabela de simbolos completa
void printPilha(){
    tabelaSimbolos *aux = inicio->topo;
    do{
        printf("\n\n\t\tLexema:%s",aux->lexema);
        printf("\n\n\t\tTipo:%s",aux->tipo);
        printf("\n\n\t\tNivel:%d",aux->nivel);
        printf("\n\n\t\tmem:%d",aux->mem);
        printf("\n\n\t\t/--------/");
        aux = aux->prev;
    } while (aux != NULL);

}


//=======================================================================================================================================
// geração de código
//=======================================================================================================================================

void iniciaPosFixa(){
    psTopo *aux = (psTopo*)malloc(sizeof (psTopo));
    psInicio = aux;
    psInicio->topo = NULL;
    printf("\n\n%s",psInicio->topo);
}

// insere na pilha pos fixa
void inserePosFixa(char valor[], int prec){
    psPilha *aux = (psPilha*)malloc(sizeof (psPilha));
    printf ("\n  == colocando na pilha %s  %d==  \n", valor, strlen(aux->valor));

    strcpy(aux->valor,valor);
    aux->precedencia = prec;

    aux->prev = psInicio->topo;
    psInicio->topo = aux;
    printPosFixa();
}

// popa e coloca na saida
char* popPosFixa()
{
    psPilha *aux = psInicio->topo;
    psPilha *aux1;
    char *res = malloc(6);


    strcpy(res, aux->valor);
    printf ("\n teste fator %s\n", aux->valor);

    aux1 = aux->prev;
    free(aux);
    aux = aux1;
    psInicio->topo = aux;

    return res;
}

void insereOperador(char op[])
{
    int precedencia = -1;

    char relacionais[6][6] = {"CME", "CMA", "CEQ", "CDIF", "CMEQ", "CMAQ"};

    if(strcmp(op, "(") == 0)
        inserePosFixa(op, -1);
    else{

        if (strcmp(op,"INV") == 0)
            precedencia = 6;
        else if (strcmp(op,"MULT") == 0 || strcmp(op,"DIVI") == 0)
            precedencia = 5;
        else if (strcmp(op,"ADD") == 0 || strcmp(op,"SUB") == 0)
            precedencia = 4;
        else if (strcmp(op,"NEG") == 0)
            precedencia = 2;
        else if (strcmp(op,"AND") == 0)
            precedencia = 1;
        else if ( strcmp(op,"OR") == 0)
            precedencia = 0;
        else{
            for (int I = 0; I< 6; I++)
            {
                if (strcmp(relacionais[I], op) == 0)
                {
                    precedencia = 3;
                    break;
                }
            }
        }
        if (precedencia == -1)
        {
            printf("\n WTF DUDE \n");
        }

        while (psInicio->topo != NULL && psInicio->topo->precedencia >= precedencia && strcmp(psInicio->topo->valor, "(") != 0)
        {

            printf ("\n testing yo: %s e %s\n", op, psInicio->topo->valor);
            gera("", popPosFixa(), "", "");
        }
        inserePosFixa(op, precedencia);

    }
}

//popa até o final dos parenteses
void popParanteses ()
{

    while(psInicio->topo != NULL && strcmp(psInicio->topo->valor, "(") != 0)
    {
        gera("", popPosFixa(),"","");

    }
    popPosFixa();
}

void popAll()
{
    while(psInicio->topo != NULL)
    {
        if (strcmp(psInicio->topo->valor, "(") != 0)
            gera("", popPosFixa(), "", "");
        else popPosFixa();

    }
}



void gera(char *rotulo, char *comando, char *param1, char *param2)
{

    fprintf ( codigo, "%s\t%s\t%s\t%s\t\n", rotulo, comando, param1, param2);
}

void printPosFixa(){
    psPilha *aux = psInicio->topo;
    do{
        printf("\n\n\t\tvalor:%s",aux->valor);
        printf("\n\n\t\tpreced:%d",aux->precedencia);
        printf("\n\n\t\t/--------/");
        aux = aux->prev;
    } while (aux != NULL);

}
