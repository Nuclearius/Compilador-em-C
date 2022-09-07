#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct tipoToken{
    char* lexema;
    char* Simbolo;
    struct tipoToken *next;
}tipoToken;

tipoToken *createToken(char* lex, char* sim)
{
    tipoToken *newToken = malloc(sizeof(tipoToken));
    newToken->lexema = malloc(strlen(lex));
    newToken->lexema = lex;
    newToken->Simbolo = malloc(strlen(sim));
    newToken->Simbolo = sim;
    newToken->next = NULL;
    return newToken;
}

void addToList(tipoToken** list,char* lex, char* sim)
{
    if (*list == NULL)
    {
        *list = createToken(lex,sim);
    }
    else
    {
        addToList(&(*list)->next, lex, sim);
    }

}

void tratarIdentificador(tipoToken* list)
{
    char* ID;
    /*
    while (c for letra ou numero ou "_")
    {
        coloca em ID
        ler;
    }
        */
    if (strcmp(ID = "programa") == 0)
        addToList(&list, ID, "sprograma");              //palavras reservadas, mais else ifs aki
    else
        addToList(&list, ID, "sidentificador");
}

void tratarDigito(tipoToken* list)
{
    char* ID;
    /*
    while(c = for numero)
    {
        coloca em ID;
        ler;
    }
    */
    addToList(&list, ID, "snumero");
}

void tratarOperador(tipoToken* list, char c)
{
    switch(c)
    {
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
    case default: printf("erro"); break;
    }
}

int main()
{
    tipoToken* list = NULL;
    //abrir aqruivo
    //ler (passar pro próximo e colocar em c)
    char c;

    while (c != 0)
    {
        /*
        if (c for letra)
            tratarIdentificador(list);
        else if (c for número)
            tratarDigito(list);
        else if (c != 32)
            tratarOperador(list);
    }
    */
    return 0;
}



