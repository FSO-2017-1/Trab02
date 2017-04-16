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
		char *mensage = "Olá";
		/* Operação obrigatória de fechar o descritor*/
		close(pipefd[0]);
		/*Escrever no pipe*/
		write(pipefd[1],mensage, strlen(mensage)+1);
		close(pipefd[1]);
	}else{
		// Processo Pai
		/* Operação obrigatória de fechar o descritor*/
		close(pipefd[1]);

		/*Lê a mensagem do pipe que vem do filho preguiçoso*/
		read(pipefd[0],msg, sizeof msg);
		printf("A mensagem do filho preguiços: %s\n", msg);
		close(pipefd[0]);

	}





	// for(n=0; n<2; n++){
	// 	if(fork()==0){
	// 		child[n] = fork();
	// 		printf("MEU NOME: %d\n", child[n]);
	// 		printf("MEU PAI: %d\n", getppid());
	// 		_exit(0);
	// 	}
	// }
	//
	//
	// for(n=0; n<2; n++){
	// 	wait(NULL);
	// }
	//
	//
	// printf("FILHO preguiçoso %d\n", child[0]);
	// printf("FILHO trabalhador %d\n", child[1]);

	/*if(lazy_child == 0){
		printf("Eu sou o processo preguiçoso %d\n", getpid());
	}else{
		int work_child = fork();
		printf("Eu sou o processo pai %d\n", getpid());
	}*/

	return 0;
}
