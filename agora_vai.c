
#define _POSIX_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>

#define BUFFER 256


int main(){
	int lazy_child;
	int worker_child;
	char msg[BUFFER] ;
	int n = 0;
	int pipefd[4];
	pid_t child[2];

	if (pipe(pipefd)<0){
		perror("pipe");
		return -1;
	}

	//Criando processo filho
	lazy_child = fork();

	if (lazy_child == -1) {
		perror("fork");
		return -1;
	}

	if (lazy_child) {
		/*Filho preguiçoso escrevendo */
		/* Operação obrigatória de fechar o descritor*/
		close(pipefd[0]);

		char msg_time[10] = __TIME__;
		char *mensage = strcat(msg_time, ": Mensagem 1 do filho dorminhoco \n");
		printf("%s\n", msg_time);
		
		/*Escrever no pipe*/
		write(pipefd[1],mensage, strlen(mensage)+1);
		close(pipefd[1]);
	}else{
		FILE *output = fopen("output.txt", "wb");
		if(output == NULL){
			printf("Erro ao abrir o arquivo!\n");
			exit(1);
		}
		// Processo Pai
		/* Operação obrigatória de fechar o descritor*/
		close(pipefd[1]);



		/*Lê a mensagem do pipe que vem do filho preguiçoso*/
		read(pipefd[0],msg, sizeof msg);
		printf("A mensagem do filho preguiços: %s\n", msg);
		fprintf(output, "%s", msg);

		close(pipefd[0]);
		fclose(output);

	}
}