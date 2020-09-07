#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 1024

void prolog_gen();
void epilog_gen();
char *coremenu_gen(char *, long int);
int depth_checker(char *, long int);
void coremenu_prolog_gen(long int);
void coremenu_epilog_gen(long int);
void case_prolog_gen(char *, long int, long int);
void case_epilog_gen(char *, long int, long int);
void tabber(long int);
char* string_strip(char*);

int main()
{
    prolog_gen();
    char *s = (char *)malloc((MAX+1) * sizeof(char));
    strcpy(s,"\0");
    fgets(s, MAX, stdin);
    coremenu_gen(s, 0);
    epilog_gen();
    free(s);
}

char* string_strip(char* s)
{
    long int len=strlen(s);
    char* strip=(char*)malloc(len*sizeof(char));
    long int j=0;
    for(long int i=0;i<len && s[i]!='\n' && s[i]!='\0';i++)
    {
        if(s[i]!='\t')
        {
            strip[j++]=s[i];
        }
    }
    strip[j]='\0';
    return strip;
}

int depth_checker(char *s, long int depth)
{
    if (s)
    {
        if(!strlen(s))
        {
            return -1;
        }
        if (depth == 0  || s[depth - 1] == '\t')
        {
            if (s[depth] == '\t')
            {
                return 2;
            }
            return 1;
        }
        return -1;
    }
    return 0;
}

void coremenu_prolog_gen(long int depth)
{
    tabber(depth * 3 + 1);
    printf("scanf(\"%%lu\",&input);\n");
    tabber(depth * 3 + 1);
    printf("while (input)\n");
    tabber(depth * 3 + 1);
    printf("{\n");
    tabber(depth * 3 + 2);
    printf("switch (input)\n");
    tabber(depth * 3 + 2);
    printf("{\n");
}

void coremenu_epilog_gen(long int depth)
{
    tabber(depth * 3 + 3);
    printf("default : break;\n");
    tabber(depth * 3 + 2);
    printf("}\n");
    tabber(depth * 3 + 2);
    printf("scanf(\"%%lu\",&input);\n");
    tabber(depth * 3 + 1);
    printf("}\n");
}

void case_prolog_gen(char *s, long int cases, long int depth)
{
    tabber(depth * 3 + 3);
    printf("case %lu :\n", cases);
    tabber(depth * 3 + 3);
    printf("{\n");
    tabber(depth * 3 + 4);
    char* strip=string_strip(s);
    printf("printf(\"%s\\n\");\n", strip);
    free(strip);
}

void case_epilog_gen(char *s, long int cases, long int depth)
{
    tabber(depth * 3 + 4);
    printf("break;\n");
    tabber(depth * 3 + 3);
    printf("}\n");
}

char *coremenu_gen(char *s, long int depth)
{
    coremenu_prolog_gen(depth);
    long int cases = 1;
    case_prolog_gen(s, cases, depth);
    strcpy(s,"\0");
    fgets(s, MAX, stdin);
    long int depth_check = depth_checker(s, depth);
    while (depth_check > 0)
    {
        if (depth_check == 2)
        {
            s = coremenu_gen(s, depth + 1);
        }
        else
        {
            case_epilog_gen(s, cases++, depth);
            case_prolog_gen(s, cases, depth);
            strcpy(s,"\0");
            fgets(s, MAX, stdin);
        }
        depth_check = depth_checker(s, depth);
    }
    case_epilog_gen(s, cases++, depth);
    coremenu_epilog_gen(depth);
    if (depth_check)
    {
        return s;
    }
    else
    {
        return NULL;
    }
}

void prolog_gen()
{
    printf("#include<stdio.h>\n");
    printf("#include<stdlib.h>\n\n");
    printf("int main()\n{\n");
    printf("\tlong int input;\n");
}

void epilog_gen()
{
    printf("\treturn 0;\n}\n");
}

void tabber(long int value)
{
    for (int i = 0; i < value; i++)
    {
        printf("\t");
    }
}