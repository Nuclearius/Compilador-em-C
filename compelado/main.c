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
    arquivo=fopen("C:/Users/nucle/Documents/GitHub/Compilador-em-C/compelado/gera1.txt","r");
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


void sintatico()
{
    if (strcmp(list->Simbolo, "sprograma") == 0)
        list = list->next;
    else return;//erro não tem programa
    if (strcmp(list->Simbolo, "sidentificador") == 0)
        list = list->next;
    else return;//erro falta identificador
    if (strcmp(list->Simbolo, "sponto_virgula") == 0)
        analisa_bloco();
    else return;//erro ponto_virg
    if (strcmp(list->Simbolo, "sponto") == 0)
    {
        if (list->next == NULL)
            return; // sucesso
        else return;// erro
    } else return; //erro

}

void analisa_bloco()
{
    list=list->next;
    et_analisa_var();
    analisa_subrotinas();
    analisa_comandos();
}


void et_analisa_var()
{
    if(strcmp(list->Simbolo,"svar")== 0)
        list=list->next;
    else return; //
            if(strcmp(list->Simbolo,"sidentificador")== 0)
            {
                while(strcmp(list->Simbolo,"sidentificador")== 0)
                {
                    analisa_var();
                    if (strcmp(list->Simbolo,"sponto_virgula")== 0)
                        list=list->next;
                    else return; //erro ; esperado
                }
            }
            else return; // erro falta identificador
}

void analisa_var()
{
    do{
        if(strcmp(list->Simbolo,"sidentificador")== 0)
           list=list->next;
            else return;
        if(strcmp(list->Simbolo,"svirgula")== 0 || strcmp(list->Simbolo,"sdoispontos")== 0)
        {
            if (strcmp(list->Simbolo,"svirgula")== 0)
            {
                list=list->next;
                if (strcmp(list->Simbolo,"sdoispontos")== 0)
                    return;//erro, identificador espserado
            }
        } else return; //erro , ou : esperado
    }while  (strcmp(list->Simbolo,"sdoispontos") != 0);
    list=list->next;
    analisa_tipo();
}

void analisa_tipo()
{
    if (strcmp(list->Simbolo,"sinteiro")!=0 && strcmp(list->Simbolo,"sbooleano")!=0)
        return;//erro
    list=list->next;

}

void analisa_comandos()
{
    if (strcmp(list->Simbolo, "sinicio") == 0)
        list = list->next;
    else return; //erro, inicio esperado
    analisa_comando_simples();
    while(strcmp(list->Simbolo,"sfim") != 0)
    {
        if (strcmp(list->Simbolo, "sponto_virgula") == 0)
        {
            list = list->next;
            if (strcmp(list->Simbolo,"sfim") != 0)
                analisa_comando_simples();
        } else return; //erro ; esperado
    }
    list= list->next;
}

void analisa_comando_simples()
{
    if (strcmp(list->Simbolo, "sidentificador") == 0)
        analisa_atrib_chprocedimento();
    else if (strcmp(list->Simbolo, "sse") == 0)
        analisa_se();
    else if (strcmp(list->Simbolo, "senquanto") == 0)
        analisa_enquanto();
    else if (strcmp(list->Simbolo, "sleia") == 0)
        analisa_leia();
    else if (strcmp(list->Simbolo, "sesceva") == 0)
        analisa_escreva();
    else analisa_comandos();

}

void analisa_atrib_chprocedimento()
{
    list=list->next;
    if (strcmp(list->Simbolo, "satribuicao") == 0)
        analisa_atribuicao();
    else analisa_chamada_procedimento();

}

void analisa_atribuicao()
{

}

void analisa_leia()
{
    list = list->next;
    if (strcmp(list->Simbolo, "sabre_parenteses") == 0)
        list=list->next;
    else return;//erro
    if (strcmp(list->Simbolo, "sidentficador") == 0)
        list=list->next;
    else return;//erro
    //random sintatic shit
    if (strcmp(list->Simbolo, "sfecha_parenteses") == 0)
        list=list->next;
    else return;//erro
}

void analisa_escreva()
{
    list = list->next;
    if (strcmp(list->Simbolo, "sabre_parenteses") == 0)
        list=list->next;
    else return;//erro
    if (strcmp(list->Simbolo, "sidentficador") == 0)
        list=list->next;
    else return;//erro
    //random sintatic shit
    if (strcmp(list->Simbolo, "sfecha_parenteses") == 0)
        list=list->next;
    else return;//erro
}

void analisa_enquanto()
{
    list = list->next;
    analisa_expressao();
    if (strcmp(list->Simbolo, "sfaca") == 0)
        list=list->next;
    else return; //erro
    analisa_comando_simples();
}

void analisa_se()
{
    list = list->next;
    analisa_expressao();
    if (strcmp(list->Simbolo, "sentao") == 0)
        list=list->next;
    else return; //erro
    analisa_comando_simples();
    if (strcmp(list->Simbolo, "ssenao") == 0)
    {
        list=list->next;
        analisa_comando_simples();
    }
}

void analisa_expressao()
{
    analisa_expressao_simples();
    if (strcmp(list->Simbolo, "smaior") == 0 || strcmp(list->Simbolo, "smaiorig") == 0 || strcmp(list->Simbolo, "sig") == 0 || strcmp(list->Simbolo, "smenor") == 0 || strcmp(list->Simbolo, "smenorig") == 0 || strcmp(list->Simbolo, "sdif") == 0 )
    {
         list=list->next;
         analisa_expressao_simples();
    }
}

void analisa_expressao_simples()
{
    if(strcmp(list->Simbolo, "smais") == 0 || strcmp(list->Simbolo, "smenos") == 0)
        list = list->next;
    analisa_termo();
    while(strcmp(list->Simbolo, "smais") == 0 || strcmp(list->Simbolo, "smenos") == 0 || strcmp(list->Simbolo, "sou") == 0)
    {
        list=list->next;
        analisa_termo();
    }
}

void analisa_termo()
{
    analisa_fator();
    while(strcmp(list->Simbolo, "smult") == 0 || strcmp(list->Simbolo, "sdiv") == 0 || strcmp(list->Simbolo, "se") == 0)
    {
        list = list->next;
        analisa_fator();
    }
}

void analisa_fator()
{
    if (strcmp(list->Simbolo, "sidentificador") == 0)
    {
        analisa_chamada_funcao();
    } else if (strcmp(list->Simbolo, "snumero") == 0)
    {
        list = list->next;
    } else if (strcmp(list->Simbolo, "snao") == 0)
    {
        list = list->next;
        analisa_fator();
    } else if (strcmp(list->Simbolo, "sabre_parenteses") == 0)
    {
        list = list->next;
        analisa_expressao();
        if (strcmp(list->Simbolo, "sfecha_parenteses") == 0)
            list = list->next;
        else return; //erro
    } else if (strcmp(list->Simbolo, "sverdadeiro") == 0 || strcmp(list->Simbolo, "sfalso") == 0)
        list = list->next;
    else return;//erro
}

void analisa_chamada_procedimento()
{

}
void analisa_chamada_funcao()
{

}

void analisa_subrotinas()
{
    while(strcmp(list->Simbolo,"sprocedimento")==0|| strcmp(list->Simbolo,"sfuncao")==0)
    {
        if((strcmp(list->Simbolo,"sprocedimento")==0))
            analisa_declaracao_procedimento();
        else analisa_declaracao_funcao();
        if(strcmp(list->Simbolo,"spontovirgula")==0)
            list=list->next;
        else return;//erro, ; esperado
    }
}

void analisa_declaracao_procedimento()
{
    list=list->next;
    if(strcmp(list->Simbolo,"sidentificador")==0){
        list=list->next;
        if(strcmp(list->Simbolo,"sponto_virgula")==0)
            analisa_bloco();
        else return;//erro ; esperado
    }
    else return;//erro

}
void analisa_declaracao_funcao(){
    list=list->next;
    if(strcmp(list->Simbolo,"sidentificador")==0){
        list=list->next;
        if(strcmp(list->Simbolo,"sdoispontos")==0){
            list=list->next;
            if(strcmp(list->Simbolo,"sinteiro")==0 || strcmp(list->Simbolo,"sbooleano")==0){
                list=list->next;
                if(strcmp(list->Simbolo,"sponto_virgula")==0)
                    analisa_bloco();
            }else return;//erro
        }else return;//erro
    }else return;//erro
}

