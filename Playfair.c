#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define ROWS 5
#define COLUMNS 5

void encrypt(char **grid , char *ch , char * res); // function for encrypt array
void decrypt(char **grid , char *ch , char * res); // function for decrypt array
void create_grid(char *key , char **grid);		   // function for create the gide with key + left other letters
char * str4en_create(char * ch , char filler);	   //this function for convert input character array in to a formate that encryption is supported(playfair)//return pointer to new array
void fix_str(char * text , int keepspaces);		   //function for remove spaces and unwanted characters from input array
int check_key_duplicates(char * key);			   //fuction for check duplicate letters in key returns 0 if no duplicates return 1 if duplicates are found
void erase(char * text , int index);			   //erase index in a array
void search_grid(char **grid , char ch , int * rnc); //search letter in grid put column number and row number in to input pointer (*rnc)
int mod(int x , int y);								 //function for find modulo operation of a values(specialy for negative values)

int main(int argc , char *argv[])
{
	//pointers
	char *text;			//input text
	char *key;			//key input
	char **grid;		//2D array grid
	char *str4en;		
	char *result;		
	
	
	int  goin4encypt = 0;
	int  goin4decypt = 0;
	int  keepspaces = 0;
	char filler = 'Z';
	
	
	if(argc < 5)
	{
		printf("<.\\Playfair.exe> [options] <\"Text\">\n");
		printf("[options] : -e <\"plainText\">   To encrypt\n");
		printf("            -d <\"cipherText\">  To decrypt\n");
		printf("            -s keep spaces in plain text\n");
		printf("            -k <key>             key for encryption and decryption");
		printf("            -f <filler>          character for filler");
		return 0;
	}

	
	
	
	//2D dynamic array for grid of alphabets

	grid = (char **)malloc(ROWS * sizeof(char *));
	
	for(int i = 0 ; i < COLUMNS ;i++)
	{
		grid[i] = (char *)malloc(COLUMNS * sizeof(char ));
		if(grid[i] == NULL)
			return -1;
	}
	
	
	
	
	
	//handle input arguments
	
	for(int a = 1 ; a < argc ; a++)
	{
		if((strcmp(argv[a],"-k")==0))
		{
			key  = strupr(argv[a+1]);
			a++;
			continue;
		}
		
		if((strcmp(argv[a],"-e")==0))
		{
			text  = strupr(argv[a+1]);
			goin4encypt = 1;
			a++;
			continue;
		}
		if((strcmp(argv[a],"-s")==0))
		{
			keepspaces = 1;
			continue;
		}
		if((strcmp(argv[a],"-f")==0))
		{
			filler = *strupr(argv[a+1]);
			a++;
			continue;
		}
		if((strcmp(argv[a],"-d")==0))
		{
			text  = strupr(argv[a+1]);
			goin4decypt = 1;
			a++;
			continue;
		}
		else{
			printf("<.\\Playfair.exe> [options] <\"Text\">\n");
			printf("[options] : -e <\"plainText\">   To encrypt\n");
			printf("            -d <\"cipherText\">  To decrypt\n");
			printf("            -s keep spaces in plain text\n");
			printf("            -k <key>             key for encryption and decryption");
			printf("            -f <filler>          character for filler");
			return -1;
		}
		
	}
	
	
	//fixing plaintext
	//remove characters other than letters
	//remove spaces if needed
	fix_str(text , keepspaces);
	
	//this line for remove spaces and invalid characters in key
	fix_str(key , 1);
	
	str4en = (char *)malloc(strlen(text)*sizeof(char));
		
	//Dynamic array for store result of encryption or decryption
	result = (char *)malloc(strlen(text)*sizeof(char));
	if(result == NULL)
	{
		printf("memory allocation failed");
	}
	
	char *token;
	token = (char *)malloc(strlen(text)*sizeof(char));
	if(token == NULL)
	{
		printf("memory allocation failed");
	}
	int token_counter = 0;
	
	char *tokenResult;
	tokenResult = (char *)malloc(strlen(text)*sizeof(char));
    if(token == NULL)
	{
		printf("memory allocation failed");
	}
	
	//only if key doesnt include duplicates
	if(!check_key_duplicates(key))
	{
		//generate the grid
		create_grid( key , grid);
		
		//going to encrypt the text
		if(goin4encypt == 1)
		{
			//without spaces
			if(!keepspaces)
			{
				str4en = realloc(str4en , strlen(text)+1*sizeof(char));
				if(!str4en)
				{
					printf("memory reallocation failed");
				}
				//convert plain text to a in a way that encrryption is supported
				str4en = str4en_create(text , filler);
				
				
				printf("\n%s => %s\n" ,text, str4en);
				encrypt(grid,str4en,result);
				printf("\ncipher text = %s\n" , result);
			}
			//with spaces	
			else
			{
				printf("\n%s =>" ,text);
				for(int b = 0 ; b <= strlen(text) ; b++)
				{
					
					if(text[b] == ' ' ||  text[b] == '\0')
					{
						token[token_counter] = '\0';
						token_counter=0;
						
						token = str4en_create(token , filler);
						printf("%s " , token);
						
						encrypt(grid,token,tokenResult);
						
						strcat(result , " ");
						strcat(result , tokenResult);
						
						continue;
						
					}
					token[token_counter++] = text[b];
					
				}
				printf("\ncipher text = %s\n" , result);
			}
			
		}
		
		//decrypting 
		//in here not going to deal with spaces(if any remove space)
		if(goin4decypt == 1)
		{
			decrypt(grid,text,result);
			printf("plain Text :%s"  , result);
		}
		
		
		
	}else
	{
		printf("key is wrong..");
	}

	
	//free(str4en);
	free(result);
	free(grid);
	return 0;
}



void create_grid(char *key , char **grid)
{
	//creating grid of alphabets
	
	char alphabet[] = "ABCDEFGHIKLMNOPQRSTUVWXYZ";
	int i = 0 , j = 0 , keyinAl = 0;
	
	printf("grid:\n");
	for(int row = 0 ; row < ROWS ; row++)
	{
		for(int column = 0 ; column < COLUMNS ; column++)
		{
			//put key elements in grid
			if(i < strlen(key))
			{
				grid[row][column] = key[i++];
				printf("%c\t" , grid[row][column]);
				continue;
			}
			
			//put other letters in grid(alphabet - key)
			for(j; j < strlen(alphabet) ; j++)
			{
				for(int jj = 0 ; jj < strlen(key) ; jj++)
				{
					keyinAl = 0;
					if(alphabet[j] == key[jj])
					{
						keyinAl =1;
						break;
					}
				}
				if(keyinAl == 0)
					break;
			}
			grid[row][column] = alphabet[j];
			j++;
			printf("%c\t" , grid[row][column]);
		}
		printf("\n");
	}
}


void encrypt(char **grid , char *ch , char * res)
{
	int count = 0;
	int char_1[2]; //1st element for row 2nd for column
	int char_2[2]; //1st element for row 2nd for column

	//for loop for encrypt piars of charcters
	for(int i = 0 ; i < strlen(ch) ; i+=2)
	{
		search_grid(grid , ch[i] , char_1);			//take row and column indexes for 1st charcter
		search_grid(grid , ch[i+1],char_2);			//take row and column indexes for 12nd charcter
		
		//encryption part 
		//3 cases
			//1. when pair is in same row
		if(char_1[0] == char_2[0])
		{
			res[count++] = grid[char_1[0]] [mod((char_1[1]+1) , ROWS)];
			res[count++] = grid[char_2[0]] [mod((char_2[1]+1) , ROWS)];	
		}
			//2. when pair is in same column
		else if(char_1[1] == char_2[1])
		{
			res[count++] = grid[ mod((char_1[0]+1) , COLUMNS) ] [char_1[1]];	
			res[count++] = grid[ mod((char_2[0]+1) , COLUMNS) ] [char_2[1]];	
		}
			//3. when pair is in different rows and columns
		else
		{
			res[count++] = grid[ char_1[0] ] [ char_2[1] ];	
			res[count++] = grid[ char_2[0] ] [ char_1[1] ];	
			
		}
		
		
	}


}

//most of the parts are same as encryption algorithm only difference is shift element by -1
void decrypt(char **grid , char *ch , char * res)
{
	int count = 0;
	int char_1[2]; //1st element for row 2nd for column
	int char_2[2]; //1st element for row 2nd for column

	
	for(int i = 0 ; i < strlen(ch) ; i+=2)
	{
		search_grid(grid , ch[i] , char_1);
		search_grid(grid , ch[i+1],char_2);
		
		if(char_1[0] == char_2[0])
		{
			res[count++] = grid[char_1[0]] [mod((char_1[1]-1) , ROWS)];
			res[count++] = grid[char_2[0]] [mod((char_2[1]-1) , ROWS)];	
		}
		else if(char_1[1] == char_2[1])
		{
			res[count++] = grid[ mod((char_1[0]-1) , COLUMNS) ] [char_1[1]];	
			res[count++] = grid[ mod((char_2[0]-1) , COLUMNS) ] [char_2[1]];	
		}
		else
		{
			res[count++] = grid[ char_1[0] ] [ char_2[1] ];	
			res[count++] = grid[ char_2[0] ] [ char_1[1] ];	
			
		}
		
		
	}
	
	
}

//modulo oparation
int mod(int x , int y)
{
	if(x >= 0 )
		return (x)%y ;
	
	return mod(y+x , y) ;

}


//modify character array to encrypt it properly
//return pointer to new array(no duplicates in pairs) 
char * str4en_create(char * ch , char filler)
{
	
	//printf("\nfiller character = %c\n" , filler);
	
	char *newtext;
	size_t size = strlen(ch);
	newtext = (char *)malloc(sizeof(char ) * size);
	
	
	if(newtext == NULL)
	{
		printf("\nmemory allocation failed for splited text");
		exit(0);
	}
	
	for(int i = 0 , j = 0 ; i < strlen(ch) ; i++)
	{
		
	
		newtext[j++] = ch[i];
		
		//if  duplicate characters found within 1 pair of letters 
		//filler character will be added after 1st duplicate character
		if(ch[i] == ch[i+1] && j%2 != 0 )
		{
			size += sizeof(char);
			newtext=realloc(newtext,size);
			newtext[j] = filler;
			j+=1;
		}
		
	}
	//if text size is odd, add filler character to end of the text
	if(strlen(newtext)%2 == 1)
	{
		newtext = (char *)realloc(newtext , size+sizeof(char));
		if(ch ==NULL)
		{
			printf("memory reallocation failed..\n");
		}
		strncat(newtext , &filler , 1);
		
	}
	
	return newtext;
}


void search_grid(char **grid , char ch , int * rnc)
{
	for(int row = 0 ; row < ROWS ; row++)
	{
		for(int column = 0 ; column < COLUMNS ; column++)
		{
			if(ch == grid[row][column])
			{
				rnc[0] = row;
				rnc[1] = column;
			}
		}
	}

}
void fix_str(char * text , int keepspaces)
{
	for(int i = 0 ; i < strlen(text) ; i++)
	{
		//replace J from I
		if(text[i] == 'J')
			text[i] = 'I';
			
		if( !(text[i] >= 'A' && text[i] <= 'Z') 
			&& (!keepspaces || text[i] != ' ' && keepspaces) )
		{
			erase(text , i);
			i--;
		}

	}
	
}

void erase(char * text , int index)
{

	for(int i = index ; i < strlen(text) ; i++)
		text[i]=text[i+1];
	text[ strlen(text)] = '\0';
	

	
	
}

int check_key_duplicates(char * key)
{
	for(int i = 0 ; i < strlen(key) ; i++)
	{
		for(int j = i+1 ; j < strlen(key) ; j++)
		{
			if(i != j && key[i] == key[j])
			{
				return 1;
			}
		}
	}
	return 0;
}





