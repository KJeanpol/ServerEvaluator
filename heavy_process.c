#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include<pthread.h>



#define PORT 8080
char path[150] = "/home/reiracm/Pictures/"; //cambiar
char client_message[2000];
char buffer[1024];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int name_img = 1;

void  socketThread(int newSocket, int env)
{

    int recv_size = 0, size = 0, read_size, write_size, packet_index = 1, stat, cyc = 0, aut=0, ite =0;
    char imagearray[10241];
    char message[9] = "Petición",num2[200];
    FILE *image;
    //send(newSocket,buffer,13,0);
    //Tamaño de la imagen
    write(newSocket, message, 13);
    read(newSocket, &size, sizeof(int));
    //printf("Tamaño de la imagen %d\n", size);
    //Cantidad de ciclos
    write(newSocket, message, 13);
    read(newSocket, &cyc, sizeof(int));
    //printf("Cantidad de ciclos %d\n", cyc);
    while(ite<cyc){
        write(newSocket, message, 13);
        read(newSocket, &aut, sizeof(int));
        //printf("Autorización%d %d\n",ite, aut);
        if (aut == -1)
        {
        sprintf(num2, "%s%d.jpg", path, name_img);
        name_img ++;
        image = fopen(num2, "w");
        if (image == NULL)
        {
            printf("No se pudo abrir la imagen\n");
            exit(1);
        }

        //Ciclo para recibir el archivo por paquetes
        struct timeval timeout = {10, 0};
        fd_set fds;
        int buffer_fd;

        while (recv_size < size)
        {
            FD_ZERO(&fds);
            FD_SET(newSocket, &fds);

            buffer_fd = select(FD_SETSIZE, &fds, NULL, NULL, &timeout);

            if (buffer_fd < 0)
            printf("Error\n");

            if (buffer_fd == 0)
            printf("Error\n");
            if (buffer_fd > 0)
            {
            do
            {
                read_size = read(newSocket, imagearray, 10241);
            } while (read_size < 0);

            //Se escriben los datos recibidos en una imagen
            write_size = fwrite(imagearray, 1, read_size, image);
            if (read_size != write_size)
            {
                printf("Error\n");
            }
            recv_size += read_size;
            }
        }
            recv_size = 0;
            //printf("¡Imagen recibida!\n");
            env++;
            fclose(image);
        }
        ite++;
    }

}


/*----------------------------------------------
---------- HEAVY PROCESS SERVER MAIN -----------
----------------------------------------------*/


int main(){

	int serverSocket, newSocket;
	struct sockaddr_in serverAddr;
	struct sockaddr_storage serverStorage;
	socklen_t addr_size;
  	pid_t pid[50];

  	//Create the socket.

  	serverSocket = socket(PF_INET, SOCK_STREAM, 0);

	// Configure settings of the server address struct

 	// Address family = Internet

 	serverAddr.sin_family = AF_INET;

 	//Set port number, using htons function to use proper byte order

 	serverAddr.sin_port = htons(7799);

    //Set IP address to localhost

    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

   	//Set all bits of the padding field to 0

    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

 	//Bind the address struct to the socket

  	bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

  	//Listen on the socket, with 40 max connection requests queued

 	if(listen(serverSocket,50)==0){

 		printf("Listening\n");

 	}

    else{

   		printf("Error\n");
   		pthread_t tid[60];
   		int i = 0;
    }


	while(1){

		/*---- Accept call creates a new socket for the incoming connection ----*/

        addr_size = sizeof serverStorage;

        newSocket = (serverSocket, (struct sockaddr *) &serverStorage, &addr_size);
        int pid_c = 0;


        if ((pid_c = fork())==0){

        	socketThread(newSocket,env);
        	env ++;
        	close(newSocket);
        }

        else{

        	pid[i++] = pid_c;

         	if( i >= 49){

         		i = 0;

             	while(i < 50){

             		waitpid(pid[i++], NULL, 0);

             	}

             i = 0;

            }

        }

    }

  	return 0;

}

