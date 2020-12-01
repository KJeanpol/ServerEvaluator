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
#include <sys/wait.h>
#include <time.h>
#include <signal.h>
#include <math.h>


#define PORT 8081

char path[150] = "/home/kimberly/Documentos/operativos/Proyecto2/Proyecto2/imgs";
char client_message[2000];
char buffer[1024];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int name_img = 1, ite4 =0;
time_t begin, end, begin_unit, end_unit;
long acum=0;
FILE *graf1, *graf2, *graf3;

void  socketThread(int newSocket, int env)
{

    int recv_size = 0, size = 0, read_size, write_size, packet_index = 1, stat, cyc = 0, aut=0, ite =0;
    char imagearray[10241];
    char message[9] = "Petición",num2[200], ruta[400];
    FILE *image;
    graf2 = fopen("2_2.txt", "a");
    graf3 = fopen("2_3.txt", "a");
    if (graf2 == NULL)
    {
        printf("No se pudo abrir la imagen\n");
        exit(1);
    }
    if (graf3 == NULL)
    {
        printf("No se pudo abrir la imagen\n");
        exit(1);
    }
    write(newSocket, message, 13);
    read(newSocket, &size, sizeof(int));

    write(newSocket, message, 13);
    read(newSocket, &cyc, sizeof(int));
    
    while(ite<cyc){
        time(&begin_unit);
        write(newSocket, message, 13);
        read(newSocket, &aut, sizeof(int));

        if (aut == -1)
        {
        sprintf(num2, "%s/%d_%d.jpg", path, env, name_img);
        name_img ++;
        image = fopen(num2, "w");
        if (image == NULL)
        {
            printf("No se pudo abrir la imagen\n");
            exit(1);
        }

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

            write_size = fwrite(imagearray, 1, read_size, image);
            if (read_size != write_size)
            {
                printf("Error\n");
            }
            recv_size += read_size;
            }
        }
            recv_size = 0;
            fclose(image);
            int tipo = 2;
            sprintf(ruta, "python3 filter.py %s %d\n", num2, tipo);
            printf("RUTA IMAGEN %s\n", num2);
            system (ruta);
            
        }
        ite++;
        time(&end_unit);
        time_t elapsed = end_unit - begin_unit;
        //printf("Tiempo solicitud: %ld seconds.\n", elapsed);
        acum += elapsed;
        //printf("Tiempo acumulado: %ld seconds.\n", acum);
        int elapsed_cifras=0, acum_cifras =0, elapsed2 = elapsed, acum2 = acum, acum3= acum, temp;
        while(elapsed2>0)
        {
            elapsed2=elapsed2/10;
            elapsed_cifras++;
        }
        while(acum2>0){
            acum2/=10;
            acum_cifras++;
        }
        elapsed_cifras = pow(10, elapsed_cifras-1);
        acum_cifras = pow(10, acum_cifras-1);
        while(elapsed_cifras >0){
            temp = elapsed/elapsed_cifras;
            elapsed%=elapsed_cifras;
            elapsed_cifras/=10;
            //printf("Escribiendo1: %d\n", temp);
            fputc(temp+48, graf2);
        }
        while(acum_cifras >0){
            temp = acum3/acum_cifras;
            acum3%=acum_cifras;
            acum_cifras/=10;
            //printf("Escribiendo2: %d\n", temp);
            fputc(temp+48, graf3);
        }
            fputc(',', graf2);
            fputc(',', graf3);
    }
    env += name_img;
    ite4 += ite;
    fclose(graf2);
    fclose(graf3);
}
int  idHijo;
void catch(int sig)
{   
    //printf("Señal: %d atrapada!\n", sig);
    int ite_cifras=0, ite2=ite4, temp=0, elapsed2, elapsed_cifras=0;
    time(&end);
    time_t elapsed = end - begin;
    printf("Tiempo total: %ld seconds.\n", elapsed);
    graf2 = fopen("2_2.txt", "a");
    graf3 = fopen("2_3.txt", "a");
    if (graf2 == NULL)
    {
        printf("No se pudo abrir la imagen\n");
        exit(1);
    }
    if (graf3 == NULL)
    {
        printf("No se pudo abrir la imagen\n");
        exit(1);
    }
    fputc(']', graf2);
    fputc(']', graf3);
    fclose(graf2);
    fclose(graf3);
    graf1 = fopen("1_1.txt", "a");

    if (graf1 == NULL)
    {
        printf("No se pudo abrir la imagen\n");
        exit(1);
    }
    /*while(ite2>0)
    {
        ite2=ite2/10;
        ite_cifras++;
    }
    ite_cifras = pow(10, ite_cifras-1);
    while(ite_cifras >0){
        temp = ite4/ite_cifras;
        ite4%=ite_cifras;
        ite_cifras/=10;
        fputc(temp+48, graf1);
    }*/
    fputc(',', graf1);
    elapsed2 = elapsed;
    while(elapsed2>0)
    {
        elapsed2=elapsed2/10;
        elapsed_cifras++;
    }
    elapsed_cifras = pow(10, elapsed_cifras-1);
    while(elapsed_cifras >0){
        temp = elapsed/elapsed_cifras;
        elapsed%=elapsed_cifras;
        elapsed_cifras/=10;
        fputc(temp+48, graf1);
    }
    fclose(graf1);
    exit(1);
}

int main(){
  int serverSocket, newSocket;
  struct sockaddr_in serverAddr;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;
  pid_t childpid;
  time(&begin);

  serverSocket = socket(PF_INET, SOCK_STREAM, 0); 
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(8081);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.2");
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
  bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
  
  remove("2_1.txt");
  remove("2_2.txt");
  remove("2_3.txt");
  
  graf2 = fopen("2_2.txt", "a");
  graf3 = fopen("2_3.txt", "a");
  if (graf2 == NULL)
  {
      printf("No se pudo abrir el archivo\n");
      exit(1);
  }
  if (graf3 == NULL)
  {
      printf("No se pudo abrir el archivo\n");
      exit(1);
  }
  fputc('[', graf2);
  fputc('[', graf3);
  fclose(graf2);
  fclose(graf3);
  if(listen(serverSocket,50)==0)
    printf("[+]Listening....\n");
  else

    printf("Error\n");  
    int env = 1;
    while(1){
            signal(SIGINT, &catch);
            printf("[+]Waiting for connections....\n");
            addr_size = sizeof serverStorage;
            if(newSocket = accept(serverSocket, (struct sockaddr *) &serverStorage, &addr_size))
            {
                printf("[+]Connection accepted\n");
                childpid = fork();
                
                if(childpid == 0){
                    socketThread(newSocket, env);
                    close(newSocket);
                    kill(getpid(), SIGKILL); 
                }
                else{
                    idHijo = childpid;
                    env ++;
                }
            }
            if (newSocket < 0) 
            {
                perror("[+]Connection failed!");
                exit(1);
            }       
    }

    close(childpid);
    return 0;
}