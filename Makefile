all:	
	gcc heavy_process.c -o Heavy_Process -lm
	gcc sequential_serve.c -o Secuencial -lm
	gcc client.c -o client -lpthread

run:
	 xterm -hold -e './Heavy_Process' &
	 xterm -hold -e './Secuencial' &
	 sleep 5
	./client 127.0.0.1 8080 imagen.jpg 2 2
	./client 127.0.0.1 8080 imagen2.jpeg 3 2
	./client 127.0.0.1 8080 1.png 1 2
	./client 127.0.0.2 8081 imagen.jpg 2 2
	./client 127.0.0.2 8081 imagen2.jpeg 3 2
	./client 127.0.0.2 8081 1.png 1 2




	
