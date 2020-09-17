#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 1024 // the maximum length of the string can be atered here, default 1024 is set here

typedef struct Node // node of the tree
{
    struct Node *sibling; // sibling pointer
    struct Node *child;   // child pointer
    char *word;           // string is stored in this address
} Node;

typedef struct Tree // structure whicch holds the pointer to the root of the tree
{
    Node *root;
} Tree;

//Tree Functions

Tree *tree_init();                                    // initialise tree
Node *node_create(char *s);                           // create node
void tree_maker(Tree *);                              // generate tree as well as print functions
char *tree_recursive_maker(char *, Node *, long int); // recurive function used inside 'tree_maker()'
int depth_checker(char *, long int);                  // checks the relative depth of the input string with given depth
char *string_strip(char *);
void tree_trasher(Tree *);           // clears tree by freeing the nodes
void tree_recursive_trasher(Node *); // used inside 'tree_trasher'
void free_node(Node *);              // free a single node

//Generate Function

void prolog_gen();                                // '#include' of the program are printed
void main_prolog_gen();                           // prints 'int main()' for the generated program
void epilog_gen();                                // prints 'return 0' as well as closes the generatedprogram
void coremenu_gen(Tree *);                        // the main function which parses the tree and generates the program
void coremenu_recursive_gen(Node *, long int);    // used recursively inside 'coremenu_gen()'
void coremenu_prolog_gen(long int);               // prolog of main menu snippet of program
void coremenu_epilog_gen(long int);               // epilog of main menu snippet of program
void case_prolog_gen(Node *, long int, long int); // prolog of switch case of program
void case_epilog_gen(Node *, long int);           // epilog of switch case of program
void tabber(long int, char *);                    // given a number and pointer to string, it prints tabs and then the string
void function_gen(char *, long int, long int);    // generates function for each level of the menu
void tree_print(Node *, long int);                // printd the tree

int main()
{
    Tree *tree = tree_init(); // initialses the tree
    prolog_gen();
    tree_maker(tree); // generate the tree as well as functions
    tree_print(tree->root, 0);
    main_prolog_gen();
    coremenu_gen(tree); // generate the core menu code from the tree
    epilog_gen();
    tree_trasher(tree); // frees tree
    return 0;
}

Tree *tree_init()
{
    Tree *tree = (Tree *)malloc(sizeof(Tree)); // memory allocated
    tree->root = node_create(NULL);            // root node is created by default here
    return tree;
}

Node *node_create(char *s) // GPT is stores as a binary tree in the form of nodes pointing to its child as well as siblings
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->child = NULL;
    node->sibling = NULL;
    node->word = s ? string_strip(s) : NULL; // the string which must be printed in the function is dtored here
    return node;
}

void tree_maker(Tree *tree)
{
    char s[MAX + 1];
    if (fgets(s, MAX + 1, stdin)) // gets the first input
        tree_recursive_maker(s, tree->root, 0);
}

void function_gen(char *s, long int depth, long int cases) // generates a function with 'printf' of the given string
{
    char *strip = string_strip(s);
    printf("void function_%s_%ld_%ld()\n{\n", strip, depth, cases); // naming scheme is function_the-string_depth_cases. hence same strings at different levels don't interfere
    printf("\tprintf(\"%s\\n\");\n", strip);
    printf("}\n\n");
    free(strip);
}

int depth_checker(char *s, long int depth) // its used to find the relative depth of the input string w.r.t to the given depth
{                                          // **convention used here -1: ANCESTOR, 0: ERROR, 1: SIBLING, 2: CHILD
    if (s)                                 // if s not NULL
    {
        if (!strlen(s) || s[0] == '\n') // if len=0, then its an ancestor
        {
            return -1;
        }
        if (depth == 0 || s[depth - 1] == '\t') // if the depth-1 is tab
        {
            if (s[depth] == '\t') // if depth is tab
            {
                return 2; // then its child
            }
            return 1; // else its a sibling
        }
        return -1; // else its an ancestor
    }
    return 0;
}

char *tree_recursive_maker(char *s, Node *parent, long int depth) // the crucial function which is used to generate the tree recursively
{
    long int cases = 1;
    Node *node = parent->child = node_create(s);    //  node is created given a string as the child of the given node
    function_gen(s, depth, cases++);                // function to print the respective string is generated
    memset(s, 0, (MAX + 1) * sizeof(char));         // clears residual string
    fgets(s, MAX + 1, stdin);                       // input string
    long int depth_check = depth_checker(s, depth); // find out the relative heirarchy of the input string with current depth
    while (depth_check > 0)                         // if input string is not an ancestor to the current depth (either sibling or child)
    {
        if (depth_check == 2) // if input string must be a child
        {
            s = tree_recursive_maker(s, node, depth + 1); // recurively call the function again passing this node as the parent
        }
        else // if input string must be a sibling
        {
            node = node->sibling = node_create(s);  // create a new node and make it its sibling
            function_gen(s, depth, cases++);        // generate function
            memset(s, 0, (MAX + 1) * sizeof(char)); // cear residual string
            fgets(s, MAX + 1, stdin);               // input string
        }
        depth_check = depth_checker(s, depth); // check depth again
    }
    if (depth_check) // if depth value is not 0 (which refers to error and hence ancestor)
    {
        return s; // exit this recursive call
    }
    return NULL;
}

char *string_strip(char *s) // to strip the input string from tabs and endlines
{
    long int len = strlen(s);
    char *strip = (char *)malloc(len * sizeof(char)); // new string
    long int j = 0;
    for (long int i = 0; i < len && s[i] != '\n' && s[i] != '\0'; i++)
    {
        if (s[i] != '\t') // remove tabs
        {
            strip[j++] = s[i];
        }
    }
    strip[j] = '\0'; // replace endline/null-character with null-character
    return strip;
}

void coremenu_prolog_gen(long int depth) // scanf, while and switch case prolog of the menu for each depth is generated here
{
    tabber(depth * 3 + 1, "scanf(\"%lu\",&input);\n");
    tabber(depth * 3 + 1, "while (input)\n");
    tabber(depth * 3 + 1, "{\n");
    tabber(depth * 3 + 2, "switch (input)\n");
    tabber(depth * 3 + 2, "{\n");
}

void coremenu_epilog_gen(long int depth) // scanf, while epilog and switch default case of the menu for each depth is generated here
{
    tabber(depth * 3 + 3, "default : break;\n");
    tabber(depth * 3 + 2, "}\n");
    tabber(depth * 3 + 2, "scanf(\"%lu\",&input);\n");
    tabber(depth * 3 + 1, "}\n");
}

void case_prolog_gen(Node *node, long int depth, long int cases) // prints the switch case prolog snippet
{
    tabber(depth * 3 + 3, "");
    printf("case %lu :\n", cases);
    tabber(depth * 3 + 3, "{\n");
    tabber(depth * 3 + 4, "");
    printf("function_%s_%ld_%ld();\n", node->word, depth, cases); //function call
}

void case_epilog_gen(Node *node, long int depth) // prints the switch case epilog snippet
{
    tabber(depth * 3 + 4, "break;\n");
    tabber(depth * 3 + 3, "}\n");
}

void coremenu_gen(Tree *tree) // generated the menu code by parsing the tree structure
{
    coremenu_recursive_gen(tree->root, 0);
}

void coremenu_recursive_gen(Node *parent, long int depth) // crucial function for generating code from the tree
{
    coremenu_prolog_gen(depth); // prolog
    Node *node = parent->child; // accesses the nodes child
    long int cases = 1;
    while (node) // iteraion
    {
        case_prolog_gen(node, depth, cases++); // if node is in same level (sibling) then generate new switch case
        if (node->child)                       // if child exists then recursively call the function to generate the next level of while and switch case
        {
            coremenu_recursive_gen(node, depth + 1);
        }
        case_epilog_gen(node, depth); // switch epilog to close the case
        node = node->sibling;
    }
    coremenu_epilog_gen(depth);
}

void prolog_gen() // generates the pre-processive directives
{
    printf("#include<stdio.h>\n#include<stdlib.h>\n\n");
}

void main_prolog_gen() // prints 'int main' for the main code
{
    printf("int main()\n{\n\tlong int input;\n");
}

void epilog_gen() // prints the closing snippet of the code
{
    printf("\treturn 0;\n}\n");
}

void tabber(long int value, char *s) // given number n and a string, it prints n tabs and then the string (used to indent code)
{
    for (int i = 0; i < value; i++)
    {
        printf("\t");
    }
    printf("%s", s);
}

void free_node(Node *node) // free individual node
{
    free(node->word);
    free(node);
}

void tree_trasher(Tree *tree) // deletes the tree
{
    tree_recursive_trasher(tree->root);
    free(tree);
}

void tree_recursive_trasher(Node *node) // recusrive function used to free the tree
{
    while (node) // if node exists
    {
        if (node->child) // if child exists then call function recursively
        {
            tree_recursive_trasher(node->child);
        }
        Node *temp = node;
        node = node->sibling; // node->sibing to traverse
        free_node(temp);
    }
}

void tree_print(Node *node, long int depth) // prints the tree, used for the purpose of debugging
{
    while (node)
    {
        printf("// ");
        tabber(depth, node->word);
        printf("\n");
        if (node->child)
        {
            tree_print(node->child, depth + 1);
        }
        node = node->sibling;
    }
}