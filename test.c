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
