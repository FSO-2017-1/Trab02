
#define _POSIX_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <stdbool.h>

#define BUFFER 256


int alarm_stop = 0;
unsigned int alarm_period = 2;
volatile sig_atomic_t lazy_flag = false;


void on_alarm(int signal){
	if(alarm_stop) return;
	else alarm(alarm_period);
}

void lazy_alarm(int sig){
	lazy_flag = true;
	
}


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
		signal(SIGALRM, lazy_alarm);
		alarm(1);

		while(getppid() > 1){
			if(lazy_flag){
				lazy_flag = false;
				/*Filho preguiçoso escrevendo */
				/* Operação obrigatória de fechar o descritor*/
				close(pipefd[0]);

				char msg_time[10] = __TIME__;
				char *mensage = strcat(msg_time, ": Mensagem 1 do filho dorminhoco \n");
				printf("%s\n", msg_time);
				
				/*Escrever no pipe*/
				write(pipefd[1],mensage, strlen(mensage)+1);
				close(pipefd[1]);
				signal(SIGALRM, lazy_alarm);
				alarm(1);
			}
		}
	}else{
		signal(SIGALRM, on_alarm);
		alarm(alarm_period);

		FILE *output = fopen("output.txt", "wb");
		if(output == NULL){
			printf("Erro ao abrir o arquivo!\n");
			exit(1);
		}
		// Processo Pai
		/* Operação obrigatória de fechar o descritor*/
		
		for(;;){
			close(pipefd[1]);
			/*Lê a mensagem do pipe que vem do filho preguiçoso*/
			read(pipefd[0],msg, sizeof msg);
			//printf("A mensagem do filho preguiços: %s\n", msg);
			fprintf(output, "%s", msg);
			
		}
		close(pipefd[0]);
		fclose(output);

		kill(lazy_child, SIGKILL);

	}
	return 0;
}