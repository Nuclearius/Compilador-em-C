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

void addToList(tipoToken** list,char* lex, char* sim);

int tratarIdentificador(int index);

void tratarDigito(tipoToken* list);

void tratarOperador(tipoToken* list);

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




    //ler (passar pro pr�ximo e colocar em c)


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

void addToList(tipoToken** list,char* lex, char* sim){
    if (*list == NULL)
    {
        *list = createToken(lex,sim);
    }
    else
    {
        addToList(&(*list)->next, lex, sim);
    }

}

int tratarIdentificador(int index){
    char ID[30];
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
    char* s = "s";
    char c;

    do{
        strcat( ID, text[index] );
        index++;
        c = text[index];

    }while ((c >= 65 && c <=90) || (c >= 97 && c <= 122) || (c >=48 && c <= 57) || c == '_');

    for (int I = 0; I < 21; I++)
    {
        if ( strcmp(ID,palavrasReservadas[I]) == 0)
        {
            strcat(s, palavrasReservadas[I]);
            addToList(&list, ID, s);
            return index;
        }
    }
    addToList(&list, ID, "sidentificador");
    return index;
}

void tratarDigito(tipoToken* list){
    char* ID;
    char c;
    while(c >=48 && c <= 57)
    {
        /*
        coloca em ID;
        ler;
        */
    }
    addToList(&list, ID, "snumero");
}

void tratarOperador(tipoToken* list){
    char c;
    switch(c){
        case ':': addToList(&list, c, "sdoispontos"); break;
        case '+': addToList(&list, c, "smais"); break;
        case '-': addToList(&list, c, "smenos"); break;
        case '*': addToList(&list, c, "smult"); break;
        case '!': addToList(&list, c, "s"); break;
        case '<': addToList(&list, c, "smenor"); break;
        case '>': addToList(&list, c, "smaior"); break;
        case '=': addToList(&list, c, "sig"); break;
        case ';': addToList(&list, c, "sponto_virgula"); break;
        case ',': addToList(&list, c, "svirgula"); break;
        case '(': addToList(&list, c, "sabre_parenteses"); break;
        case ')': addToList(&list, c, "sfecha_parenteses"); break;
        case '.': addToList(&list, c, "sponto"); break;
        default: printf("Erro, Operador %c não identificado", c); break;
    }
}

void lerChar(int index){
    char c = text[index];


    if ((c >= 65 && c <=90)|| (c >= 97 && c <= 122))
        index = tratarIdentificador(index);
    else if (c >=48 && c <= 57)
        tratarDigito(list);
    else if (c != 32)
        tratarOperador(list);

}
