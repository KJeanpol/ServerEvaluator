#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <unistd.h> 
#include<pthread.h>

char *ip, *img;
int port,cyc, thr;
void * cientThread(void *arg)
{
  //printf("Inicia thread\n");
  char message[1000];
  char buffer[1024];
  int clientSocket;
  struct sockaddr_in serverAddr;
  socklen_t addr_size;

  clientSocket = socket(PF_INET, SOCK_STREAM, 0);
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(port);
  serverAddr.sin_addr.s_addr = inet_addr(ip);
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
  addr_size = sizeof serverAddr;
  connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

    FILE *picture;
    int size, read_size, stat, packet_index, ciclos=0;
    char send_buffer[10240], read_buffer[256], read_buffer2[256];
    picture = fopen(img, "r");

    if (picture == NULL)
    {
        printf("Error al abrir archivo");
    }

    fseek(picture, 0, SEEK_END);
    size = ftell(picture);
    fclose(picture);
    //printf("El tamaño de la imagen es %d\n", size);
    //Enviar tamaño de imagen
    if(read(clientSocket, buffer, 1024)  < 0)
    {
        printf("Fallo al recibir\n");
    }
    if( send(clientSocket , (void*)&size, sizeof(int),0) < 0)
    {
        printf("Fallo al enviar\n");
    }
    //printf("P1: %s\n",buffer);
    //Enviar cantidad de ciclos
    if(read(clientSocket, buffer, 1024)  < 0)
    {
        printf("Fallo al recibir\n");
    }
    if( send(clientSocket , (void*)&cyc, sizeof(int),0) < 0)
    {
        printf("Fallo al enviar\n");
    }
    //printf("P2: %s\n",buffer);
    //Enviar imágenes
    while (ciclos < cyc)
  {
    FILE *picture;
    int size, read_size, packet_index;
    char send_buffer[10240], read_buffer[256], read_buffer2[256];
    picture = fopen(img, "r");

    if (picture == NULL)
    {
    printf("Error al abrir archivo");
    }
    int stat, aut=-1;
    char read_buffer3[256];
    //Se envía autorización

    if(read(clientSocket, buffer, 1024)  < 0)
    {
    printf("Fallo al recibir\n");
    }
    if( send(clientSocket , (void*)&aut, sizeof(int),0) < 0)
    {
        printf("Fallo al enviar\n");

    }
    //printf("P3: %s\n",buffer);

    //La imagen se envía por paquetes
    while (!feof(picture))
    {
        read_size = fread(send_buffer, 1, sizeof(send_buffer) - 1, picture);
        do
        {
        stat = write(clientSocket, send_buffer, read_size);
        } while (stat < 0);
        bzero(send_buffer, sizeof(send_buffer));
    }
    //printf("Enviando...\n");
    fclose(picture);
    ciclos++;
  }
    //Finalizar
    close(clientSocket);
    pthread_exit(NULL);
}
int main(int argc, char *argv[]){
  int i = 0;
  pthread_t tid[51];
  if (argc != 6)
  {
    printf("ingrese los parámetros correctos");
    exit(1);
  }
  ip = argv[1];
  port = atoi(argv[2]);
  thr = atoi(argv[4]); 
  cyc = atoi(argv[5]);
  img = argv[3];
  printf("IP: %s - PUERTO: %d - IMAGEN: %s - HILOS: %d - CICLOS %d\n", ip, port,img, thr,cyc);
  printf("Enviando archivos...\n");
  while(i< thr)
  {
    if( pthread_create(&tid[i], NULL, cientThread, NULL) != 0 )
           printf("Error al crear Thread\n");
    i++;
  }
  sleep(20);
  i = 0;
  while(i< thr)
  {
     pthread_join(tid[i++],NULL);
  }
  return 0;
}