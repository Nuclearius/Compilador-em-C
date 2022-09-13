#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 255

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~* VARIAVEIS *~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*/

typedef struct tipoToken{
    char* lexema;
    char* Simbolo;
    struct tipoToken *next;
}tipoToken;

char text[2048];

FILE *arquivo;

tipoToken* list = NULL;

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~* CABEÇALHOS *~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*/

tipoToken *createToken(char* lex, char* sim);

void addToList(char* lex, char* sim);

int tratarIdentificador(int index);

int tratarDigito(int index);

void tratarOperador(int index);

void lerChar(int index);

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~* MAIN *~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*/

int main(){
    char line[LINE_LENGTH];

    //O endereço deve ser alterado para o adequado SEMPRE
    arquivo=fopen("/home/luckytods/CLionProjects/Compilador-em-C/compelado/gera1.txt","r");
    if(arquivo == NULL) {
        printf("ERRO");
        exit(1);
    }
    while(fgets(line,LINE_LENGTH,arquivo)){
        strcat(text, line);
        memset(line, 0, sizeof(line));
    }

    lerChar(0);

    while(list != NULL)
    {
        printf ("simbolo: %s \t\t\t lexema: %s \n", list->Simbolo, list->lexema);
        list = list->next;
    }

         fclose(arquivo);
        return 0;
}

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~* FUNÇÕES *~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*/

tipoToken *createToken(char* lex, char* sim){
    tipoToken *newToken = malloc(sizeof(tipoToken));
    newToken->lexema = malloc(strlen(lex));
    newToken->lexema = lex;
    newToken->Simbolo = malloc(strlen(sim));
    newToken->Simbolo = sim;
    newToken->next = NULL;
    return newToken;
}

void addToList(char* lex, char* sim){

    tipoToken *auxlist = list;

    while (list != NULL)
        list = list->next;
    list = createToken(lex,sim);
    list = auxlist;
}

int tratarIdentificador(int index){
    char ID[30] = {0};
    char palavrasReservadas[][20] = { "programa", //
                                    "se",
                                    "entao",
                                    "senao",
                                    "enquanto",
                                    "faca",
                                    "inicio",
                                    "fim",
                                    "escreva",
                                    "leia",
                                    "var",      //
                                    "inteiro",  //
                                    "booleano",
                                    "verdadeiro",
                                    "falso",
                                    "procedimento",
                                    "funcao",   //
                                    "div",
                                    "e",
                                    "ou",
                                    "nao"};
    char s[30] = {0};
    s[0] = 's';
    char cs[2];
    cs[1] = '\0';
    cs [0] = text[index];
    do{
        strcat( ID, cs);
        index++;
        cs[0] = text[index];
    }while ((cs[0] >= 65 && cs[0] <=90) || (cs[0]>= 97 && cs[0] <= 122) || (cs[0] >=48 && cs[0] <= 57) || cs[0] == '_');
    char IDfim[30];
    strcpy(IDfim, ID);
    for (int I = 0; I < 21; I++)
    {

        if ( strcmp(ID,palavrasReservadas[I]) == 0)
        {
            strcat(s, ID);
            addToList(ID, s);
            return index;
        }
    }
    addToList(ID, "sidentificador");
    return index;
}

int tratarDigito(int index){
    char ID[30];
   char cs[2];
    cs[1] = '\0';
    cs [0] = text[index];
    do{

        strcat( ID, cs);
        index++;
        cs[0] = text[index];
    }
    while(cs[0] >=48 && cs[0] <= 57);
    addToList(ID, "snumero");
    return index;
}

void tratarOperador(int index){
    char c[2];
    c[1] = '\0';
    c[0] = text[index];
    switch(c[0]){
        case ':': addToList(c, "sdoispontos"); break;
        case '+': addToList(c, "smais"); break;
        case '-': addToList(c, "smenos"); break;
        case '*': addToList(c, "smult"); break;
        case '!': addToList(c, "s"); break;
        case '<': addToList(c, "smenor"); break;
        case '>': addToList(c, "smaior"); break;
        case '=': addToList(c, "sig"); break;
        case ';': addToList(c, "sponto_virgula"); break;
        case ',': addToList(c, "svirgula"); break;
        case '(': addToList(c, "sabre_parenteses"); break;
        case ')': addToList(c, "sfecha_parenteses"); break;
        case '.': addToList(c, "sponto"); break;
        default: printf("Erro, Operador %c não identificado \n", c[0]); break;
    }
}

void lerChar(int index){
    char c = text[index];

    while (c != 0)
    {
        if (c == '{')
        {
            while (c!= '}' && c != 0)
            {
                index++;
                c = text[index];
            }
            index++;
        } else if ((c >= 65 && c <=90)|| (c >= 97 && c <= 122))
            index = tratarIdentificador(index);
        else if (c >=48 && c <= 57)
            index =tratarDigito(index);
        else if (c == 32 || c == 10 || c == 13)
            index++;
        else
        {
            tratarOperador(index);
            index++;
        }
        c = text[index];
    }
}
