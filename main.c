#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include <unistd.h>
#include <stdio.h>  /* defines FILENAME_MAX */
//#define WINDOWS  /* uncomment this line to use it for windows.*/
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
int isKeyword(char buffer[]){
    char keywords[2][5] = {"int","loop"};
    
    int i, flag = 0;
    
    for(i = 0; i < 2; ++i){
        if(strcmp(keywords[i], buffer) == 0){
            flag = 1;
            break;
        }
    }
    
    return flag;
}
int isIdentifier(char buffer[]){
    
    int flag = 0;
    int k = 0 ;
    if (buffer[0]=='_'){
    	flag=0;
    	return flag;
	}
    while (buffer[k]!=NULL){
    	if (isalpha(buffer[k]) != 0 | buffer[k] == '_'){
    		flag=1;
		}
        else{
        	flag=0;
        	return flag;
		}
		k++;
	}
    
    return flag;
}
int isOperator(char buffer[]){
    char operators[3][5] = {"add","sub","move"};
    int i, flag = 0;
    for(i = 0; i < 3; ++i){
        if(strcmp(operators[i], buffer) == 0){
            flag = 1;
            break;
        }
    }
    
    return flag;
}
int isIterationOp(char buffer[]){
    char iterationOp[1][6] = {"times"};
    int flag = 0;
    if(strcmp(iterationOp[0], buffer) == 0){
        flag = 1;
    }
    return flag;
}
int isAssignment(char buffer[]){
    char assignment[2][6] = {"to","from"};
    int i,flag = 0;
    for(i = 0; i < 2; ++i){
        if(strcmp(assignment[i], buffer) == 0){
            flag = 1;
            break;
        }
    }
    return flag;
}



int main(){
    char ch, buffer[20]; //projede max uzunluk 20 denilmiþti.
    int endofLineCount = 0,operatorCount=0,keywordCount=0,identifierCount=0,parantesisCount = 0;
    FILE *fp;
    int j=0; int kj=0;
    int isComment = 0; int iterationOpCount = 0;
    int isStringContent = 0,satirsay=1;
    int isParanthesis=0;
    int isNumber=0;
    
	int isError=0;
	char intControl[10];
    int isPunch=0;
    char filename[8] ;
    printf("Dosya adini uzantisiz olarak girin: ");
    scanf("%s",filename);
    
    
    char buff[FILENAME_MAX+1000000];
    GetCurrentDir( buff, FILENAME_MAX );
    buff[strlen(buff)] = '/';
    
    int i ;
    for (i=0 ; i<sizeof(filename)-1; i++){
    	if(isalpha(filename[i])) {
    		buff[strlen(buff)] = filename[i];	
		}
    	
    	
	}
	buff[strlen(buff)] = '.';
	buff[strlen(buff)] = 'b';
	buff[strlen(buff)] = 'a';
	printf("Current working dir: %s\n", buff);
    
    fp = fopen(buff,"r");
    char moveDeger[20];
    
    int isTo=0;
   
    int moveDegerAlindi = 0;
    if(fp == NULL){
        printf("error while opening the file\n");
        exit(0);
    }
    
  while((ch = fgetc(fp)) != EOF){
        isParanthesis=0;
        if (ch=='-' || ch=='+' || ch==' ' || ch=='.' || isdigit(ch) & (kj<10)){
            
            intControl[kj++] = ch;
			int k;
            for (k=0; k<kj ; k++){
                if(isdigit(intControl[k])){
                	int i;
                    for(i=0; i<kj ; i++){
                        if(intControl[i]=='.'){
                            isError=1;
                            break;
                        }
                    }
                }
                else if((intControl[k]=='+') & (isdigit(intControl[k+1]))){
                    isError = 1;
                    break;
                    
                }
                else if((intControl[k]=='-') & (intControl[k+1] == ' ' || intControl[k+1]=='-') ){
                    isError = 1;
                    break;
                }
                
            }
        }
        
        else{
            if(kj>0){
                intControl[kj] = '\0';
                kj = 0;
            }
            
            if(isError){
                printf("%d. satir - NOT A VALID INTEGER -> %s\n",satirsay,intControl);
                isError=0;
                buffer[j] = '\0';
                j = 0;
            }
        }
        if(!isError){
            if(ch == '{'){
                isComment = 1;
                while(isComment){
                    printf("%c",ch);
                    ch = fgetc(fp);
                    if(ch=='}'){
                        isComment=0;
                        printf("%c is comment\n",ch);
                    }
                }
            }
            
            if(ch == ','){
                isPunch=1;
            }
            
            if (ch == '"'){
                isStringContent = 1;
                while(isStringContent){
                    printf("%c",ch);
                    ch = fgetc(fp);
                    if(ch=='"'){
                        isStringContent=0;
                        printf("%c is string content\n",ch);
                        ch = fgetc(fp);
                        if(ch == ','){
                            printf(", is punctuation\n");
                        }
                    }
                }
            }
            
            
            else if(isalnum(ch) || ch=='-'){
                buffer[j++] = ch;
                
            }
            
            else if((ch == ' ' || ch == '.' || ch == '[' || ch==']') && (j != 0)){
                
                buffer[j] = '\0';
                j = 0;
                isNumber=0;
                if (isdigit(buffer[0]) || buffer[0] == '-'){
                    isNumber = 1;
                    int k = 1;
                    
                    while(buffer[k]!=NULL){
                        if(!(isdigit(buffer[k]))){
                            isNumber=0;
                            break;
                        }
                        else{
                            isNumber=1;
                        }
                        k++;
                    }
                }
                if(buffer[0] == '-'){
                    if (!isdigit(buffer[1])){
                        isNumber=0;
                    }
                }
                if (isNumber){
                    printf("%s is number\n",buffer);
                    
                }
                if (!isNumber){
                    if(isKeyword(buffer) == 1){
                        
                        printf("%s is keyword\n", buffer);
                        keywordCount++;
                    }
                    else if (isOperator(buffer)){
                        printf("%s is operator\n",buffer);
                        operatorCount++;
                        
                    }
                    else if (isAssignment(buffer)){
                        printf("%s is assignment\n",buffer);
                        
                    }
                    
                    else if (strcmp("newline", buffer) == 0){
                        printf("%s is new line\n",buffer);
                    }
                    
                    else{
                        if (isIterationOp(buffer)){
                            printf("%s is iteration operator\n", buffer);
                            iterationOpCount++;
                            
                            
                        }
                        else if (isIdentifier(buffer)){
                            printf("%s is identifier\n", buffer);
                            identifierCount++;
                        }
                        else{
                        	if (strcmp("-",buffer)!=0){
                        		printf("* %d. satirda hata - GECERSIZ IDENTIFIER -> '%s' *\n",satirsay,buffer);
							}
                            
                        }
                        
                    }
                    if (isPunch){
                        printf(", is punctuation\n");
                        isPunch = 0;
                    }
                    
                }
            }
            
            // Süslü parantezler yorum satýrýný temsil ettiði için onlarý bu kategoriye dahil etmedik.
            // Verilen örneklerde yalnýzca köþeli parantezler bulunduðu için sadece onlarý kontrol ettik.
            
            if(ch == '[' | ch == ']'){
                parantesisCount++;
                satirsay++;
                printf("%c is paranthes\n",ch);
            }
            
            
            if (ch == '.'){
                endofLineCount++;
                printf(". is end of line character\n");
                satirsay++;
            }
            
        }
 
        }
        

    printf("*****\n");
    printf("End of line character count: %d\n",endofLineCount);
    printf("Keyword count: %d\n",keywordCount);

    printf("Identifier count: %d\n",identifierCount);
    printf("Parantesis count: %d\n",parantesisCount);
    
    fclose(fp);
    
    return 0;
}
