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

int main()
{
    tipoToken* list = NULL;                                                                         //teste da lista
    char c[] = "hello";                                                                             //o add n ta atualizando na mai ainda
    char d[] = "there";
    for (int I = 0; I < 10; I++)
    {
        addToList(&list, c,d);

    }
    while (list != NULL){
        printf("OWK: %s %s\n", list->lexema, list->Simbolo);
        list = list->next;
    };
    /*
    //abre arquivo
    //ler
    char c;

    while(c != 0)                                                                                   //logica q o professor passou
    do{                                                                                             //n garanto q funfa
            while((c == '{' || c == 32 ) && c != 0)
        do{ if (c == '{')
            {
                while(c != '}' && c != 0)
                do{
                    //ler
                }
                //ler
            }
            while(c == 32 && c != 0)
            do{
                    //ler
            }
        }
        if (c != 0)
        {
            //pega
            //insere
        }
    }
    //fecha
    */
    return 0;
}
/*
    void TrataIdentificador() //e palavra reservada
    {
        char* ID;
        char c;
        ID = c;
        //ler
        while(isLetra(c) == 1 || isDigito(c) == 1 || c == '_')
        {
           ID == ID + c;
           //ler
        }
    }

    int isLetra (char c)
    {
        if ((c >= 65 && c <= 90 ) || (c >=97 && c <= 122 ))
            return 1;
        else
            return 0;
    }

    int isDigito(char c)
    {
        if (c >= 48 && c <=57)
            return 1;
        else
            return 0;
    }
*/
