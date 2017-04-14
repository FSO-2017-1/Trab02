#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>

int main(){
	pid_t child[2];
	int n = 0;

	for(n=0; n<2; n++){
		if(fork()==0){
			child[n] = getpid();
			printf("MEU NOME: %d\n", child[n]);
			printf("MEU PAI: %d\n", getppid());
			_exit(0);
		}
	}

	for(n=0; n<2; n++){
		wait(NULL);
	}

	
	printf("FILHO preguiçoso %d\n", child[0]);
	printf("FILHO trabalhador %d\n", child[1]);

	/*if(lazy_child == 0){
		printf("Eu sou o processo preguiçoso %d\n", getpid());
	}else{
		int work_child = fork();
		printf("Eu sou o processo pai %d\n", getpid());
	}*/

	return 0;
}

