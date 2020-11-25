#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h> 


#define PORT 8080
char path[150] = "/home/reiracm/Pictures/"; //cambiar

int receive_image(int socket, int env);

int main(){

	int sockfd, ret;
	struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", 8080);

	if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}


	while(1){

		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if((childpid = fork()) == 0){
			close(sockfd);

			while(1){
				//recv(newSocket, buffer, 1024, 0);
				if(strcmp(buffer, ":exit") == 0){
					printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
				}else{
					int env = 0;
					printf("Client: %s\n", buffer);
					receive_image(newSocket, env);
    				fflush(stdout);
					send(newSocket, buffer, strlen(buffer), 0);
					bzero(buffer, sizeof(buffer));
					childpid = 0;
				}
			}
		}

	}

	close(newSocket);


	return 0;
}

int receive_image(int socket, int env)
{
  int recv_size = 0, size = 0, read_size, write_size, packet_index = 1, stat;
  char imagearray[10241];
  FILE *image;

  //Se calcula el tamaño de la imagen
  do
  {
    stat = read(socket, &size, sizeof(int));
  } while (stat < 0);

  char buffer[] = "Got it";
  do
  {
    stat = write(socket, &buffer, sizeof(int));
  } while (stat < 0);

  char num2[200]; 
  sprintf(num2,"%s%d.jpg", path, env);
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
    FD_SET(socket, &fds);

    buffer_fd = select(FD_SETSIZE, &fds, NULL, NULL, &timeout);

    if (buffer_fd < 0)
      printf("Error\n");

    if (buffer_fd == 0)
      printf("Error\n");

    if (buffer_fd > 0)
    {
      do
      {
        read_size = read(socket, imagearray, 10241);
      } while (read_size < 0);

      //Se escriben los datos recibidos en una imagen
      write_size = fwrite(imagearray, 1, read_size, image);

      if (read_size != write_size)
      {
        printf("Error\n");
      }

      recv_size += read_size;
      packet_index++;
    }
  }

  fclose(image);
  printf("¡Imagen recibida!\n");
  return 1;
}