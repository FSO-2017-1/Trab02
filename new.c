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
#include <stdbool.h>
#define BUFFER 256

int alarm_stop = 0;
unsigned int alarm_period = 3;
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
	int pipefd[4];
	char msg[BUFFER];

	//Criando processo filho
	lazy_child = fork();

	//Teste do fork
	if (lazy_child == -1) {
		perror("fork");
		return -1;
	}

	//Teste do pipe
	if (pipe(pipefd)<0){
		perror("pipe");
		return -1;
	}

	if(lazy_child==0){
		printf("PID FILHO %d\n", getpid());
		signal(SIGALRM, lazy_alarm);
		alarm(1);
		//gambirra
		while(getppid() > 1){
			if(lazy_flag){
				printf("HELLOOOO \n");
				printf("PAPAI %d\n", getppid());
				lazy_flag = false;

				//char msg_time[10] = __TIME__;
				//char *mensage = strcat(msg_time, ": Mensagem 1 do filho dorminhoco \n");
				/* Operação obrigatória de fechar o descritor*/
				//close(pipefd[0]);
				/*Escrever no pipe*/
				//write(pipefd[1],mensage, strlen(mensage)+1);
				signal(SIGALRM, lazy_alarm);
				alarm(1);
			}
		}
		//kill(getpid(), SIGALRM);
		
	}else{
		printf("PAIZAO\n");
		signal(SIGALRM, on_alarm);
		alarm(alarm_period);
		//FILE *output = fopen("output.txt", "wb");
		for(;;){

			//close(pipefd[1]);

			/*Lê a mensagem do pipe que vem do filho preguiçoso*/
			//read(pipefd[0],msg, sizeof msg);
			//printf("A mensagem do filho preguiços: %s\n", msg);


			/*if(output == NULL){
				printf("Erro ao abrir o arquivo!\n");
				exit(1);
			}

			fprintf(output, msg);

			close(pipefd[0]);*/
		
		}
		//fclose(output);
		kill(getpid(), SIGKILL);
	}
	return 0;
}