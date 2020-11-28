#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h> 
#include<pthread.h>

char path[150] = "/home/kimberly/Documentos/operativos/Proyecto2/img/"; //cambiar
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

int main(){
  int serverSocket, newSocket;
  struct sockaddr_in serverAddr;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;

  serverSocket = socket(PF_INET, SOCK_STREAM, 0); 
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(8081);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
  bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

  //Se agregan conexiones a una cola
  if(listen(serverSocket,50)==0)
    printf("Escuchando\n");
  else
    printf("Error\n");
    int env = 0;
    while(1)
    {
      printf("Siguiente conexion:\n");
      addr_size = sizeof serverStorage;
      if (newSocket = accept(serverSocket, (struct sockaddr *) &serverStorage, &addr_size))
      {
        puts("Aceptada");
      }

      if (newSocket < 0)
      {
        perror("Falló la conexión");
        exit(1);
      }
        socketThread(newSocket, env);
        env ++;
        close(newSocket);
    }
  return 0;
}