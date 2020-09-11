#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 1024 // size of string

void prolog_gen();                                // prolog of the generated C code
void epilog_gen();                                // epilog of the generated C code
char *coremenu_gen(char *, long int);             // recursive function which generates a menu
int depth_checker(char *, long int);              // checks the depth of the the sring with respect to the input depth (child, sibling, or ancestor)
void coremenu_prolog_gen(long int);               // prolog of the core menu code
void coremenu_epilog_gen(long int);               // epilog of the core menu code
void case_prolog_gen(char *, long int, long int); // prolog for a switch case
void case_epilog_gen(char *, long int, long int); // epilog for a switch case
void tabber(long int, char *);                    // function to add tabs along with the string while streaming output
char *string_strip(char *);                       // to strip string from tabs and endlines

int main()
{
    prolog_gen();
    char s[MAX + 1];
    fgets(s, MAX, stdin);
    coremenu_gen(s, 0);
    epilog_gen();
    return 0;
}

char *string_strip(char *s)
{
    long int len = strlen(s);
    char *strip = (char *)malloc(len * sizeof(char));
    long int j = 0;
    for (long int i = 0; i < len && s[i] != '\n' && s[i] != '\0'; i++)
    {
        if (s[i] != '\t')
        {
            strip[j++] = s[i]; // stores only non-tab characters
        }
    }
    strip[j] = '\0';
    return strip;
}

int depth_checker(char *s, long int depth)
{ // 1 - sibling, 2 - child, -1 - ancestor (parent/parent's parent and so on) w.r.t the given depth
    if (s)
    {
        if (!strlen(s))
        {
            return -1;
        }
        if (depth == 0 || s[depth - 1] == '\t')
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

void coremenu_prolog_gen(long int depth) // prolog of the core snippet of the menu
{
    tabber(depth * 3 + 1, "scanf(\"%lu\",&input);\n");
    tabber(depth * 3 + 1, "while (input)\n");
    tabber(depth * 3 + 1, "{\n");
    tabber(depth * 3 + 2, "switch (input)\n");
    tabber(depth * 3 + 2, "{\n");
}

void coremenu_epilog_gen(long int depth) // epilog of the core snippet of the menu
{
    tabber(depth * 3 + 3, "default : break;\n");
    tabber(depth * 3 + 2, "}\n");
    tabber(depth * 3 + 2, "scanf(\"%lu\",&input);\n");
    tabber(depth * 3 + 1, "}\n");
}

void case_prolog_gen(char *s, long int cases, long int depth) // prolog of the switch case
{
    tabber(depth * 3 + 3, "");
    printf("case %lu :\n", cases);
    tabber(depth * 3 + 3, "{\n");
    tabber(depth * 3 + 4, "");
    char *strip = string_strip(s);
    printf("printf(\"%s\\n\");\n", strip);
    free(strip);
}

void case_epilog_gen(char *s, long int cases, long int depth) // epilog of the switch case
{
    tabber(depth * 3 + 4, "break;\n");
    tabber(depth * 3 + 3, "}\n");
}

char *coremenu_gen(char *s, long int depth) // the main recursive function which generates the menu
{
    coremenu_prolog_gen(depth);       // prolog
    long int cases = 1;               // indicates the current switch case
    case_prolog_gen(s, cases, depth); // initially supplied with a line which is treated as the first case
    memset(s,0,(MAX+1)*sizeof(char));
    fgets(s, MAX, stdin);                           // reads the next line
    long int depth_check = depth_checker(s, depth); // finds the relative depth of the input line w.r.t to the current depth
    while (depth_check > 0)                         // while its not an ancestor or not a blank string
    {
        if (depth_check == 2) // if its a child
        {
            s = coremenu_gen(s, depth + 1); // recursively calls the function with depth = depth+1
        }
        else // if its a sibling
        {
            case_epilog_gen(s, cases++, depth); // close the previous switch case
            case_prolog_gen(s, cases, depth);   // open the new switch case
            memset(s,0,(MAX+1)*sizeof(char));
            fgets(s, MAX, stdin); // read the next input
        }
        depth_check = depth_checker(s, depth); // check depth
    }
    case_epilog_gen(s, cases++, depth); // close the switch case
    coremenu_epilog_gen(depth);         // close the core menu snippet
    if (depth_check)                    // if the read string s is an ancestor
    {
        return s; // return s
    }
    return NULL; // else return NULL
}

void prolog_gen() // main code prolog
{
    printf("#include<stdio.h>\n#include<stdlib.h>\n\nint main()\n{\n\tlong int input;\n");
}

void epilog_gen() // main code epilog
{
    printf("\treturn 0;\n}\n");
}

void tabber(long int value, char *s) // introduces value*'\t' and prints s
{
    for (int i = 0; i < value; i++)
    {
        printf("\t");
    }
    printf("%s", s);
}