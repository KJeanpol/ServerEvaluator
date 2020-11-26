#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

//#define PORT 8080
//#define SERVER_IP "127.0.0.1"

//cliente < ip > < puerto > < imagen > < N − threads > < N − ciclos > //pendiente threads y ciclos
int send_image(int socket, const char *ruta, int cyc);
int main(int argc, char *argv[])
{
  if (argc != 6)
  {
    printf("ingrese los parámetros correctos");
    exit(1);
  }
  int sock;
  struct sockaddr_in serverAddress;

  sock = socket(AF_INET, SOCK_STREAM, 0);
  memset(&serverAddress, 0, sizeof(serverAddress));
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = inet_addr(argv[1]);
  serverAddress.sin_port = htons(atoi(argv[2]));
  connect(sock, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
  send_image(sock, argv[3], atoi(argv[5]));
  return 0;
}

int send_image(int socket, const char *ruta, int cyc)
{
  FILE *picture;
  int size, read_size, stat, packet_index;
  char send_buffer[10240], read_buffer[256], read_buffer2[256], read_buffer3[256];
  ;

  picture = fopen(ruta, "r");

  if (picture == NULL)
  {
    printf("Error al abrir archivo");
  }

  fseek(picture, 0, SEEK_END);
  size = ftell(picture);

  //Primero se envía el tamaño de la imagen
  int ciclos = 0, aut = -1;
  ;

  write(socket, (void *)&size, sizeof(int));
  do
  {
    stat = read(socket, &read_buffer, 255);
  } while (stat < 0);

  //Se envía la cantidad de ciclos
  write(socket, (void *)&cyc, sizeof(int));
  do
  {
    stat = read(socket, &read_buffer2, 255);
  } while (stat < 0);

  while (ciclos <= cyc)
  {
    //Se envía siguiente imagen
    write(socket, (void *)&aut, sizeof(int));
    do
    {
      stat = read(socket, &read_buffer3, 255);
    } while (stat < 0);
    //La imagen se envía por paquetes
    packet_index = 1;

    rewind(picture);
    //fseek(picture, 0, SEEK_SET);
    while (!feof(picture))
    {

      read_size = fread(send_buffer, 1, sizeof(send_buffer) - 1, picture);
      do
      {
        stat = write(socket, send_buffer, read_size);
      } while (stat < 0);
      packet_index++;
      //bzero(send_buffer, sizeof(send_buffer));
    }
    printf("Enviando...\n");
    ciclos++;
  }
  fclose(picture);
  close(socket);
  return 0;
}