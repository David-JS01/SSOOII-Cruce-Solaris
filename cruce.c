#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "cruce.h"

pid_t crearHijo();
void perrorExit(char*mensaje, int retorno);
int semaforo;
int memoria;
int buzon;
struct mensaje {
	long tipo;
	int mensaje;
};
int numeroProcesos;
struct sigaction sigactionNueva, sigactionAntigua;
struct sigaction sigactionNuevaIg, sigactionAntiguaIg;
void sigInt(int signo);
int pidPadre;


union semun 
        {int             val;
         struct semid_ds *buf;
         ushort_t        *array;
         };

int main (int argc, char *argv[]){
char var[40];
int velocidad, pid, i, iteracion, var1;
int proceso;
int tipoCoche;
int cruzando=0;
int zonaPeligro;
char *variable;
long tipo2, tipo3, tipoAnterior;
long tipoRest, tipoGuardado;
union semun variableUnion;
struct posiciOn pos, pos2, posAnterior, posNac;
struct sembuf waitSEMpadre, signalSEMpadre;
struct sembuf waitSEM1, signalSEM1;
struct sembuf waitSEM2, signalSEM2;
struct sembuf waitSEMCruce, signalSEMCruce;
struct sembuf wait0SEMP1[2], wait0SEMP2[2];
struct sembuf waitSEMP1_Gestor_Sem, signalSEMP1_Gestor_Sem;
struct sembuf waitSEMP2_Gestor_Sem, signalSEMP2_Gestor_Sem;
struct sembuf wait0CocheAtr1, wait0CocheAtr2;
struct sembuf waitPeatonAtr1, signalPeatonAtr1;
struct sembuf waitPeatonAtr2, signalPeatonAtr2;
struct sembuf wait0PeatonAtr2;
struct sembuf wait0PeatonAtr1;
struct sembuf waitCocheAtr1, signalCocheAtr1;
struct sembuf waitCocheAtr2, signalCocheAtr2;
struct sembuf waitSEMPeaton, signalSEMPeaton;
struct sembuf waitSEMNacPeaton, signalSEMNacPeaton;
struct sembuf wait0CRUZANDO;
struct sembuf waitCRUZANDO;
struct mensaje mens;

 if (argc!=3){
 	perrorExit("Debe incluir dos argumentos la ejecucion del programa", 1);
 	return 1;
 }
 numeroProcesos=atoi(argv[1]);
 velocidad=atoi(argv[2]);
 
 if (numeroProcesos<=2){
 	perrorExit("Debe haber mas de dos procesos", 1);
 	return 2;
 }
 
 if (velocidad<0){
 	return 3;
 }
 
 
 
 pidPadre=getpid();
 if ((semaforo=semget(IPC_PRIVATE, 15, IPC_CREAT | 0600))==-1)
	perrorExit("semget crear", 4);
 variableUnion.val=numeroProcesos-1;
 if (semctl(semaforo, 1, SETVAL, &variableUnion)==-1)
  	perrorExit("semctl sem 1", 5);
 variableUnion.val=0; 
 if (semctl(semaforo, 2, SETVAL, &variableUnion)==-1)
  	perrorExit("semctl sem 2", 6);
 variableUnion.val=0;
 if (semctl(semaforo, 3, SETVAL, &variableUnion)==-1)
  	perrorExit("semctl sem 3", 7);
 variableUnion.val=1; 	
 if (semctl(semaforo, 4, SETVAL, &variableUnion)==-1)
  	perrorExit("semctl sem 4", 8);
 variableUnion.val=0; 
 if (semctl(semaforo, 5, SETVAL, &variableUnion)==-1)
  	perrorExit("semctl sem 5", 9);
 variableUnion.val=1; 	
 if (semctl(semaforo, 6, SETVAL, &variableUnion)==-1)
  	perrorExit("semctl sem 6", 10);
 variableUnion.val=0;
 if (semctl(semaforo, 7, SETVAL, &variableUnion)==-1)
  	perrorExit("semctl sem 7", 11);
 variableUnion.val=0; 	
 if (semctl(semaforo, 8, SETVAL, &variableUnion)==-1)
  	perrorExit("semctl sem 8", 12);
 variableUnion.val=0;
 if (semctl(semaforo, 9, SETVAL, &variableUnion)==-1)
  	perrorExit("semctl sem 9", 13);
 variableUnion.val=1;
 if (semctl(semaforo, 10, SETVAL, &variableUnion)==-1)
  	perrorExit("semctl sem 10", 14);
 variableUnion.val=1;
 if (semctl(semaforo, 11, SETVAL, &variableUnion)==-1)
  	perrorExit("semctl sem 11", 15); 	
 variableUnion.val=1;
 if (semctl(semaforo, 12, SETVAL, &variableUnion)==-1)
  	perrorExit("semctl sem 12", 16);
 variableUnion.val=0;
 if (semctl(semaforo, 13, SETVAL, &variableUnion)==-1)
  	perrorExit("semctl sem 13", 17);
 variableUnion.val=0;
 if (semctl(semaforo, 14, SETVAL, &variableUnion)==-1)
  	perrorExit("semctl sem 14", 18); 	 
 if ((memoria=shmget(IPC_PRIVATE, 256*sizeof(char), IPC_CREAT|0600))==-1)
	perrorExit("shmget", 19);
	
 if ((variable=shmat(memoria, NULL, 0))==NULL)
		perrorExit("shmat", 20);
		
 buzon=msgget(IPC_PRIVATE, IPC_CREAT |0600);
 if (buzon==-1)
 		perrorExit("msgget IPC_CREAT", 21);

 
 for (i=1; i<=57; i++){
 	if (i!=47){
 		mens.tipo=i;
  		mens.mensaje=10;
  		if (msgsnd(buzon, &mens, sizeof(struct mensaje)-sizeof(long), 0)==-1)
  				perrorExit("msgsnd padre", 22);
  	}
 }


 
 var1=0;
for (i=100; i<=1750; i++){
	if (var1==51){
		i=i+49;
		var1=0;
	}
	mens.tipo=i;
	if (msgsnd(buzon, &mens, sizeof(struct mensaje)-sizeof(long), 0)==-1)
  		perrorExit("msgsnd padre", 23);
	var1++;	
}



 waitSEMpadre.sem_num=1;
 waitSEMpadre.sem_op=-1;
 waitSEMpadre.sem_flg=0;
  
 signalSEMpadre.sem_num=1;
 signalSEMpadre.sem_op=1;
 signalSEMpadre.sem_flg=0;
 
 waitSEM1.sem_num=2;
 waitSEM1.sem_op=-1;
 waitSEM1.sem_flg=0;
  
 signalSEM1.sem_num=2;
 signalSEM1.sem_op=1;
 signalSEM1.sem_flg=0;
 
 waitSEM2.sem_num=3;
 waitSEM2.sem_op=-1;
 waitSEM2.sem_flg=0;
  
 signalSEM2.sem_num=3;
 signalSEM2.sem_op=1;
 signalSEM2.sem_flg=0;
 
 waitSEMCruce.sem_num=4;
 waitSEMCruce.sem_op=-1;
 waitSEMCruce.sem_flg=0;
  
 signalSEMCruce.sem_num=4;
 signalSEMCruce.sem_op=1;
 signalSEMCruce.sem_flg=0;
 
 
 
 wait0SEMP1[0].sem_num=5;
 wait0SEMP1[0].sem_op=0;  
 wait0SEMP1[0].sem_flg=0;
 
 wait0SEMP1[1].sem_num=14;
 wait0SEMP1[1].sem_op=1;  
 wait0SEMP1[1].sem_flg=0;

 
 wait0SEMP2[0].sem_num=6;
 wait0SEMP2[0].sem_op=0;  
 wait0SEMP2[0].sem_flg=0;
 
 wait0SEMP2[1].sem_num=14;
 wait0SEMP2[1].sem_op=1;  
 wait0SEMP2[1].sem_flg=0;

 
 waitSEMP1_Gestor_Sem.sem_num=5;
 waitSEMP1_Gestor_Sem.sem_op=-1;  
 waitSEMP1_Gestor_Sem.sem_flg=0; 
  
 signalSEMP1_Gestor_Sem.sem_num=5;
 signalSEMP1_Gestor_Sem.sem_op=1;
 signalSEMP1_Gestor_Sem.sem_flg=0;
 
 
 waitSEMP2_Gestor_Sem.sem_num=6;
 waitSEMP2_Gestor_Sem.sem_op=-1;  
 waitSEMP2_Gestor_Sem.sem_flg=0;
  
 signalSEMP2_Gestor_Sem.sem_num=6;
 signalSEMP2_Gestor_Sem.sem_op=1;
 signalSEMP2_Gestor_Sem.sem_flg=0;
 

 wait0CocheAtr1.sem_num=7;
 wait0CocheAtr1.sem_op=0;
 wait0CocheAtr1.sem_flg=0; 
 
 wait0CocheAtr2.sem_num=8;
 wait0CocheAtr2.sem_op=0;
 wait0CocheAtr2.sem_flg=0; 
  
 waitPeatonAtr1.sem_num=7;
 waitPeatonAtr1.sem_op=-1;
 waitPeatonAtr1.sem_flg=0; 
 
 signalPeatonAtr1.sem_num=7;
 signalPeatonAtr1.sem_op=1;
 signalPeatonAtr1.sem_flg=0;
 
 waitPeatonAtr2.sem_num=8;
 waitPeatonAtr2.sem_op=-1;
 waitPeatonAtr2.sem_flg=0; 
 
 signalPeatonAtr2.sem_num=8;
 signalPeatonAtr2.sem_op=1;
 signalPeatonAtr2.sem_flg=0;
 
 wait0PeatonAtr2.sem_num=9;
 wait0PeatonAtr2.sem_op=0;
 wait0PeatonAtr2.sem_flg=0;
 
 waitCocheAtr2.sem_num=9;
 waitCocheAtr2.sem_op=-1;
 waitCocheAtr2.sem_flg=0; 
 
 signalCocheAtr2.sem_num=9;
 signalCocheAtr2.sem_op=1;
 signalCocheAtr2.sem_flg=0;
 
 wait0PeatonAtr1.sem_num=13;
 wait0PeatonAtr1.sem_op=0;
 wait0PeatonAtr1.sem_flg=0;
 
 waitCocheAtr1.sem_num=13;
 waitCocheAtr1.sem_op=-1;
 waitCocheAtr1.sem_flg=0; 
 
 signalCocheAtr1.sem_num=13;
 signalCocheAtr1.sem_op=1;
 signalCocheAtr1.sem_flg=0;

 
 
 
 waitSEMPeaton.sem_num=10;
 waitSEMPeaton.sem_op=-1;
 waitSEMPeaton.sem_flg=0;
  
 signalSEMPeaton.sem_num=10;
 signalSEMPeaton.sem_op=1;
 signalSEMPeaton.sem_flg=0;
 
 waitSEMNacPeaton.sem_num=11;
 waitSEMNacPeaton.sem_op=-1;
 waitSEMNacPeaton.sem_flg=0;
  
 signalSEMNacPeaton.sem_num=11;
 signalSEMNacPeaton.sem_op=1;
 signalSEMNacPeaton.sem_flg=0;
 
 wait0CRUZANDO.sem_num=14;
 wait0CRUZANDO.sem_op=0;
 wait0CRUZANDO.sem_flg=0;
 
 waitCRUZANDO.sem_num=14;
 waitCRUZANDO.sem_op=-1;
 waitCRUZANDO.sem_flg=0;
 
 
  CRUCE_inicio(velocidad, numeroProcesos, semaforo, variable);
  if (semop(semaforo, &waitSEMpadre, 1)==-1) 
  		perrorExit("semop wait padre 1", 24);
  pid=crearHijo();
  if (pid==0){
  	while(1){
  	
  		CRUCE_pon_semAforo(SEM_C1, VERDE);
  		if (semop(semaforo, &signalSEM1, 1)==-1)
  			perrorExit("semop signal", 25);
  		CRUCE_pon_semAforo(SEM_C2, ROJO);
  		if (semop(semaforo, &signalSEMP1_Gestor_Sem, 1)==-1)
  			perrorExit("semop signal", 26);
  		if (semop(semaforo, &wait0CRUZANDO, 1)==-1)
  			perrorExit("semop wait", 27);
  		CRUCE_pon_semAforo(SEM_P1, ROJO);
  		CRUCE_pon_semAforo(SEM_P2, VERDE);
  		if (semop(semaforo, &waitSEMP2_Gestor_Sem, 1)==-1)
  					perrorExit("semop wait", 28);
  		for (i=0; i<6; i++){
  			pausa();
  			if (i==4){
  				if (semop(semaforo, &waitSEM1, 1)==-1)
  					perrorExit("semop wait", 29);
  				CRUCE_pon_semAforo(SEM_C1, AMARILLO);
  			}
  		}
  		
  		CRUCE_pon_semAforo(SEM_C1, ROJO);
  		CRUCE_pon_semAforo(SEM_C2, VERDE);
  		if (semop(semaforo, &signalSEM2, 1)==-1)
  			perrorExit("semop signal", 30);
  		CRUCE_pon_semAforo(SEM_P1, ROJO);
  		if (semop(semaforo, &signalSEMP2_Gestor_Sem, 1)==-1)
  			perrorExit("semop signal", 31);
  		if (semop(semaforo, &wait0CRUZANDO, 1)==-1)
  			perrorExit("semop wait", 32);
  		CRUCE_pon_semAforo(SEM_P2, ROJO);
  		for (i=0; i<9; i++){
  			pausa();
  			if (i==7){
  			if (semop(semaforo, &waitSEM2, 1)==-1)
  					perrorExit("semop wait", 33);
  			CRUCE_pon_semAforo(SEM_C2, AMARILLO);
  			}
  		}
  		
  		if (semop(semaforo, &waitSEMCruce, 1)==-1)
  			perrorExit("semop wait padre 2", 34);
  		if (semop(semaforo, &signalSEMCruce, 1)==-1)
 	 				perrorExit("semop signal padre 1", 35);
  		
		CRUCE_pon_semAforo(SEM_C1, ROJO);
  		CRUCE_pon_semAforo(SEM_C2, ROJO);
  		CRUCE_pon_semAforo(SEM_P1, VERDE);
  		if (semop(semaforo, &waitSEMP1_Gestor_Sem, 1)==-1)
  					perrorExit("semop wait", 36);
  		CRUCE_pon_semAforo(SEM_P2, ROJO);
  		for (i=0; i<12; i++)
  			pausa();
  			
  	}
  	exit(0);
  }
  if (getpid()==pidPadre){
  
  	sigactionNueva.sa_handler=sigInt;
  	if (sigemptyset(&sigactionNueva.sa_mask)==-1)
  		perrorExit("sigaction", 37);
  	sigactionNueva.sa_flags=0;
  	if (sigaction(SIGINT, &sigactionNueva, &sigactionAntigua)==-1)
  		perrorExit("sigaction", 38);
  		
  	sigactionNuevaIg.sa_handler=SIG_IGN;
  	if (sigemptyset(&sigactionNuevaIg.sa_mask)==-1)
  		perrorExit("sigaction", 39);
  	sigactionNuevaIg.sa_flags=0;
  	if (sigaction(SIGCHLD, &sigactionNuevaIg, &sigactionAntiguaIg)==-1)
  		perrorExit("sigaction", 40);
  
  while(1){
  		proceso=CRUCE_nuevo_proceso();
  	if (semop(semaforo, &waitSEMpadre, 1)==-1) 
  		perrorExit("semop wait padre 3", 41);
  	pid=crearHijo();
  	if (pid==0)
  		break;
  }
  }
 
  
  
  if (pid==0){
  if (sigaction(SIGINT, &sigactionAntigua, NULL)==-1)
 	perrorExit("sigaction", 42);
  if (proceso==COCHE){
  	while (1){
 	 	pos=CRUCE_inicio_coche();
 	 	if (pos.x==-3)
 	 		tipoCoche=0;
 	 	else if (pos.y==1)
 	 		tipoCoche=1;
 	 	while (pos.y>=0){
 	 		if (pos.y==10)
 				tipo2=(long)pos.x+4;
 			else if (pos.x==33)
 				tipo2=(long)pos.y+37;
  			
 			if (posAnterior.x==33 && tipo2==37){
 				tipo2=47;
 			}
 			
 			tipoGuardado=tipo2;	
 			if(tipo2==1){
 				if (msgrcv(buzon, &mens, sizeof(struct mensaje)-sizeof(long), tipo2, 0)==-1)
  				perrorExit("msgrcv", 43);
  			}
 			else if (tipo2<38){
 				
 				if (msgrcv(buzon, &mens, sizeof(struct mensaje)-sizeof(long), tipo2, 0)==-1)
  				perrorExit("msgrcv", 44);
  				tipo2--;
  				if (msgrcv(buzon, &mens, sizeof(struct mensaje)-sizeof(long), tipo2, 0)==-1)
  				perrorExit("msgrcv", 45);
  				
  			}
  			
  						
  			if (tipo2>=38){
  				tipo3=tipo2;
  				if (tipo2==47)
  					tipo3=37;
  				if (tipo2==49 && posAnterior.y==10){
  					tipo3=46;
  					if (msgrcv(buzon, &mens, sizeof(struct mensaje)-sizeof(long), tipo3, 0)==-1)
  						perrorExit("msgrcv", 46);
  					tipo3=48;
  					if (msgrcv(buzon, &mens, sizeof(struct mensaje)-sizeof(long), tipo3, 0)==-1)
  						perrorExit("msgrcv", 47);
  					tipo3=tipo2;
  				}
  				if (msgrcv(buzon, &mens, sizeof(struct mensaje)-sizeof(long), tipo3, 0)==-1)
  					perrorExit("msgrcv", 48);
  				
  			}
  			
  			tipo2=tipoGuardado;
  			
  			if (tipo2==45 || tipo2==31){
  				if (semop(semaforo, &waitSEMCruce, 1)==-1)
  					perrorExit("semop wait", 49);	
  			}
  			
  			if (tipo2==19 || tipo2==20){
  				if (semop(semaforo, &wait0CocheAtr2, 1)==-1)
  					perrorExit("semop wait", 50);
  				if (semop(semaforo, &signalCocheAtr2, 1)==-1)
  					perrorExit("semop wait", 51);
  			}
  			
  			if (tipo2==50){
  				if (semop(semaforo, &wait0CocheAtr1, 1)==-1)
  					perrorExit("semop wait", 52);
  				if (semop(semaforo, &signalCocheAtr1, 1)==-1)
  					perrorExit("semop signal", 53);
  			}
  			
  			
  			if (pos.x==33 && pos.y==6){
  				if (semop(semaforo, &waitSEM1, 1)==-1)
  					perrorExit("semop wait", 54);
  			}
  			if (pos.x==13 && pos.y==10 ){
  				if (semop(semaforo, &waitSEM2, 1)==-1)
  					perrorExit("semop wait", 55);
  			}
 	 		pos2=CRUCE_avanzar_coche(pos);
 	 		if (pos.x==33 && pos.y==6){
  				if (semop(semaforo, &signalSEM1, 1)==-1)
  					perrorExit("semop signal", 56);
  			}
  			if (pos.x==13 && pos.y==10 ){
  				if (semop(semaforo, &signalSEM2, 1)==-1)
  					perrorExit("semop signal", 57);
  			}
 	 		pausa_coche();
 	 		
 	 		
 	 		
 	 		if (tipo2==37 && tipoCoche==0){
 	 			if (semop(semaforo, &waitCocheAtr2, 1)==-1)
  					perrorExit("semop wait", 58);
 	 		}
 	 		
 	 		if (tipo2==57){
 	 			if (semop(semaforo, &waitCocheAtr1, 1)==-1)
  					perrorExit("semop wait", 59);
 	 		}
 			
 			if (tipo2==53){
 				if (semop(semaforo, &signalSEMCruce, 1)==-1)
 	 				perrorExit("semop signal", 60);	
 			}
 			
 			
 			if (tipo2>7 && tipo2<38){
 	 			mens.tipo=tipo2-7;
 	 			if (msgsnd(buzon, &mens, sizeof(struct mensaje)-sizeof(long), 0)==-1)
  					perrorExit("msgsnd hijo", 61);
  				mens.tipo=tipo2-8;
  				if (msgsnd(buzon, &mens, sizeof(struct mensaje)-sizeof(long), 0)==-1)
  					perrorExit("msgsnd hijo", 62);
 	 		}
 	 		
 	 		
 	 		
 	 		if (tipo2>41){
 	 			if (posAnterior.y==10 && tipoCoche==0){
 	 				for (i=31; i<=36; i++){
 	 					mens.tipo=i;
 	 					if (msgsnd(buzon, &mens, sizeof(struct mensaje)-sizeof(long), 0)==-1)
  							perrorExit("msgsnd hijo", 63);
 	 				}
 	 			}else{
 	 				mens.tipo=tipo2-4;
 	 				if (tipo2==51){
 	 					mens.tipo=37;
 	 				}
 	 				if (msgsnd(buzon, &mens, sizeof(struct mensaje)-sizeof(long), 0)==-1)
  						perrorExit("msgsnd hijo", 64);
  				}	
  			}
  			
  			
  			
 			posAnterior=pos;
 			pos=pos2;
 			
}
 	 	CRUCE_fin_coche();
 	 	for (i=54;i<=57;i++){
 	 		mens.tipo=i;
 	 		if (msgsnd(buzon, &mens, sizeof(struct mensaje)-sizeof(long), 0)==-1)
  				perrorExit("msgsnd hijo", 65);
  		}
 	 	if (semop(semaforo, &signalSEMpadre, 1)==-1)
 	 		perrorExit("semop signal", 66);
 	 
 	 	return 0;
 	 	break;
 	 
  	}
  }
  if (proceso==PEAToN){
  	while (1){
  		iteracion=0;
  		if (semop(semaforo, &waitSEMPeaton, 1)==-1)
  					perrorExit("semop wait", 67);
  		if (semop(semaforo, &waitSEMNacPeaton, 1)==-1) 
  					perrorExit("semop wait", 68);
  		zonaPeligro=1;			
  		pos2=CRUCE_inicio_peatOn_ext(&posNac);
  		tipo2=(posNac.y+1)*100+posNac.x;
  		if (msgrcv(buzon, &mens, sizeof(struct mensaje)-sizeof(long), tipo2, 0)==-1)
  				perrorExit("msgrcv", 69);
  		if (semop(semaforo, &signalSEMPeaton, 1)==-1)
 	 		perrorExit("semop signal", 70);
 	 	tipoAnterior=tipo2;
 	 	pos=pos2;
  		while (pos.y>=0){
  			tipo2=(pos.y+1)*100+pos.x;
  			if (msgrcv(buzon, &mens, sizeof(struct mensaje)-sizeof(long), tipo2, 0)==-1)
  				perrorExit("msgrcv", 71);
  				
			 		
  			if (tipo2<=1127 && tipo2>=1120){
  				if (semop(semaforo, &signalPeatonAtr2, 1)==-1)
  					perrorExit("semop signal", 72);
  				if (semop(semaforo, &wait0PeatonAtr2, 1)==-1)
  					perrorExit("semop wait", 73);
  			}
  			if (tipo2==1431 || tipo2==1531 || tipo2==1631){
  				if (semop(semaforo, &wait0PeatonAtr1, 1)==-1)
  					perrorExit("semop wait", 74);
  			}
  			
  			
  			if (tipo2==1430 || tipo2==1530 || tipo2==1630){
  				if (semop(semaforo, wait0SEMP1, 2)==-1)
  					perrorExit("semop wait", 75);
  				cruzando=1;
  				if (semop(semaforo, &signalPeatonAtr1, 1)==-1)
  					perrorExit("semop signal", 76);
  			}
  			
  			if (tipo2<=1227 && tipo2>=1220){
  				if (semop(semaforo, wait0SEMP2, 2)==-1)
  					perrorExit("semop wait", 77);
  				
  				cruzando=1;
  			}
  			
  			pos=CRUCE_avanzar_peatOn(pos);
  			pausa();
  			
  			if (cruzando && (tipo2==1434 || tipo2==1534 || tipo2==1634) ){
  				if (semop(semaforo, &waitCRUZANDO, 1)==-1)
  					perrorExit("semop wait", 78);
  				cruzando=0;
  			}
  			
  			if (cruzando && (tipo2>=920 && tipo2<=927) ){
  				if (semop(semaforo, &waitCRUZANDO, 1)==-1)
  					perrorExit("semop wait", 79);
  				cruzando=0;
  			}
  			
  			if ( (tipo2<=1700 || tipo2>=1729) && tipo2!=1300 && tipo2!=1400 && tipo2!=1500 && tipo2!=1600 && zonaPeligro){
 				if (semop(semaforo, &signalSEMNacPeaton, 1)==-1) 
  					perrorExit("semop signal", 80);
  				zonaPeligro=0;
 			}	
  			
  			if (tipo2>=820 && tipo2<=827){
  				if (semop(semaforo, &waitPeatonAtr2, 1)==-1)
  					perrorExit("semop wait", 81);
  			}
  			
  			if (tipo2==1440 || tipo2==1540 || tipo2==1640){
  				if (semop(semaforo, &waitPeatonAtr1, 1)==-1)
  					perrorExit("semop wait", 82);
  			}
  			
  			
  			
  				mens.tipo=tipoAnterior;
  				if (msgsnd(buzon, &mens, sizeof(struct mensaje)-sizeof(long), 0)==-1)
  					perrorExit("msgsnd hijo", 83);
  			
  			tipoAnterior=tipo2;
  			
  			iteracion++;
  		}
  		CRUCE_fin_peatOn();
  		mens.tipo=tipoAnterior;
  		if (msgsnd(buzon, &mens, sizeof(struct mensaje)-sizeof(long), 0)==-1)
  			perrorExit("msgsnd hijo", 84);
  			
  		if (semop(semaforo, &signalSEMpadre, 1)==-1)
 	 		perrorExit("semop signal", 85);
 	 		
 	 	return 0;
  	}
  }
}
  
  return 0;

}



void sigInt(int signo){
 int retorno, i;
 system("clear");
 CRUCE_fin();
 system("clear");
 printf("\nEl programa ha finalizado correctamente!\n");
 		
 for (i=0; i<numeroProcesos; i++)
 	wait(&retorno);
 	
 if (semctl(semaforo, 0, IPC_RMID)==-1)
 	perrorExit("semctl rmid", 86);
 
 
 if (shmctl(memoria,IPC_RMID, NULL)==-1)
		perrorExit("shmctl", 87);

 if (msgctl(buzon, IPC_RMID, 0)==-1)
 	perrorExit("msgctl rmid", 88);
 
 if (sigaction(SIGINT, &sigactionAntigua, NULL)==-1)
 	perrorExit("sigaction", 89);
 	
 exit(0);

} 

void perrorExit(char*mensaje, int retorno)
{
	perror(mensaje);
	exit(retorno);
}
 
pid_t crearHijo(){

	pid_t pid=fork();
	
	switch(pid)
	{
		case-1:
			perrorExit("fork", 90);
			break;
	}
	return pid;
}

