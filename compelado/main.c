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

int tratarDigito(int index);

int tratarOperador(int index);

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~* MAIN *~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*/

int main(){
    char line[LINE_LENGTH];

    //O endereço deve ser alterado para o adequado SEMPRE
    arquivo=fopen("C:/Users/nucle/Documents/GitHub/Compilador-em-C/compelado/teste_2a.txt","r");
    if(arquivo == NULL) {
        printf("ERRO");
        exit(1);
    }
    while(fgets(line,LINE_LENGTH,arquivo)){
        strcat(text, line);
        memset(line, 0, sizeof(line));
    }

    lerChar(0);

    printList(&list);



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

void printList(tipoToken** list)
{
    printf("simbolo\t\t\tlexema\n");
    while(*list != NULL)
    {
        printf ("%s\t\t\t%s \n", (*list)->Simbolo, (*list)->lexema);
        *list = (*list)->next;
    }

}

int tratarIdentificador(int index){
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
            addToList(&list, ID, s);
            return index;
        }
    }
    addToList(&list, ID, "sidentificador");
    return index;
}

int tratarDigito(int index){
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
    addToList(&list, ID, "snumero");
    return index;
}

int tratarOperador(int index){
    char ID[30] = {0};
    char c[2];
    c[1] = '\0';
    c[0] = text[index];
    index++;
    switch(c[0]){
        case '+': addToList(&list, c, "smais"); break;
        case '-': addToList(&list, c, "smenos"); break;
        case '*': addToList(&list, c, "smult"); break;
        case ';': addToList(&list, c, "sponto_virgula"); break;
        case ',': addToList(&list, c, "svirgula"); break;
        case '.': addToList(&list, c, "sponto"); break;
        case '(': addToList(&list, c, "sabre_parenteses"); break;
        case ')': addToList(&list, c, "sfecha_parenteses"); break;
        case '=': addToList(&list, c, "sig"); break;
        case '!':
            if (text[index] != '=')
                printf("Erro, caractere %s nao esperado, esperava \"=\"\n", text[index]);
            else
                {
                    index++;
                    addToList(&list, "!=", "sdif");
                }
            break;
        case ':':
            if (text[index] != '=')
                addToList(&list, c, "sdoispontos");
            else
                {
                    index++;
                    addToList(&list, ":=", "satribuicao");
                }
            break;
        case '<':
            if (text[index] != '=')
                addToList(&list, c, "smenor");
            else
                {
                    index++;
                    addToList(&list, "<=", "smenorig");
                }
            break;
        case '>':
            if (text[index] != '=')
                addToList(&list, c, "smaior");
            else
            {
                index++;
                addToList(&list, ">=", "smaiorig");
            }
            break;
        default: printf("Erro, Operador %s nao identificado \n", c); break;
    }
    return index;
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
            c = text[index];
        } else if ((c >= 65 && c <=90)|| (c >= 97 && c <= 122))
            index = tratarIdentificador(index);
        else if (c >=48 && c <= 57)
            index =tratarDigito(index);
        else if (c == 32 || c == 10 || c == 13)
            index++;
        else
            index = tratarOperador(index);
        c = text[index];
    }
}
