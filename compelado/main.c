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

typedef struct ns{
    char* tipo;
    char* simbolo;
    int escopo;
}noSimbolo;

typedef struct pilha{
    struct pilha *prev;
    noSimbolo simbolo;
} tabelaSimbolos;

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~* CABEÇALHOS *~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*/

tipoToken *createToken(char* lex, char* sim);

void tratarIdentificador();

void tratarDigito();

void tratarOperador();

void lexico();

void analisador();

void analisa_bloco();

void et_analisa_var();

void analisa_var();

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

noSimbolo criaNo();

tabelaSimbolos* criaPilha();

noSimbolo pushNo(char* simbolos, char* tipo, int escopo);

void insereTabela(tabelaSimbolos **pilha, char* lexema, char* tipo, int nivel, int rotulo);

noSimbolo popPilha(tabelaSimbolos **pilha);

bool scanPilha(char* simbolo);

void defineTipoVar(char* tipo);

void defineTipoFun(char* tipo);

char* confereTipo(char* simbolo);

bool pesquisa_duplicvar(char* simbolo);

bool pesquisa_declvar(char* simbolo);

bool pesquisa_procedimento(char *funcao);

bool pesquisa_funcao(char *funcao);

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~* VARIAVEIS *~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*/

char text[2048];

FILE *arquivo;

tipoToken* token = NULL;

tabelaSimbolos* TS;


int _index_ = 0;

int escopo_global = 0;


/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~* MAIN *~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*/

int main(){
    char line[LINE_LENGTH];
    //O endereço deve ser alterado para o adequado SEMPRE
    //arquivo=fopen("C:/Users/nucle/OneDrive/Documentos/GitHub/Compilador-em-C/compelado/sint1.txt","r");
    //arquivo=fopen("/home/luckytods/CLionProjects/Compilador-em-C/compelado/gera1.txt","r");
    arquivo=fopen("C:/Users/19088582/Downloads/Compilador-em-C-main/compelado/sint10.txt","r");
    if(arquivo == NULL) {
        printf("ERRO");
        exit(1);
    }

    TS = NULL;

    while(fgets(line,LINE_LENGTH,arquivo)){
        strcat(text, line);
        memset(line, 0, sizeof(line));
    }
    analisador();
    printf("\nanalise sintatica concluida\n");

    fclose(arquivo);
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
            token = createToken( ID, s);
            return;
        }
    }

    token = createToken( ID, "sidentificador");
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
    token = createToken( ID, "snumero");
}

void tratarOperador(){
    char ID[30] = {0};
    char c[2];
    c[1] = '\0';
    c[0] = text[_index_];
    _index_++;
    switch(c[0]){
        case '+': token = createToken( c, "smais"); break;
        case '-': token = createToken( c, "smenos"); break;
        case '*': token = createToken( c, "smult"); break;
        case ';': token = createToken( c, "sponto_virgula"); break;
        case ',': token = createToken( c, "svirgula"); break;
        case '.': token = createToken( c, "sponto"); break;
        case '(': token = createToken( c, "sabre_parenteses"); break;
        case ')': token = createToken( c, "sfecha_parenteses"); break;
        case '=': token = createToken( c, "sig"); break;
        case '!':
            if (text[_index_] != '=')
                printf("\terro, caractere %s nao esperado, esperava \"=\"\n", text[_index_]);
            else
                {
                    _index_++;
                    token = createToken( "!=", "sdif");

                }
            break;
        case ':':
            if (text[_index_] != '=')
                token = createToken( c, "sdoispontos");
            else
                {
                    _index_++;
                    token = createToken( ":=", "satribuicao");
                }
            break;
        case '<':
            if (text[_index_] != '=')
                token = createToken( c, "smenor");
            else
                {
                    _index_++;
                    token = createToken( "<=", "smenorig");
                }
            break;
        case '>':
            if (text[_index_] != '=')
                token = createToken( c, "smaior");
            else
            {
                _index_++;
                token = createToken( ">=", "smaiorig");
            }
            break;
        default: printf("erro, Operador %s nao identificado \n", c); break;
    }
}

void lexico(){
    char c = text[_index_];
        if (c == 0)
            token = NULL;

        else if (c == '{')
        {
            while (c!= '}' && c != 0)
            {
                _index_++;
                c = text[_index_];
                if (c == 0)
                    printf("erro, comentario sem fim\n");
            }
            _index_++;
            lexico();
        } else if ((c >= 65 && c <=90)|| (c >= 97 && c <= 122))
            tratarIdentificador();
        else if (c >=48 && c <= 57)
            tratarDigito();
        else if (c == 32 || c == 10 || c == 13)
        {
            _index_++;
            lexico();
        }
        else
            tratarOperador();
        c = text[_index_];

}

void analisador(){
    printf("\n[analise]\n");
    lexico();

    if (strcmp(token->Simbolo, "sprograma") == 0)
        {lexico(); printf("\n %s ", token->lexema);}
    else {printf("\terro em %s, \"programa\" esperado, %s recebido\n", token->lexema, token->Simbolo);return;}//\terro não tem programa

    if (strcmp(token->Simbolo, "sidentificador") == 0){

        insereTabela(&TS, token->lexema,"nomedeprograma", escopo_global, 0);
        if (TS == NULL)
            printf(" yes ");
        else printf(" no ");

        lexico(); printf("\n %s ", token->lexema);
        }
    else {printf("\terro em %s, identificador esperado\n", token->lexema, token->Simbolo);return;}//\terro falta identificador
    if (strcmp(token->Simbolo, "sponto_virgula") == 0)
        analisa_bloco();
    else  {printf("\terro em %s, \";\" esperado\n", token->lexema);return;}//\terro ponto_virg
    if (strcmp(token->Simbolo, "sponto") == 0)
    {
        lexico();
        if (token == NULL)
            return; // sucesso
        else {printf("\terro em %s, final de codigo esperado\n", token->lexema);return;}// \terro
    } else {printf("\terro em %s, \".\" esperado\n", token->lexema);return;} //\terro
}

void analisa_bloco(){
    printf("\n[analisa_bloco]\n");
    lexico();
    printf("\n %s ", token->lexema);
    et_analisa_var();
    analisa_subrotinas();
    analisa_comandos();
    printf("\n[analisa_bloco end]\n");
}


void et_analisa_var(){
    printf("\n[et_analisa_var]\n");
    if(strcmp(token->Simbolo,"svar")== 0)
    {lexico(); printf("\n %s ", token->lexema);//\terro
        if(strcmp(token->Simbolo,"sidentificador")== 0)
        {
            while(strcmp(token->Simbolo,"sidentificador")== 0)
            {
                analisa_var();
                if (strcmp(token->Simbolo,"sponto_virgula")== 0)
                    {lexico(); printf("\n %s ", token->lexema);}
                else  {printf("\terro em %s, \";\" esperado\n", token->lexema);return;} //\terro ; esperado
            }
        }else {printf("\terro em %s, identificador esperado\n", token->lexema);return;}// \terro falta identificador
        printf("\n[et_analisa_var end]\n");
    }
}

void analisa_var(){
    printf("\n[analisa_var]\n");
    do{
        if(strcmp(token->Simbolo,"sidentificador")== 0){
            if (!(pesquisa_duplicvar(token->lexema))) {

                insereTabela(TS,token->lexema, "variavel", escopo_global, 0);
                lexico();
                printf("\n %s ", token->lexema);
            }
            else{
                printf("ERRO, nome de variavel ja utilizado");
                return;
            }
        }
        else {printf("\terro em %s, identificador esperado\n", token->lexema);return;}
        if(strcmp(token->Simbolo,"svirgula")== 0 || strcmp(token->Simbolo,"sdoispontos")== 0)
        {
            if (strcmp(token->Simbolo,"svirgula")== 0)
            {
                lexico();
                printf("\n %s ", token->lexema);
                if (strcmp(token->Simbolo,"sdoispontos")== 0)
                    {printf("\terro em %s, identificador esperado\n", token->lexema);return;}//\terro, identificador espserado
            }
        } else {printf("\terro em %s, \",\" ou \":\" esperado\n", token->lexema);return;} //\terro , ou : esperado
    }while  (strcmp(token->Simbolo,"sdoispontos") != 0);
    lexico();
    printf("\n %s ", token->lexema);
    analisa_tipo();
    printf("\n[analisa_var end]\n");
}

void analisa_tipo(){
    printf("\n[analisa_tipo]\n");
    if (strcmp(token->Simbolo,"sinteiro")!=0 && strcmp(token->Simbolo,"sbooleano")!=0){
        printf("\terro em %s, tipo nao reconhecido\n", token->lexema);
        return;
    }//\terro

    defineTipoVar(token->lexema);

    lexico();
    printf("\n %s ", token->lexema);
    printf("\n[analisa_tipo end]\n");
}

void analisa_comandos(){
    printf("\n[analisa_comandos]\n");
    if (strcmp(token->Simbolo, "sinicio") != 0)
     {printf("\terro em %s, inicio esperado\n", token->lexema);return;} //\terro, inicio esperado
    lexico();
    printf("\n %s ", token->lexema);
    analisa_comando_simples();
    while(strcmp(token->Simbolo,"sfim") != 0)
    {
        if (strcmp(token->Simbolo, "sponto_virgula") == 0)
        {
            lexico();
            printf("\n %s ", token->lexema);
            if (strcmp(token->Simbolo,"sfim") != 0)
                analisa_comando_simples();
        } else {printf("\terro em %s, \";\" esperado\n", token->lexema);return;} //\terro ; esperado
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
/*
void analisa_atrib_chprocedimento(){
    printf("\n[analisa_atrib_chprocedimento]\n");
    //pode ser melhor checar aqui se o identificador é um procedimento, se não, passar pro analisa_atribuição, talvez até fazer o analisa_chamada_procedimento aki;
    analisa_chamada_procedimento();

    lexico();
    printf("\n %s ", token->lexema);



    if (strcmp(token->Simbolo, "satribuicao") == 0)
    {
        analisa_atribuicao();
    }
    printf("\n[analisa_atrib_chprocedimento end]\n");
}

void analisa_atribuicao(){
    printf("\n[analisa_atribuicao]\n");
    lexico();
    printf("\n %s ", token->lexema);
    //semantic shit
    analisa_expressao();
    printf("\n[analisa_atribuicao end]\n");
}
*/

void analisa_atrib_chprocedimento(){
    printf("\n[analisa_atrib_chprocedimento]\n");
    //pode ser melhor checar aqui se o identificador é um procedimento, se não, passar pro analisa_atribuição, talvez até fazer o analisa_chamada_procedimento aki;
    if (!pesquisa_procedimento(token->lexema))
        analisa_atribuicao();
    else
    {
    //gera codigo
    }

    lexico();
    printf("\n %s ", token->lexema);
    printf("\n[analisa_atrib_chprocedimento end]\n");
}

void analisa_atribuicao(){
    printf("\n[analisa_atribuicao]\n");

    if (!pesquisa_declvar(token->lexema))
        return;// erro, identificador inválido

    char *tipo = confereTipo(token->lexema);
    lexico();
    printf("\n %s ", token->lexema);
    if (strcmp(token->Simbolo, "satribuicao") != 0)
        return; //erro, ':=' esperado

    lexico();
    printf("\n %s ", token->lexema);
    int exp_tipo = analisa_expressao();
    if (exp_tipo == 2)
        return; //erro
    if ((strcmp(tipo, "variavel booleano") == 0 && exp_tipo == -1)||(strcmp(tipo, "variavel inteiro") == 0 && exp_tipo == 0))
    {
        printf("\n[analisa_atribuicao end]\n");
        return; // sem erro
    }

    return; //expressão incompatível com váriavel
}

void analisa_leia(){
    printf("\n[analisa_leia]\n");
    lexico();
    printf("\n %s ", token->lexema);
    if (strcmp(token->Simbolo, "sabre_parenteses") == 0)
        {lexico(); printf("\n %s ", token->lexema);}
    else {printf("\terro em %s, \"(\" esperado\n", token->lexema);return;}//\terro
    if (strcmp(token->Simbolo, "sidentificador") == 0){
        if(pesquisa_declvar(token->lexema)) {
            if (strcmp(confereTipo(token->lexema), "variavel inteiro") == 0)
            {
            lexico();
            printf("\n %s ", token->lexema);
            } else return;; //erro, variável inválida
        }
        else{
            printf("ERRO, variavel não foi encontrada");
            return;
        }
    }
    else {printf("\terro em %s, identificador esperado\n", token->lexema);return;}//\terro
    if (strcmp(token->Simbolo, "sfecha_parenteses") == 0)
        {lexico(); printf("\n %s ", token->lexema);}
    else {printf("\terro em %s, \")\" esperado\n", token->lexema);return;}//\terro
    printf("\n[analisa_leia end]\n");
}

void analisa_escreva(){
    printf("\n[analisa_escreva]\n");
    lexico();
    printf("\n %s ", token->lexema);
    if (strcmp(token->Simbolo, "sabre_parenteses") == 0)
        {lexico(); printf("\n %s ", token->lexema);}
    else {printf("\terro em %s, \"(\" esperado\n", token->lexema);return;}//\terro
    if (strcmp(token->Simbolo, "sidentificador") == 0){
        if(pesquisa_declvar(token->lexema)) {
           if (strcmp(confereTipo(token->lexema), "variavel inteiro") == 0)
            {
            lexico();
            printf("\n %s ", token->lexema);
            } else return;; //erro, variável inválida
        }
        else{
            printf("ERRO, variavel não encontrada");
            return;
        }
    }
    else {printf("\terro em %s, identificador esperado\n", token->lexema);return;}//\terro

    if (strcmp(token->Simbolo, "sfecha_parenteses") == 0)
        {lexico(); printf("\n %s ", token->lexema);}
    else {printf("\terro em %s, \")\" esperado\n", token->lexema);return;}//\terro
    printf("\n[analisa_escreva end]\n");
}

void analisa_enquanto(){
    printf("\n[analisa_enquanto]\n");
    lexico();
    printf("\n %s ", token->lexema);
    if (analisa_expressao() != -1)
        return; //erro, não é bool
    if (strcmp(token->Simbolo, "sfaca") == 0)
        {lexico(); printf("\n %s ", token->lexema);}
    else {printf("\terro em %s, faca esperado\n", token->lexema);return;} //\terro
    analisa_comando_simples();
    printf("\n[analisa_enquanto end]\n");
}

void analisa_se(){
    printf("\n[analisa_se]\n");
    lexico();
    printf("\n %s ", token->lexema);
    if (analisa_expressao() != -1)
        return; //erro, não é bool
    if (strcmp(token->Simbolo, "sentao") == 0)
        {lexico(); printf("\n %s ", token->lexema);}
    else {printf("\terro em %s, entao esperado\n", token->lexema);return;} //\terro
    analisa_comando_simples();
    if (strcmp(token->Simbolo, "ssenao") == 0)
    {
        lexico();
        printf("\n %s ", token->lexema);
        analisa_comando_simples();
    }
    printf("\n[analisa_se end]\n");
}

int analisa_expressao(){
    printf("\n[analisa_expressao]\n");

    int res = analisa_expressao_simples();                // 0 = int, -1 = bool
    if (strcmp(token->Simbolo, "smaior") == 0 || strcmp(token->Simbolo, "smaiorig") == 0 || strcmp(token->Simbolo, "sig") == 0 || strcmp(token->Simbolo, "smenor") == 0 || strcmp(token->Simbolo, "smenorig") == 0 || strcmp(token->Simbolo, "sdif") == 0 )
    {
        if (res != -1)
            return 2; //erro
        lexico();
        printf("\n %s ", token->lexema);
        if (analisa_expressao_simples() != -1)
            return 2; //erro
    }
    printf("\n[analisa_expressao end]\n");
    return res;
}

int analisa_expressao_simples(){
    printf("\n[analisa_expressao_simples]\n");
    int tipo = -1;
    if(strcmp(token->Simbolo, "smais") == 0 || strcmp(token->Simbolo, "smenos") == 0)
        {   tipo = 0;
            lexico();
            printf("\n %s ", token->lexema);
        }
    if (tipo == 0 && analisa_termo() != 0)
        return 2; //erro, tipo incompativel

    while(strcmp(token->Simbolo, "smais") == 0 || strcmp(token->Simbolo, "smenos") == 0 || strcmp(token->Simbolo, "sou") == 0)
    {
        if (strcmp(token->Simbolo, "sou") == 0)
            tipo = -1;
        else tipo = 0;
        lexico();
        printf("\n %s ", token->lexema);
        if (analisa_termo() != tipo)
            return 2; // erro
    }
    return tipo;
    printf("\n[analisa_expressao_simples end]\n");
}

int analisa_termo(){
    printf("\n[analisa_termo]\n");
    int tipo = analisa_fator();
    while(strcmp(token->Simbolo, "smult") == 0 || strcmp(token->Simbolo, "sdiv") == 0 || strcmp(token->Simbolo, "se") == 0)
    {
        if ((tipo == 0 && strcmp(token->Simbolo, "se") == 0) || ((strcmp(token->Simbolo, "smult") == 0 || strcmp(token->Simbolo, "sdiv") == 0)&& tipo == -1))
            return 2; //erro, tipos incompativeis
        lexico();
        printf("\n %s ", token->lexema);
        if (analisa_fator() != tipo)
            return 2; //erro tipos não compatíveis
    }
    printf("\n[analisa_termo end]\n");
    return tipo;
}

int  analisa_fator(){
    printf("\n[analisa_fator]\n");
    int tipo = 0; // 0 = int, -1 = bool;
    if (strcmp(token->Simbolo, "sidentificador") == 0) {

        if (pesquisa_funcao(token->lexema))
        {
            if (strcmp(confereTipo(token->lexema), "funcao booleano") == 0)
                tipo = -1;
            lexico();
            printf("\n %s ", token->lexema);

        } else if (pesquisa_declvar(token->lexema))
        {
            if (strcmp(confereTipo(token->lexema), "variavel booleano") == 0)
                tipo = -1;
            lexico();
            printf("\n %s ", token->lexema);
        } else return 2; //erro
    }else if (strcmp(token->Simbolo, "snumero") == 0)
    {
        lexico();
        printf("\n %s ", token->lexema);
    } else if (strcmp(token->Simbolo, "snao") == 0)
    {
        lexico();
        printf("\n %s ", token->lexema);
        tipo = analisa_fator();
        if (tipo != -1)
            return 2; // erro
    } else if (strcmp(token->Simbolo, "sabre_parenteses") == 0)
    {
        lexico();
        printf("\n %s ", token->lexema);
        tipo = analisa_expressao();
        if (strcmp(token->Simbolo, "sfecha_parenteses") == 0)
            {lexico(); printf("\n %s ", token->lexema);}
        else {printf("\terro em %s, \")\" esperado\n", token->lexema);return;} //\terro
    } else if (strcmp(token->Simbolo, "sverdadeiro") == 0 || strcmp(token->Simbolo, "sfalso") == 0)
        {
            tipo = -1;
            lexico();
            printf("\n %s ", token->lexema);
        }
    else {printf("\terro em %s, verdadeiro ou falso esperado\n", token->lexema);return;}//\terro
    printf("\n[analisa_fator end]\n");

    return tipo;
}
/*
void analisa_chamada_procedimento(){
    printf("\n[analisa_chamada_procedimento]\n");
    if(pesquisa_procedimento(token->lexema)){
        //gera código shiet
    }
    else {
        printf("ERRO, procedimento nao existe");
        return;
    }
    printf("\n[analisa_chamada_procedimento end]\n");
}*/
/*
void analisa_chamada_funcao(){
    printf("\n[analisa_chamada_funcao]\n");
    if(pesquisa_funcao(token->lexema)){
        //gera código shiet
    }
    else {
        printf("ERRO, funcao nao existe");
        return;
    }
    lexico();
    printf("\n %s ", token->lexema);
    printf("\n[analisa_chamada_funcao end]\n");
}*/

void analisa_subrotinas()
{
    printf("\n[analisa_subrotinas]\n");
    while(strcmp(token->Simbolo,"sprocedimento")==0|| strcmp(token->Simbolo,"sfuncao")==0)
    {
        if(strcmp(token->Simbolo,"sprocedimento")==0)
            analisa_declaracao_procedimento();
        else analisa_declaracao_funcao();
        if(strcmp(token->Simbolo,"sponto_virgula")==0)
            {lexico(); printf("\n %s ", token->lexema);}
        else {printf("\terro em %s, \";\" esperado\n", token->lexema);return;}//\terro, ; esperado
    }
    printf("\n[analisa_subrotinas end]\n");
}

void analisa_declaracao_procedimento(){
    printf("\n[analisa_declaracao_procedimento]\n");
    lexico();
    escopo_global++;
    printf("\n %s ", token->lexema);
    if(strcmp(token->Simbolo,"sidentificador")==0){
        if(!(pesquisa_procedimento(token->lexema))) {
            //insere tabela
            insereTabela(TS, token->lexema, "procedimento", escopo_global, 0);
            lexico();
            printf("\n %s ", token->lexema);
            if (strcmp(token->Simbolo, "sponto_virgula") == 0)
                analisa_bloco();
            else {
                printf("\terro em %s, \";\" esperado\n", token->lexema);
                return;
            }
        }
        else{
            printf("ERRO, procedimentos com nomes iguais");
            return;
        }
    }
    else {printf("\terro em %s, identificador esperado\n", token->lexema);return;}//\terro
    printf("\n[analisa_declaracao_procedimento end]\n");
    escopo_global--;
}
void analisa_declaracao_funcao(){
    printf("\n[analisa_declaracao_funcao]\n");
    lexico();
    escopo_global++;
    printf("\n %s ", token->lexema);
    if(strcmp(token->Simbolo,"sidentificador")==0){
        if(!(scanPilha(token->lexema))) {
            //InsereTabela
            insereTabela(TS, token->lexema, "", escopo_global, 0);
            lexico();
            escopo_global++;
            printf("\n %s ", token->lexema);
            if (strcmp(token->Simbolo, "sdoispontos") == 0) {
                lexico();
                printf("\n %s ", token->lexema);
                if (strcmp(token->Simbolo, "sinteiro") == 0 || strcmp(token->Simbolo, "sbooleano") == 0) {
                    if(token->Simbolo == "Sinteiro")
                        TS->simbolo.tipo = "funcao inteiro";
                    else
                        TS->simbolo.tipo = "funcao booleana";
                    lexico();
                    printf("\n %s ", token->lexema);
                    if (strcmp(token->Simbolo, "sponto_virgula") == 0)
                        analisa_bloco();
                } else {
                    printf("\terro em %s, tipo nao reconhecido\n", token->lexema);
                    return;
                }//\terro
            } else {
                printf("\terro em %s, \":\" esperado\n", token->lexema);
                return;
            }//\terro
        }
        else{
            printf("ERRO, identificador já utilizado");
        }
    }else {printf("\terro em %s, identificador esperado\n", token->lexema);return;}//\terro
    printf("\n[analisa_declaracao_funcao end]\n");
    escopo_global--;
}

//Função cria e prepara um nó para iniciar uma pilha
noSimbolo criaNo(){
    noSimbolo novoNo;
    novoNo.escopo = NULL;
    novoNo.simbolo = (char*) malloc(sizeof(char));
    novoNo.simbolo = NULL;
    novoNo.tipo = (char*) malloc(sizeof(char));
    novoNo.tipo = NULL;
    return novoNo;

}
//Cria uma nova pilha
tabelaSimbolos* criaPilha(){
    tabelaSimbolos* pilhaNova = (tabelaSimbolos*) malloc(sizeof(tabelaSimbolos));
    pilhaNova->simbolo = criaNo();
    pilhaNova->prev = NULL;
    return pilhaNova;
}

noSimbolo pushNo(char* simbolos, char* tipo, int escopo){
    noSimbolo novoNo;
    novoNo.escopo = escopo;
    novoNo.simbolo = (char*) malloc(sizeof(char));
    novoNo.simbolo = simbolos;
    novoNo.tipo = (char*) malloc(sizeof(char));
    novoNo.tipo = tipo;
    return novoNo;
}

void insereTabela(tabelaSimbolos **pilha, char* lexema, char* tipo, int nivel, int rotulo){
    tabelaSimbolos* aux = (tabelaSimbolos*) malloc(sizeof(tabelaSimbolos));

    if(pilha == NULL)
    {
        *pilha = criaPilha();
        (*pilha)->simbolo = pushNo(lexema, tipo, nivel);

    }
    else{
        aux->simbolo = pushNo(lexema, tipo, nivel);
        aux->prev = *pilha;
        *pilha = aux;
    }

    free(aux);
}

noSimbolo popPilha(tabelaSimbolos **pilha){
    noSimbolo aux;



    if((*pilha)->prev == NULL){

        aux = (*pilha)->simbolo;
        *pilha = NULL;
        return aux;
    }

    aux = (*pilha)->simbolo;
    *pilha = (*pilha)->prev;
    return aux;

}

bool scanPilha(char* simbolo){
    tabelaSimbolos *aux = (tabelaSimbolos*) malloc(sizeof(tabelaSimbolos));
    noSimbolo noAux;
    bool resp = false;

    do{
        noAux = popPilha(&TS);
        if(strcmp(noAux.simbolo, simbolo) == 0){
            insereTabela(TS,noAux.simbolo,noAux.tipo, noAux.escopo, 0);
            resp = true;
            break;
        }
        insereTabela(aux,noAux.simbolo,noAux.tipo, noAux.escopo, 0);
    }while(TS->simbolo.escopo != NULL);
    do{
        noAux = popPilha(aux);
        insereTabela(TS,noAux.simbolo,noAux.tipo, noAux.escopo, 0);
    }while(aux->simbolo.escopo != NULL);

    return resp;
}




void defineTipoVar(char* tipo){
    tabelaSimbolos *aux = criaPilha();
    noSimbolo noAux;
    char tipoFull[20] = "variavel ";
    do{

        noAux = popPilha(&TS);
        strcat(tipoFull,tipo);
        insereTabela(aux,noAux.simbolo,tipoFull, noAux.escopo, 0);
    }while(strcmp(TS->simbolo.tipo,"variavel") == 0);

    do{
        noAux = popPilha(&aux);
        insereTabela(TS,noAux.simbolo,noAux.tipo, noAux.escopo, 0);
    }while(aux->prev != NULL);

}

void defineTipoFun(char* tipo){
    tabelaSimbolos *aux = (tabelaSimbolos*) malloc(sizeof(tabelaSimbolos));
    noSimbolo noAux;

    do{
        noAux = popPilha(&TS);
        strcat(noAux.tipo," ");
        strcat(noAux.tipo,tipo);
        insereTabela(aux,noAux.simbolo,noAux.tipo, noAux.escopo, 0);
    }while(strcmp(TS->simbolo.tipo,"funcao") == 0);
    do{
        noAux = popPilha(&aux);
        insereTabela(TS,noAux.simbolo,noAux.tipo, noAux.escopo, 0);
    }while(aux->simbolo.escopo != NULL);

}

char* confereTipo(char* simbolo){
    tabelaSimbolos *aux = (tabelaSimbolos*) malloc(sizeof(tabelaSimbolos));
    noSimbolo noAux;
    char *resp = NULL;

    do{
        noAux = popPilha(&TS);
        if(strcmp(noAux.simbolo, simbolo) == 0 && escopo_global <= noAux.escopo){
            insereTabela(TS,noAux.simbolo,noAux.tipo, noAux.escopo, 0);
            strcpy(resp, noAux.tipo);
            break;
        }
        insereTabela(aux,noAux.simbolo,noAux.tipo, noAux.escopo, 0);
    }while(TS->simbolo.escopo != NULL);
    do{
        noAux = popPilha(&aux);
        insereTabela(TS,noAux.simbolo,noAux.tipo, noAux.escopo, 0);
    }while(aux->simbolo.escopo != NULL);

    return resp;

}

bool pesquisa_duplicvar(char* simbolo){
    tabelaSimbolos *aux = criaPilha();
    noSimbolo noAux;
    bool resp = false;

    do{

        noAux = popPilha(&TS);
        if (TS == NULL)
            printf(" test yess ");



        if(strcmp(noAux.simbolo, simbolo) == 0 && (((strcmp(noAux.tipo, "variável inteiro" ) == 0 || strcmp(noAux.tipo, "variável booleano" ) == 0) &&  escopo_global == noAux.escopo)
                                                   ||(strcmp(noAux.tipo, "variável inteiro" ) != 0 && strcmp(noAux.tipo, "variável booleano" ) != 0 && escopo_global <= noAux.escopo ) )){
            insereTabela(TS,noAux.simbolo,noAux.tipo, noAux.escopo, 0);
            resp = true;
            break;
        }
        insereTabela(aux,noAux.simbolo,noAux.tipo, noAux.escopo, 0);

    }while(TS != NULL);

    do{
        noAux = popPilha(&aux);
        insereTabela(TS,noAux.simbolo,noAux.tipo, noAux.escopo, 0);
    }while(aux != NULL);
    return resp;
}

bool pesquisa_declvar(char* simbolo){
    tabelaSimbolos *aux = (tabelaSimbolos*) malloc(sizeof(tabelaSimbolos));
    noSimbolo noAux;
    bool resp = false;

    do{
        noAux = popPilha(&TS);
        if(strcmp(noAux.simbolo, simbolo) == 0 && (strcmp(noAux.tipo, "variável inteiro" ) == 0 || strcmp(noAux.tipo, "variável booleano" ) == 0) &&  escopo_global <= noAux.escopo){
            insereTabela(TS,noAux.simbolo,noAux.tipo, noAux.escopo, 0);
            resp = true;
            break;
        }
        insereTabela(aux,noAux.simbolo,noAux.tipo, noAux.escopo, 0);
    }while(TS->simbolo.escopo != NULL);
    do{
        noAux = popPilha(&aux);
        insereTabela(TS,noAux.simbolo,noAux.tipo, noAux.escopo, 0);
    }while(aux->simbolo.escopo != NULL);

    return resp;
}

bool pesquisa_procedimento(char *funcao){
    tabelaSimbolos *aux = (tabelaSimbolos*) malloc(sizeof(tabelaSimbolos));
    noSimbolo noAux;
    bool resp = false;

    do{
        noAux = popPilha(&TS);
        if(strcmp(noAux.simbolo, funcao) == 0 && strcmp(noAux.tipo, "procedimento") && escopo_global <= noAux.escopo){
            insereTabela(TS,noAux.simbolo,noAux.tipo, noAux.escopo, 0);
            resp = true;
            break;
        }
        insereTabela(aux,noAux.simbolo,noAux.tipo, noAux.escopo, 0);
    }while(TS->simbolo.escopo != NULL);
    do{
        noAux = popPilha(&aux);
        insereTabela(TS,noAux.simbolo,noAux.tipo, noAux.escopo, 0);
    }while(aux->simbolo.escopo != NULL);

    return resp;
}

bool pesquisa_funcao(char *funcao){
    tabelaSimbolos *aux = (tabelaSimbolos*) malloc(sizeof(tabelaSimbolos));
    noSimbolo noAux;
    bool resp = false;

    do{
        noAux = popPilha(&TS);
        if(strcmp(noAux.simbolo, funcao) == 0 && (strcmp(noAux.tipo, "funcao bool") || strcmp(noAux.tipo, "funcao int")) &&  escopo_global <= noAux.escopo){
            insereTabela(TS,noAux.simbolo,noAux.tipo, noAux.escopo, 0);
            resp = true;
            break;
        }
        insereTabela(aux,noAux.simbolo,noAux.tipo, noAux.escopo, 0);
    }while(TS->simbolo.escopo != NULL);
    do{
        noAux = popPilha(&aux);
        insereTabela(TS,noAux.simbolo,noAux.tipo, noAux.escopo, 0);
    }while(aux->simbolo.escopo != NULL);

    return resp;
}

