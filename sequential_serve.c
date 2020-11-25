//Servidor sequencial: servidor que acepta consultas de forma iterativa en un único proceso a modo de FIFO.
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<stdlib.h>
#include<netinet/in.h>
#include <fcntl.h> 
#include <unistd.h> 
#include <stdio.h>

#define PORT 8080
char path[150] = "/home/kimberly/Documentos/operativos/Proyecto2/img/"; //cambiar

int receive_image(int socket, int env);
int main()
{

 int sock;
 int newsock;
 int clientAddressLength;
 struct sockaddr_in serverAddress;
 struct sockaddr_in clientAddress;

 sock=socket(AF_INET,SOCK_STREAM,0);
 memset(&serverAddress,0,sizeof(serverAddress));
 serverAddress.sin_family=AF_INET;
 serverAddress.sin_addr.s_addr=htonl(INADDR_ANY);
 serverAddress.sin_port=htons(PORT);
 bind(sock,(struct sockaddr *)&serverAddress, sizeof(serverAddress));
 listen(sock,5);

 //Se acepta una conexión de la cola
 while(1)
 {
  printf("\n-------Esperando por nuevas conexiones--------\n");
  clientAddressLength=sizeof(clientAddress);
  if (newsock=accept(sock,(struct sockaddr*)&clientAddress,&clientAddressLength))
  {
    puts("Conexión aceptada");
  }

  if (newsock < 0)
  {
    perror("Falló la conexión");
    return 1;
  }
  int env = 0;
  int count = 0;
  int ciclos = 3;
    while(count < ciclos)
  {
    printf("Ejecutando ciclo %d", count);
    receive_image(newsock, env);
    fflush(stdout);
    env++;
    count ++;
  }
 close(newsock);
 }
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

