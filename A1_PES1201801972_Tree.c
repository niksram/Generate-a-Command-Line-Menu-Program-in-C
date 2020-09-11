#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 1024

typedef struct Node
{
    struct Node *sibling;
    struct Node *child;
    char *word;
} Node;

typedef struct Tree
{
    Node *root;
} Tree;

//Tree Functions

Tree *tree_init();
Node *node_create(char *s);
void tree_maker(Tree *);
char *tree_recursive_maker(char *, Node *, long int);
int depth_checker(char *, long int);
char *string_strip(char *);
void tree_trasher(Tree *);
void tree_recursive_trasher(Node *);
void free_node(Node *);

//Generate Function

void prolog_gen();
void main_prolog_gen();
void epilog_gen();
void coremenu_gen(Tree *);
void coremenu_recursive_gen(Node *, long int);
void coremenu_prolog_gen(long int);
void coremenu_epilog_gen(long int);
void case_prolog_gen(Node *, long int, long int);
void case_epilog_gen(Node *, long int);
void tabber(long int, char *);
void function_gen(char *, long int, long int);
// void tree_print(Node* ,long int );

int main()
{
    Tree *tree = tree_init();
    prolog_gen();
    tree_maker(tree);
    main_prolog_gen();
    coremenu_gen(tree);
    epilog_gen();
    tree_trasher(tree);
    return 0;
}

Tree *tree_init()
{
    Tree *tree = (Tree *)malloc(sizeof(Tree));
    tree->root = node_create(NULL);
    return tree;
}

Node *node_create(char *s)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->child = NULL;
    node->sibling = NULL;
    node->word = s ? string_strip(s) : NULL;
    return node;
}

void tree_maker(Tree *tree)
{
    char s[MAX + 1];
    fgets(s, MAX, stdin);
    tree_recursive_maker(s, tree->root, 0);
}

void function_gen(char *s, long int depth, long int cases)
{
    char *strip = string_strip(s);
    printf("void function_%s_%ld_%ld()\n{\n", strip, depth, cases);
    printf("\tprintf(\"%s\\n\");\n", strip);
    printf("}\n\n");
    free(strip);
}

char *tree_recursive_maker(char *s, Node *parent, long int depth)
{
    long int cases = 1;
    Node *node = parent->child = node_create(s);
    function_gen(s, depth, cases++);
    memset(s,0,(MAX+1)*sizeof(char));
    fgets(s, MAX, stdin);
    long int depth_check = depth_checker(s, depth);
    while (depth_check > 0)
    {
        if (depth_check == 2)
        {
            s = tree_recursive_maker(s, node, depth + 1);
        }
        else
        {
            node = node->sibling = node_create(s);
            function_gen(s, depth, cases++);
            memset(s,0,(MAX+1)*sizeof(char));
            fgets(s, MAX, stdin);
        }
        depth_check = depth_checker(s, depth);
    }
    if (depth_check)
    {
        return s;
    }
    return NULL;
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
            strip[j++] = s[i];
        }
    }
    strip[j] = '\0';
    return strip;
}

int depth_checker(char *s, long int depth)
{
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

void coremenu_prolog_gen(long int depth)
{
    tabber(depth * 3 + 1, "scanf(\"%lu\",&input);\n");
    tabber(depth * 3 + 1, "while (input)\n");
    tabber(depth * 3 + 1, "{\n");
    tabber(depth * 3 + 2, "switch (input)\n");
    tabber(depth * 3 + 2, "{\n");
}

void coremenu_epilog_gen(long int depth)
{
    tabber(depth * 3 + 3, "default : break;\n");
    tabber(depth * 3 + 2, "}\n");
    tabber(depth * 3 + 2, "scanf(\"%lu\",&input);\n");
    tabber(depth * 3 + 1, "}\n");
}

void case_prolog_gen(Node *node, long int depth, long int cases)
{
    tabber(depth * 3 + 3, "");
    printf("case %lu :\n", cases);
    tabber(depth * 3 + 3, "{\n");
    tabber(depth * 3 + 4, "");
    printf("function_%s_%ld_%ld();\n", node->word, depth, cases);
}

void case_epilog_gen(Node *node, long int depth)
{
    tabber(depth * 3 + 4, "break;\n");
    tabber(depth * 3 + 3, "}\n");
}

void coremenu_gen(Tree *tree)
{
    coremenu_recursive_gen(tree->root, 0);
}

void coremenu_recursive_gen(Node *parent, long int depth)
{
    coremenu_prolog_gen(depth);
    Node *node = parent->child;
    long int cases = 1;
    while (node)
    {
        case_prolog_gen(node, depth, cases++);
        if (node->child)
        {
            coremenu_recursive_gen(node, depth + 1);
        }
        case_epilog_gen(node, depth);
        node = node->sibling;
    }
    coremenu_epilog_gen(depth);
}

void prolog_gen()
{
    printf("#include<stdio.h>\n#include<stdlib.h>\n\n");
}

void main_prolog_gen()
{
    printf("int main()\n{\n\tlong int input;\n");
}

void epilog_gen()
{
    printf("\treturn 0;\n}\n");
}

void tabber(long int value, char *s)
{
    for (int i = 0; i < value; i++)
    {
        printf("\t");
    }
    printf("%s", s);
}

void free_node(Node *node)
{
    free(node->word);
    free(node);
}

void tree_trasher(Tree *tree)
{
    tree_recursive_trasher(tree->root);
    free(tree);
}
void tree_recursive_trasher(Node *node)
{
    while (node)
    {
        if (node->child)
        {
            tree_recursive_trasher(node->child);
        }
        Node *temp = node;
        node = node->sibling;
        free_node(temp);
    }
}

// void tree_print(Node* node,long int depth)
// {
//     while(node)
//     {
//         tabber(depth,node->word);
//         printf("\n");
//         if(node->child)
//         {
//             tree_print(node->child,depth+1);
//         }
//         node=node->sibling;
//     }
// }