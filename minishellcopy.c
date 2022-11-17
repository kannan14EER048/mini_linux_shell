/****************************************************************Mini linux shell************************************************************************/

/********header files for i/o , system calls and strings*******/
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/types.h>

/*******************function declarations**********************/
void prompt();
void mycat(char[]);
void myhead(char []);
void mytail(char []);
void mycp(char [],char[]);
void mymv(char [],char[]);
void myrm(char []);
void myhistory();

/********main function - where the execution starts*************/
int main()
{
									//for the initial screen of shell
	int repeat=1;
	do
	{	prompt();						//prints the prompt	
		static char command[30];
		scanf("%s",&command);					//getting user command
		char history [2048]=" ";				
		char flName [1024]=" ";					
		char flName2 [1024]=" ";
		
       		if(strncmp(command,"exit",4)==0)			
		{	
			exit(1);					//exits the loop for "exit" command

		}

		int pair=fork();					//spawns a child process
		
		if(pair==0)						//child process starts
		{		
	
			if( (strcmp(command,"mycat")==0))		//compares the string with input
			{	
				scanf("%s",&flName);			//gets argument 1
				mycat(flName);				
				
			}
			else if(strncmp(command,"myhead",6)==0)
			{
				scanf("%s",&flName); 
				myhead(flName);
			
			}
			else if(strncmp(command,"mytail",6)==0)
			{	
				scanf("%s",&flName); 
				mytail(flName);
				
			}
			else if(strncmp(command,"myrm",4)==0)
			{
				scanf("%s",&flName);
				myrm(flName); 
				
			}
			else if(strcmp(command,"mymv")==0)
			{	
				
				scanf("%s",&flName); 
				scanf("%s",&flName2); 
				mymv(flName,flName2);
				
			}
			else if(strcmp(command,"mycp")==0)
			
			{
				
				scanf("%s",&flName); 
				scanf("%s",&flName2); 
				mycp(flName,flName2);
				
			}
			else if(strcmp(command,"myhistory")==0)
			{	
				
				myhistory();						//function call
				
			}
			else
			{
				printf("command not found...\n");			//if the input command is unknown
			
			}
			
			/*********writing command and arguments into history.txt *******/
			int fdhistory=open("history.txt",O_CREAT|O_WRONLY|O_APPEND,0666);
			write(fdhistory,command,strlen(command));
			fdhistory=open("history.txt",O_CREAT|O_WRONLY|O_APPEND,0666);
			write(fdhistory," ",1);
			write(fdhistory,flName,strlen(flName));
			write(fdhistory," ",1);
			write(fdhistory,flName2,strlen(flName2));
			write(fdhistory,"\n",1);
			close(fdhistory);
			
		}
		else
		{
			wait();
		}
 	
	}while(repeat!=0);								//to repeat the execution until the loop breaks		
	return 0;
}									


/********************for the prompt screen********************/
void prompt()
{
	printf("[minishell@localhost myhome]$");	
	return ;

}

/****mycat : opens , reads and prints the file content********/
void mycat(char flName[])
{
	char mycatbuf [1024]=" ";
	int fd=0;
	fd=open(flName,O_RDONLY);							
	read(fd,mycatbuf,1024);								//read()- read the content from argument-file
	printf(mycatbuf);
	close(fd);
	return;
}

/*******myhead : prints top 10 lines of the file**************/
void myhead(char flName[])
{
	char myheadbuf [1024]=" ";
	int fd,count=0;
	fd=open(flName,O_RDONLY);
	read(fd,myheadbuf,1024);
	char* lines=strtok(myheadbuf,"\n");
	while(count<10)
	{
		count++;
		printf(lines);
		lines=strtok(NULL,"\n");
		printf("\n");
	}
	close(fd);
return;
}

/************mytail : prints bottom 10 lines of file***********/
void mytail(char flName[])
{
	char mytailbuf [1024]=" ";
	char mytailbuf2 [1024]=" ";
	int fd1,fd2,count=0;
	fd1=open(flName,O_RDONLY);
	read(fd1,mytailbuf,1024);
	char* lines=strtok(mytailbuf,"\n");
	while(lines!=NULL)
	{
		count++;
		lines=strtok(NULL,"\n");
	}
	close(fd1);
	fd2=open(flName,O_RDONLY);
	read(fd2,mytailbuf2,1024);
	int i=1;
	read(fd2,mytailbuf2,1024);
	if(count<10)
	{
		printf(mytailbuf2);
		
	}
	else
	{	
		char *lines_1=strtok(mytailbuf2,"\n");
		while(lines_1!=NULL)
		{
			lines_1=strtok(NULL,"\n");
			i++;
			if(i>(count-10))
			{
				printf(lines_1);
				printf("\n");	
			}	
		}
	}
	return;
}

/*******************myrm : removes the file *******************/
void myrm(char flName[])
{
	if(unlink(flName)==-1)								//removes the file
	{
		
		printf("file doesn't exist..\n");
	}
	else
	{
		printf("file has been deleted...\n");	
	}
	return;
}

/****copies file A to file B and file A remains same***********/
void mycp(char flName[],char flName2[])							//takes 2 arguments (source and destination files)
{
	int fd_original,fd_copy=0;								
	static char mycpbuf[1024];
	fd_original=open(flName,O_RDONLY);
	fd_copy=open(flName2,O_WRONLY|O_APPEND);					//if the file has content already, it appends
	read(fd_original,mycpbuf,1024);
	write(fd_copy,mycpbuf,sizeof(mycpbuf));
	printf("%s has been copied to %s\n",flName,flName2);
	close(fd_original);
	close(fd_copy);
	return;
}

/**copies the contents to file B from file A and deletes file A**/
void mymv(char flName[],char flName2[])							//takes two arguments(source and destination files
{
	int fd_source,fd_dest=0;
	char mymvbuf [1024];
	fd_source=open(flName,O_RDONLY);
	fd_dest=open(flName2,O_WRONLY | O_APPEND);					//if file has content already, it appends
	read(fd_source,mymvbuf,1024);
	unlink(flName);									//deletes the source file
	write(fd_dest,mymvbuf,1024);	
	close(fd_source);
	close(fd_dest);
	return;

}

/********* myhistory :prints last 50 executed commands************/
void myhistory()
{	

	char myhistorybuf1 [1024]=" ";
	char myhistorybuf2 [1024]=" ";
	int fd1,fd2,count=0;
	fd1=open("history.txt",O_RDONLY);
	read(fd1,myhistorybuf1,1024);
	char* lines=strtok(myhistorybuf1,"\n");
	while(lines!=NULL)
	{
		count++;
		lines=strtok(NULL,"\n");
	}
	close(fd1);
	fd2=open("history.txt",O_RDONLY);
	read(fd2,myhistorybuf2,1024);
	int i=1;
	if(count<50)
	{
		printf(myhistorybuf2);
		
	}
	else
	{	
		char *lines_1=strtok(myhistorybuf2,"\n");
		while(lines_1!=NULL)
		{
			lines_1=strtok(NULL,"\n");
			i++;
			if(i>(count-50))
			{
				printf(lines_1);
				printf("\n");	
			}	
		}
	}
	return;

}


/*********************************************************************** END OF FILE ********************************************************************/
