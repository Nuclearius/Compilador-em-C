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

int tratarOperador(int index);

void lerChar(int index);

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~* MAIN *~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*/

int main(){
    char line[LINE_LENGTH];
    //O endereço deve ser alterado para o adequado SEMPRE
    arquivo=fopen("C:/Users/nucle/Documents/GitHub/Compilador-em-C/compelado/sint1.txt","r");
    //arquivo=fopen("/home/luckytods/CLionProjects/Compilador-em-C/compelado/gera1.txt","r");

    if(arquivo == NULL) {
        printf("ERRO");
        exit(1);
    }
    while(fgets(line,LINE_LENGTH,arquivo)){
        strcat(text, line);
        memset(line, 0, sizeof(line));
    }
    lerChar(0);
    tipoToken* head = list;
    //printList(&list);
    sintatico();
    printf("\nanalise sintatica concluida\n");

    tipoToken* tmp;
    while (head != NULL)
    {
        tmp = head;
        head = head->next;
        free(tmp->lexema);
        free(tmp->Simbolo);
        free(tmp);
    }

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

void addToList(char* lex, char* sim){

    tipoToken *auxlist = list;

    while (list != NULL)
        list = list->next;
    list = createToken(lex,sim);
    list = auxlist;
}

void printList(tipoToken** list){
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
            addToList(ID, s);
            return index;
        }
    }
    addToList(ID, "sidentificador");
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
    addToList(ID, "snumero");
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
                printf("\terro, caractere %s nao esperado, esperava \"=\"\n", text[index]);
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
        default: printf("erro, Operador %s nao identificado \n", c); break;
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


void sintatico(){
    printf("\n[sintatico]\n");
    if (strcmp(list->Simbolo, "sprograma") == 0)
        {list = list->next; printf("\n %s ", list->lexema);}
    else {printf("\terro em %s, \"programa\" esperado\n", list->lexema);return;}//\terro não tem programa
    if (strcmp(list->Simbolo, "sidentificador") == 0)
        {list = list->next; printf("\n %s ", list->lexema);}
    else {printf("\terro em %s, identificador esperado\n", list->lexema);return;}//\terro falta identificador
    if (strcmp(list->Simbolo, "sponto_virgula") == 0)
        analisa_bloco();
    else  {printf("\terro em %s, \";\" esperado\n", list->lexema);return;}//\terro ponto_virg
    if (strcmp(list->Simbolo, "sponto") == 0)
    {
        if (list->next == NULL)
            return; // sucesso
        else {printf("\terro em %s, final de codigo esperado\n", list->lexema);return;}// \terro
    } else {printf("\terro em %s, \".\" esperado\n", list->lexema);return;} //\terro
}

void analisa_bloco(){
    printf("\n[analisa_bloco]\n");
    list = list->next;
    printf("\n %s ", list->lexema);
    et_analisa_var();
    analisa_subrotinas();
    analisa_comandos();
    printf("\n[analisa_bloco end]\n");
}


void et_analisa_var(){
    printf("\n[et_analisa_var]\n");
    if(strcmp(list->Simbolo,"svar")== 0)
    {list = list->next; printf("\n %s ", list->lexema);//\terro
        if(strcmp(list->Simbolo,"sidentificador")== 0)
        {
            while(strcmp(list->Simbolo,"sidentificador")== 0)
            {
                analisa_var();
                if (strcmp(list->Simbolo,"sponto_virgula")== 0)
                    {list = list->next; printf("\n %s ", list->lexema);}
                else  {printf("\terro em %s, \";\" esperado\n", list->lexema);return;} //\terro ; esperado
            }
        }else {printf("\terro em %s, identificador esperado\n", list->lexema);return;}// \terro falta identificador
        printf("\n[et_analisa_var end]\n");
    }
}

void analisa_var(){
    printf("\n[analisa_var]\n");
    do{
        if(strcmp(list->Simbolo,"sidentificador")== 0)
           {list = list->next; printf("\n %s ", list->lexema);}
        else {printf("\terro em %s, identificador esperado\n", list->lexema);return;}
        if(strcmp(list->Simbolo,"svirgula")== 0 || strcmp(list->Simbolo,"sdoispontos")== 0)
        {
            if (strcmp(list->Simbolo,"svirgula")== 0)
            {
                list = list->next;
                printf("\n %s ", list->lexema);
                printf("\n %s ", list->lexema);
                if (strcmp(list->Simbolo,"sdoispontos")== 0)
                    {printf("\terro em %s, identificador esperado\n", list->lexema);return;}//\terro, identificador espserado
            }
        } else {printf("\terro em %s, \",\" ou \":\" esperado\n", list->lexema);return;} //\terro , ou : esperado
    }while  (strcmp(list->Simbolo,"sdoispontos") != 0);
    list = list->next;
    printf("\n %s ", list->lexema);
    analisa_tipo();
    printf("\n[analisa_var end]\n");
}

void analisa_tipo(){
    printf("\n[analisa_tipo]\n");
    if (strcmp(list->Simbolo,"sinteiro")!=0 && strcmp(list->Simbolo,"sbooleano")!=0)
        {printf("\terro em %s, tipo nao reconhecido\n", list->lexema);return;}//\terro
    list = list->next;
    printf("\n %s ", list->lexema);
    printf("\n[analisa_tipo end]\n");
}

void analisa_comandos(){
    printf("\n[analisa_comandos]\n");
    if (strcmp(list->Simbolo, "sinicio") != 0)
     {printf("\terro em %s, inicio esperado\n", list->lexema);return;} //\terro, inicio esperado
    list = list->next;
    printf("\n %s ", list->lexema);
    analisa_comando_simples();
    while(strcmp(list->Simbolo,"sfim") != 0)
    {
        if (strcmp(list->Simbolo, "sponto_virgula") == 0)
        {
            list = list->next;
            printf("\n %s ", list->lexema);
            if (strcmp(list->Simbolo,"sfim") != 0)
                analisa_comando_simples();
        } else {printf("\terro em %s, \";\" esperado\n", list->lexema);return;} //\terro ; esperado
    }
    list = list->next;
    printf("\n %s ", list->lexema);
    printf("\n[analisa_comandos end]\n");
}

void analisa_comando_simples(){
    printf("\n[analisa_comando_simples]\n");
    if (strcmp(list->Simbolo, "sidentificador") == 0)
        analisa_atrib_chprocedimento();
    else if (strcmp(list->Simbolo, "sse") == 0)
        analisa_se();
    else if (strcmp(list->Simbolo, "senquanto") == 0)
        analisa_enquanto();
    else if (strcmp(list->Simbolo, "sleia") == 0)
        analisa_leia();
    else if (strcmp(list->Simbolo, "sescreva") == 0)
        analisa_escreva();
    else analisa_comandos();
    printf("\n[analisa_comando_simples end]\n");
}

void analisa_atrib_chprocedimento(){
    printf("\n[analisa_atrib_chprocedimento]\n");
    /*
    list = list->next;
    printf("\n %s ", list->lexema);
    */
    if (strcmp(list->next->Simbolo, "satribuicao") == 0)
    {
        list = list->next;
        printf("\n %s ", list->lexema);
        analisa_atribuicao();
    }
    else analisa_chamada_procedimento();
    printf("\n[analisa_atrib_chprocedimento end]\n");
}

void analisa_atribuicao(){
    printf("\n[analisa_atribuicao]\n");
    list = list->next;
    printf("\n %s ", list->lexema);
    //semantic shit
    analisa_expressao();
    printf("\n[analisa_atribuicao end]\n");
}

void analisa_leia(){
    printf("\n[analisa_leia]\n");
    list = list->next;
    printf("\n %s ", list->lexema);
    if (strcmp(list->Simbolo, "sabre_parenteses") == 0)
        {list = list->next; printf("\n %s ", list->lexema);}
    else {printf("\terro em %s, \"(\" esperado\n", list->lexema);return;}//\terro
    if (strcmp(list->Simbolo, "sidentificador") == 0)
        {list = list->next; printf("\n %s ", list->lexema);}
    else {printf("\terro em %s, identificador esperado\n", list->lexema);return;}//\terro
    //random semantic shit
    if (strcmp(list->Simbolo, "sfecha_parenteses") == 0)
        {list = list->next; printf("\n %s ", list->lexema);}
    else {printf("\terro em %s, \")\" esperado\n", list->lexema);return;}//\terro
    printf("\n[analisa_leia end]\n");
}

void analisa_escreva(){
    printf("\n[analisa_escreva]\n");
    list = list->next;
    printf("\n %s ", list->lexema);
    if (strcmp(list->Simbolo, "sabre_parenteses") == 0)
        {list = list->next; printf("\n %s ", list->lexema);}
    else {printf("\terro em %s, \"(\" esperado\n", list->lexema);return;}//\terro
    if (strcmp(list->Simbolo, "sidentificador") == 0)
        {list = list->next; printf("\n %s ", list->lexema);}
    else {printf("\terro em %s, identificador esperado\n", list->lexema);return;}//\terro
    //random semantic shit
    if (strcmp(list->Simbolo, "sfecha_parenteses") == 0)
        {list = list->next; printf("\n %s ", list->lexema);}
    else {printf("\terro em %s, \")\" esperado\n", list->lexema);return;}//\terro
    printf("\n[analisa_escreva end]\n");
}

void analisa_enquanto(){
    printf("\n[analisa_enquanto]\n");
    list = list->next;
    printf("\n %s ", list->lexema);
    analisa_expressao();
    if (strcmp(list->Simbolo, "sfaca") == 0)
        {list = list->next; printf("\n %s ", list->lexema);}
    else {printf("\terro em %s, faca esperado\n", list->lexema);return;} //\terro
    analisa_comando_simples();
    printf("\n[analisa_enquanto end]\n");
}

void analisa_se(){
    printf("\n[analisa_se]\n");
    list = list->next;
    printf("\n %s ", list->lexema);
    analisa_expressao();
    if (strcmp(list->Simbolo, "sentao") == 0)
        {list = list->next; printf("\n %s ", list->lexema);}
    else {printf("\terro em %s, entao esperado\n", list->lexema);return;} //\terro
    analisa_comando_simples();
    if (strcmp(list->Simbolo, "ssenao") == 0)
    {
        list = list->next;
        printf("\n %s ", list->lexema);
        analisa_comando_simples();
    }
    printf("\n[analisa_se end]\n");
}

void analisa_expressao(){
    printf("\n[analisa_expressao]\n");
    analisa_expressao_simples();
    if (strcmp(list->Simbolo, "smaior") == 0 || strcmp(list->Simbolo, "smaiorig") == 0 || strcmp(list->Simbolo, "sig") == 0 || strcmp(list->Simbolo, "smenor") == 0 || strcmp(list->Simbolo, "smenorig") == 0 || strcmp(list->Simbolo, "sdif") == 0 )
    {
         list = list->next;
         printf("\n %s ", list->lexema);
         analisa_expressao_simples();
    }
    printf("\n[analisa_expressao end]\n");
}

void analisa_expressao_simples(){
    printf("\n[analisa_expressao_simples]\n");
    if(strcmp(list->Simbolo, "smais") == 0 || strcmp(list->Simbolo, "smenos") == 0)
        {list = list->next; printf("\n %s ", list->lexema);}
    analisa_termo();
    while(strcmp(list->Simbolo, "smais") == 0 || strcmp(list->Simbolo, "smenos") == 0 || strcmp(list->Simbolo, "sou") == 0)
    {
        list = list->next;
        printf("\n %s ", list->lexema);
        analisa_termo();
    }
    printf("\n[analisa_expressao_simples end]\n");
}

void analisa_termo(){
    printf("\n[analisa_termo]\n");
    analisa_fator();
    while(strcmp(list->Simbolo, "smult") == 0 || strcmp(list->Simbolo, "sdiv") == 0 || strcmp(list->Simbolo, "se") == 0)
    {
        list = list->next;
        printf("\n %s ", list->lexema);
        analisa_fator();
    }
    printf("\n[analisa_termo end]\n");
}

void analisa_fator(){
    printf("\n[analisa_fator]\n");
    if (strcmp(list->Simbolo, "sidentificador") == 0)
    {
        analisa_chamada_funcao();
    } else if (strcmp(list->Simbolo, "snumero") == 0)
    {
        list = list->next;
        printf("\n %s ", list->lexema);
    } else if (strcmp(list->Simbolo, "snao") == 0)
    {
        list = list->next;
        printf("\n %s ", list->lexema);
        analisa_fator();
    } else if (strcmp(list->Simbolo, "sabre_parenteses") == 0)
    {
        list = list->next;
        printf("\n %s ", list->lexema);
        analisa_expressao();
        if (strcmp(list->Simbolo, "sfecha_parenteses") == 0)
            {list = list->next; printf("\n %s ", list->lexema);}
        else {printf("\terro em %s, \")\" esperado\n", list->lexema);return;} //\terro
    } else if (strcmp(list->Simbolo, "sverdadeiro") == 0 || strcmp(list->Simbolo, "sfalso") == 0)
        {list = list->next; printf("\n %s ", list->lexema);}
    else {printf("\terro em %s, verdadeiro ou falso esperado\n", list->lexema);return;}//\terro
    printf("\n[analisa_fator end]\n");
}

void analisa_chamada_procedimento(){
    printf("\n[analisa_chamada_procedimento]\n");
    //semantic shiet
    list = list->next;
    printf("\n %s ", list->lexema);
    printf("\n[analisa_chamada_procedimento end]\n");
}
void analisa_chamada_funcao(){
    printf("\n[analisa_chamada_funcao]\n");
    //semantic shiet
    list = list->next;
    printf("\n %s ", list->lexema);
    printf("\n[analisa_chamada_funcao end]\n");
}

void analisa_subrotinas()
{
    printf("\n[analisa_subrotinas]\n");
    while(strcmp(list->Simbolo,"sprocedimento")==0|| strcmp(list->Simbolo,"sfuncao")==0)
    {
        if(strcmp(list->Simbolo,"sprocedimento")==0)
            analisa_declaracao_procedimento();
        else analisa_declaracao_funcao();
        if(strcmp(list->Simbolo,"sponto_virgula")==0)
            {list = list->next; printf("\n %s ", list->lexema);}
        else {printf("\terro em %s, \";\" esperado\n", list->lexema);return;}//\terro, ; esperado
    }
    printf("\n[analisa_subrotinas end]\n");
}

void analisa_declaracao_procedimento(){
    printf("\n[analisa_declaracao_procedimento]\n");
    list = list->next;
    printf("\n %s ", list->lexema);
    if(strcmp(list->Simbolo,"sidentificador")==0){
        list = list->next;
        printf("\n %s ", list->lexema);
        if(strcmp(list->Simbolo,"sponto_virgula")==0)
            analisa_bloco();
        else {printf("\terro em %s, \";\" esperado\n", list->lexema);return;}//\terro ; esperado
    }
    else {printf("\terro em %s, identificador esperado\n", list->lexema);return;}//\terro
    printf("\n[analisa_declaracao_procedimento end]\n");
}
void analisa_declaracao_funcao(){
    printf("\n[analisa_declaracao_funcao]\n");
    list = list->next;
    printf("\n %s ", list->lexema);
    if(strcmp(list->Simbolo,"sidentificador")==0){
        list = list->next;
        printf("\n %s ", list->lexema);
        if(strcmp(list->Simbolo,"sdoispontos")==0){
            list = list->next;
            printf("\n %s ", list->lexema);
            if(strcmp(list->Simbolo,"sinteiro")==0 || strcmp(list->Simbolo,"sbooleano")==0){
                list = list->next;
                printf("\n %s ", list->lexema);
                if(strcmp(list->Simbolo,"sponto_virgula")==0)
                    analisa_bloco();
            }else {printf("\terro em %s, tipo nao reconhecido\n", list->lexema);return;}//\terro
        }else {printf("\terro em %s, \":\" esperado\n", list->lexema);return;}//\terro
    }else {printf("\terro em %s, identificador esperado\n", list->lexema);return;}//\terro
    printf("\n[analisa_declaracao_funcao end]\n");
}

