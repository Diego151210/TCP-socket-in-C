//Servidor
#include <stdio.h> /* for perror() */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

#define SUMA 1
#define RESTA 2
#define PRODUCTO 3
#define DIVISION 4

int calculator(int op1, int op2, int operator) {
	switch(operator) {
		case SUMA:
			return op1+op2;
		case RESTA:
			return op1 - op2;
		case PRODUCTO:
			return op1 * op2;
		case DIVISION:
			return (float)op1 / (float)op2;
		default:
			return -1;
	}
}



int main(){
 
	int sockfd, aceptar,addrlen,ret, nuevo_sockfd;
	pid_t cpid;

	struct sockaddr_in sin;
	struct sockaddr_in rem;
	
	struct header {
		unsigned char packetType;
		unsigned char id;
		unsigned char len;
	};
	  
	struct datos{
		unsigned int primer_termino;
		unsigned int segundo_termino;
		unsigned int operacion;
	};
	  
	struct pdu{
		struct header cab;	
		struct datos payload;	
	};
  
	
	struct pdu paquete;

	sockfd=socket(PF_INET,SOCK_STREAM,0);
	if(sockfd < 0) {
		 perror("Error creating the socket");
		 return -1;
	}

	sin.sin_addr.s_addr=htonl(INADDR_ANY); //similar a 0.0.0.0, cualquier interfaz de cualquier pc puede ser el servidor
	sin.sin_family = PF_INET;
	sin.sin_port = htons (5555); //puerto servidor
	
	ret = bind (sockfd, (struct sockaddr *)&sin, sizeof (sin)); //rellenamos el socket vacio con los datos especificados
	if(ret < 0) {
		 perror("Error binding the socket");
		 return -1;
	}

	listen(sockfd,2);
	addrlen = sizeof (struct sockaddr_in );

	for(;;) { //ciclo infinito que acepta y atiende peticiones
		nuevo_sockfd=accept(sockfd, (struct sockaddr *)&rem,&addrlen);  //nuevo_sockfd es la variable que usamos como nuevo socket para hacer la operacion, dejando sockfd libre para recibir mas peticiones
		cpid=fork();
		for(;!cpid;){
			recv(nuevo_sockfd, &paquete, sizeof(paquete),0);
			if(paquete.cab.packetType==0){
				printf("Cliente recibido \n");
				paquete.cab.packetType=99;
				send(nuevo_sockfd, &paquete, sizeof(paquete),0);
			}
			if(paquete.cab.packetType==1){
				printf("Primer operando: %d \n Segundo operando: %d \n Operacion: %d \n", paquete.payload.primer_termino, paquete.payload.segundo_termino, paquete.payload.operacion);
				paquete.payload.operacion=calculator						(paquete.payload.primer_termino,paquete.payload.segundo_termino,paquete.payload.operacion);
				printf("Solucion: %d\n", paquete.payload.operacion);
		    send(nuevo_sockfd, &paquete, sizeof(paquete),0);
			}
			if(paquete.cab.packetType==2) break;
		}
		close(nuevo_sockfd);
	}
}
