#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~* STRUCT *~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*/

 FILE *arquivo;
typedef struct tipoToken{
    char* lexema;
    char* Simbolo;
    struct tipoToken *next;
}tipoToken;

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~* CABEÇALHOS *~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*/

tipoToken *createToken(char* lex, char* sim);

void addToList(tipoToken** list,char* lex, char* sim);

void tratarIdentificador(tipoToken* list);

void tratarDigito(tipoToken* list);

void tratarOperador(tipoToken* list, char c);

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~* MAIN *~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*/

int main(){
    tipoToken* list = NULL;
    if(!(arquivo=fopen("gera1.txt ","r"))) {
        printf("ERRO");
        exit(1);
    }
    //ler (passar pro pr�ximo e colocar em c)
    char c;

    while (c != 0)
        /*
     {
        if (c for letra)
            tratarIdentificador(list);
        else if (c for n�mero)
            tratarDigito(list);
        else if (c != 32)
            tratarOperador(list);
    }
    */
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

void tratarIdentificador(tipoToken* list){
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

void tratarDigito(tipoToken* list){
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

void tratarOperador(tipoToken* list, char c){

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
        case default: printf("erro"); break;
    }
}
