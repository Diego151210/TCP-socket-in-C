//Cliente
#include <stdio.h> /* for perror() */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(){
  struct header {   //estructura de cabecera
    unsigned char packetType;
    unsigned char id;
    unsigned char len;
  };

  struct datos{                     //estructura de datos
      unsigned int primer_termino;
      unsigned int segundo_termino;
      unsigned int operador;
  };

  struct pdu{              //estructura de los paquetes de datos que se envian, esta formado por la estructura de la cabecera y la de los datos
   struct header cab;
   struct datos payload;
  };

  struct pdu paquete;
	int sockfd, addrlen,ret;
	struct sockaddr_in sin;
	struct sockaddr_in rem;
	char  dirserv[16]; //para guardar la direccion del servidor en caso de que la pidas tu

	sockfd=socket(PF_INET,SOCK_STREAM,0);
	if(sockfd < 0){
		 perror("Error creating the socket");
		 return -1;
	}

	rem.sin_family = PF_INET;
	rem.sin_addr.s_addr=inet_addr("127.0.0.1");  //interfaz de loopback para hacer pruebas,de forma normal tendriamos que pedirle al cliente la IP del servidor con el que se quiere conectar
	//printf("Cual es la direccion IP del servidor?\n");
	//scanf("%s", dirserv);
	rem.sin_port = htons (5555); //puerto del servidor

	addrlen = sizeof (struct sockaddr );

	connect (sockfd, (struct sockaddr *)&rem, addrlen);

        printf("Estableciendo conexion \n");
        paquete.cab.packetType=0;
        send(sockfd,&paquete,sizeof(paquete),0);
        recv(sockfd,&paquete,sizeof(paquete),0);
        if(paquete.cab.packetType==99) {
		printf("Conexion establecida \n");
		paquete.cab.packetType=1;
	}
        else printf("Error en la conexion \n");

	char maintain_conex='s';

	do{
		printf("Introduce operando 1, operando 2 y operador: ");
		scanf(" %d %d %d",&paquete.payload.primer_termino,&paquete.payload.segundo_termino,&paquete.payload.operador); 
		printf("eco \n");
		send(sockfd,&paquete,sizeof(paquete),0);
		recv(sockfd,&paquete,sizeof(paquete),0);
		printf("El resultado es %d\n",paquete.payload.operador);

		printf("Quiere realizar otra operacion? (s/n): ");
		scanf(" %c", &maintain_conex);

		}while (maintain_conex=='s' || maintain_conex=='S');
	paquete.cab.packetType=2;
	send(sockfd,&paquete,sizeof(paquete),0);
	printf("Cerrando conexion \n");
	close(sockfd);

	return 0;
}

