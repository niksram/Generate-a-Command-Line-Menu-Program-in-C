**DESCRIPTION**

The following program is designed to generate a C program for a ***command line menu interface*** given the Tree heirarchy of the menu as input.
***

**FORMAT**
***menu.dat*** is used to represent the structure of the menu. The format is as follows:
* all lines from the starting of the file beginning with **'#'** is considered as preprocessor directives.
* the number of tabs (**'\t'**) in the following file indicates the depth of the heirarchy.
* *Note that using spaces instead if **'\t'** for tabs is not valid*.
***

**PROCEDURE** - *To generate the C Menu Program* 
```bash
make menuc=output.c menudat=test.dat
```
* ***test.dat*** is the the tree structure which indicates the menu hierarchy
* ***output.c*** is the name of the generated C program
***

**EXAMPLE**

***Tree.dat***
```dat
#include<string.h>
char s[100];strcpy(s,"hello");printf("string initialised\n");
    printf("%s Tom",s);
    printf("%s Jerry",s);
printf("welcome)
```
***Generated C Code***

```c
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

int main()
{
	long int input;
	scanf("%lu",&input);
	while (input)
	{
		switch (input)
		{
			case 1 :
			{
				char s[100];
				stcpy(s,"hello");
				printf("string initialised\n");
				scanf("%lu",&input);
				while (input)
				{
					switch (input)
					{
						case 1 :
						{
							printf("%s Tom\n");
							break;
						}
						case 2 :
						{
							printf("%s Jerry\n");
							break;
						}
						default : break;
					}
					scanf("%lu",&input);
				}
				break;
			}
			case 2 :
			{
				printf("welcome\n");
				break;
			}
			default : break;
		}
		scanf("%lu",&input);
	}
	return 0;
}
```