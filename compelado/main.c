#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 255

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~* VARIAVEIS *~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*/

typedef struct tipoToken{
    char* lexema;
    char* Simbolo;
    //struct tipoToken *next;
}tipoToken;

char text[2048];

FILE *arquivo;

tipoToken* token = NULL;

int index = 0;

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~* CABEÇALHOS *~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*/

tipoToken *createToken(char* lex, char* sim);

void addToList(char* lex, char* sim);

void tratarIdentificador();

void tratarDigito();

void tratarOperador();

void lexico();

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~* MAIN *~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*/

int main(){
    char line[LINE_LENGTH];
    //O endereço deve ser alterado para o adequado SEMPRE
    arquivo=fopen("C:/Users/nucle/Documents/GitHub/Compilador-em-C/compelado/sint1.txt","r");
    //arquivo=fopen("/home/luckytods/CLionProjects/Compilador-em-C/compelado/gera1.txt","r");
    //arquivo=fopen("C:/Users/19086818/Downloads/Compilador-em-C-test/compelado/sint10.txt","r");
    if(arquivo == NULL) {
        printf("ERRO");
        exit(1);
    }
    while(fgets(line,LINE_LENGTH,arquivo)){
        strcat(text, line);
        memset(line, 0, sizeof(line));
    }
    analise();
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
    cs [0] = text[index];
    do{
        strcat( ID, cs);
        index++;
        cs[0] = text[index];
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
    cs [0] = text[index];
    do{

        strcat( ID, cs);
        index++;
        cs[0] = text[index];
    }
    while(cs[0] >=48 && cs[0] <= 57);
    token = createToken( ID, "snumero");
}

void tratarOperador(){
    char ID[30] = {0};
    char c[2];
    c[1] = '\0';
    c[0] = text[index];
    index++;
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
            if (text[index] != '=')
                printf("\terro, caractere %s nao esperado, esperava \"=\"\n", text[index]);
            else
                {
                    index++;
                    token = createToken( "!=", "sdif");

                }
            break;
        case ':':
            if (text[index] != '=')
                token = createToken( c, "sdoispontos");
            else
                {
                    index++;
                    token = createToken( ":=", "satribuicao");
                }
            break;
        case '<':
            if (text[index] != '=')
                token = createToken( c, "smenor");
            else
                {
                    index++;
                    token = createToken( "<=", "smenorig");
                }
            break;
        case '>':
            if (text[index] != '=')
                token = createToken( c, "smaior");
            else
            {
                index++;
                token = createToken( ">=", "smaiorig");
            }
            break;
        default: printf("erro, Operador %s nao identificado \n", c); break;
    }
}

void lexico(){
    char c = text[index];
        if (c == 0)
            token = NULL;
        else if (c == '{')
        {
            while (c!= '}' && c != 0)
            {
                index++;
                c = text[index];
                if (c == 0)
                    printf("erro, comentario sem fim\n");
            }
            index++;
            lexico();
        } else if ((c >= 65 && c <=90)|| (c >= 97 && c <= 122))
            tratarIdentificador();
        else if (c >=48 && c <= 57)
            tratarDigito();
        else if (c == 32 || c == 10 || c == 13)
        {
            index++;
            lexico();
        }
        else
            tratarOperador();
        c = text[index];
}

void analise(){
    printf("\n[analise]\n");
    lexico();
    printf ("teste: %s %s\n", token->lexema, token->Simbolo);
    if (strcmp(token->Simbolo, "sprograma") == 0)
        {lexico(); printf("\n %s ", token->lexema);}
    else {printf("\terro em %s, \"programa\" esperado, %s recebido\n", token->lexema, token->Simbolo);return;}//\terro não tem programa
    if (strcmp(token->Simbolo, "sidentificador") == 0)
        {lexico(); printf("\n %s ", token->lexema);}
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
        if(strcmp(token->Simbolo,"sidentificador")== 0)
           {lexico(); printf("\n %s ", token->lexema);}
        else {printf("\terro em %s, identificador esperado\n", token->lexema);return;}
        if(strcmp(token->Simbolo,"svirgula")== 0 || strcmp(token->Simbolo,"sdoispontos")== 0)
        {
            if (strcmp(token->Simbolo,"svirgula")== 0)
            {
                lexico();
                printf("\n %s ", token->lexema);
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
    if (strcmp(token->Simbolo,"sinteiro")!=0 && strcmp(token->Simbolo,"sbooleano")!=0)
        {printf("\terro em %s, tipo nao reconhecido\n", token->lexema);return;}//\terro
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

void analisa_atrib_chprocedimento(){
    printf("\n[analisa_atrib_chprocedimento]\n");
    lexico();
    printf("\n %s ", token->lexema);
    if (strcmp(token->Simbolo, "satribuicao") == 0)
    {
        analisa_atribuicao();
    }
    else analisa_chamada_procedimento();
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

void analisa_leia(){
    printf("\n[analisa_leia]\n");
    lexico();
    printf("\n %s ", token->lexema);
    if (strcmp(token->Simbolo, "sabre_parenteses") == 0)
        {lexico(); printf("\n %s ", token->lexema);}
    else {printf("\terro em %s, \"(\" esperado\n", token->lexema);return;}//\terro
    if (strcmp(token->Simbolo, "sidentificador") == 0)
        {lexico(); printf("\n %s ", token->lexema);}
    else {printf("\terro em %s, identificador esperado\n", token->lexema);return;}//\terro
    //random semantic shit
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
    if (strcmp(token->Simbolo, "sidentificador") == 0)
        {lexico(); printf("\n %s ", token->lexema);}
    else {printf("\terro em %s, identificador esperado\n", token->lexema);return;}//\terro
    //random semantic shit
    if (strcmp(token->Simbolo, "sfecha_parenteses") == 0)
        {lexico(); printf("\n %s ", token->lexema);}
    else {printf("\terro em %s, \")\" esperado\n", token->lexema);return;}//\terro
    printf("\n[analisa_escreva end]\n");
}

void analisa_enquanto(){
    printf("\n[analisa_enquanto]\n");
    lexico();
    printf("\n %s ", token->lexema);
    analisa_expressao();
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
    analisa_expressao();
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

void analisa_expressao(){
    printf("\n[analisa_expressao]\n");
    analisa_expressao_simples();
    if (strcmp(token->Simbolo, "smaior") == 0 || strcmp(token->Simbolo, "smaiorig") == 0 || strcmp(token->Simbolo, "sig") == 0 || strcmp(token->Simbolo, "smenor") == 0 || strcmp(token->Simbolo, "smenorig") == 0 || strcmp(token->Simbolo, "sdif") == 0 )
    {
         lexico();
         printf("\n %s ", token->lexema);
         analisa_expressao_simples();
    }
    printf("\n[analisa_expressao end]\n");
}

void analisa_expressao_simples(){
    printf("\n[analisa_expressao_simples]\n");
    if(strcmp(token->Simbolo, "smais") == 0 || strcmp(token->Simbolo, "smenos") == 0)
        {lexico(); printf("\n %s ", token->lexema);}
    analisa_termo();
    while(strcmp(token->Simbolo, "smais") == 0 || strcmp(token->Simbolo, "smenos") == 0 || strcmp(token->Simbolo, "sou") == 0)
    {
        lexico();
        printf("\n %s ", token->lexema);
        analisa_termo();
    }
    printf("\n[analisa_expressao_simples end]\n");
}

void analisa_termo(){
    printf("\n[analisa_termo]\n");
    analisa_fator();
    while(strcmp(token->Simbolo, "smult") == 0 || strcmp(token->Simbolo, "sdiv") == 0 || strcmp(token->Simbolo, "se") == 0)
    {
        lexico();
        printf("\n %s ", token->lexema);
        analisa_fator();
    }
    printf("\n[analisa_termo end]\n");
}

void analisa_fator(){
    printf("\n[analisa_fator]\n");
    if (strcmp(token->Simbolo, "sidentificador") == 0)
    {
        analisa_chamada_funcao();
    } else if (strcmp(token->Simbolo, "snumero") == 0)
    {
        lexico();
        printf("\n %s ", token->lexema);
    } else if (strcmp(token->Simbolo, "snao") == 0)
    {
        lexico();
        printf("\n %s ", token->lexema);
        analisa_fator();
    } else if (strcmp(token->Simbolo, "sabre_parenteses") == 0)
    {
        lexico();
        printf("\n %s ", token->lexema);
        analisa_expressao();
        if (strcmp(token->Simbolo, "sfecha_parenteses") == 0)
            {lexico(); printf("\n %s ", token->lexema);}
        else {printf("\terro em %s, \")\" esperado\n", token->lexema);return;} //\terro
    } else if (strcmp(token->Simbolo, "sverdadeiro") == 0 || strcmp(token->Simbolo, "sfalso") == 0)
        {lexico(); printf("\n %s ", token->lexema);}
    else {printf("\terro em %s, verdadeiro ou falso esperado\n", token->lexema);return;}//\terro
    printf("\n[analisa_fator end]\n");
}

void analisa_chamada_procedimento(){
    printf("\n[analisa_chamada_procedimento]\n");
    //semantic shiet
    printf("\n[analisa_chamada_procedimento end]\n");
}
void analisa_chamada_funcao(){
    printf("\n[analisa_chamada_funcao]\n");
    //semantic shiet
    lexico();
    printf("\n %s ", token->lexema);
    printf("\n[analisa_chamada_funcao end]\n");
}

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
    printf("\n %s ", token->lexema);
    if(strcmp(token->Simbolo,"sidentificador")==0){
        lexico();
        printf("\n %s ", token->lexema);
        if(strcmp(token->Simbolo,"sponto_virgula")==0)
            analisa_bloco();
        else {printf("\terro em %s, \";\" esperado\n", token->lexema);return;}//\terro ; esperado
    }
    else {printf("\terro em %s, identificador esperado\n", token->lexema);return;}//\terro
    printf("\n[analisa_declaracao_procedimento end]\n");
}
void analisa_declaracao_funcao(){
    printf("\n[analisa_declaracao_funcao]\n");
    lexico();
    printf("\n %s ", token->lexema);
    if(strcmp(token->Simbolo,"sidentificador")==0){
        lexico();
        printf("\n %s ", token->lexema);
        if(strcmp(token->Simbolo,"sdoispontos")==0){
            lexico();
            printf("\n %s ", token->lexema);
            if(strcmp(token->Simbolo,"sinteiro")==0 || strcmp(token->Simbolo,"sbooleano")==0){
                lexico();
                printf("\n %s ", token->lexema);
                if(strcmp(token->Simbolo,"sponto_virgula")==0)
                    analisa_bloco();
            }else {printf("\terro em %s, tipo nao reconhecido\n", token->lexema);return;}//\terro
        }else {printf("\terro em %s, \":\" esperado\n", token->lexema);return;}//\terro
    }else {printf("\terro em %s, identificador esperado\n", token->lexema);return;}//\terro
    printf("\n[analisa_declaracao_funcao end]\n");
}

/*
void addPilha(){

}

void rmPilha(){

}

bool scanPilha(char* simbolo){

}

*/
