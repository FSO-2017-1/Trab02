#define _POSIX_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>

#define BUFFER 256

/*int alarm_stop = 0;
unsigned int alarm_period = 30;

void on_alarm(int signal){
	if(alarm_stop) return;
	else alarm(alarm_period);
	printf("HM É MESMO\n");
}*/

int main(){
	int lazy_child;
	int worker_child;
	char msg[BUFFER], msg_usr[BUFFER];
	int n = 0;
	int pipefd[4];
	pid_t child[2];
	int random = rand() % 3;

	printf("DATA : %s HORA: %s\n",__DATE__,__TIME__);

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
		sleep(random);
		char msg_time[10] = __TIME__;
		char *mensage = strcat(msg_time, ": Mensagem 1 do filho dorminhoco \n");
	
		/* Operação obrigatória de fechar o descritor*/
		close(pipefd[0]);

		/*Escrever no pipe*/
		write(pipefd[1],mensage, strlen(mensage)+1);

		//printf("Informe a mensagem:\n");
		//scanf("%s",msg_usr);
		//close(pipefd[1]);

		//printf("%s\n",msg_usr );
		//sleep(5);
		//printf("DOIDO\n");
	}else{
		// Processo Pai
		/* Operação obrigatória de fechar o descritor*/
		signal(SIGALRM, on_alarm);
		alarm(alarm_period);
		close(pipefd[1]);

		/*Lê a mensagem do pipe que vem do filho preguiçoso*/
		read(pipefd[0],msg, sizeof msg);
		printf("A mensagem do filho preguiçoso: %s\n", msg);

		//Escrita do output file
		FILE *output = fopen("output.txt", "wb");
		if(output == NULL){
			printf("Erro ao abrir o arquivo!\n");
			exit(1);
		}

		fprintf(output, msg);

		close(pipefd[0]);
		fclose(output);

		printf("Processo será assassinado\n");
		kill(getpid(), SIGKILL);
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

