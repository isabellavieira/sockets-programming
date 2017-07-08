/*
 * File:   server.c
 * Author: sachetto
 *
 * Created on 19 de Setembro de 2014, 15:54
 */

#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>        /* for POSIX threads */

#define SERVER_PORT       (2002)
#define MAX_SIZE          (2048)
#define BYTES 1024

char* read_file (const char* filename, size_t* length);
char* read_request(int sockd);
void send_response(int sockd, char *nomeDoArquivo);
void respondToRequest (int conn_s) ;
void *respondToRequestThread (void *arg);

/* Estrutura de argumentos para as threads*/
struct ThreadArgs {
    int socketID;                     
};

/**
 * Function main
 * @params: 	porta (8080, 8060, 2020 ou 2002)
 * @execucao: 	./thread 8080
 */
int main(int argc, char** argv) {

	int list_s;
	int conn_s;
	int addr_size;
	short int port;
	struct sockaddr_in servaddr;
	struct sockaddr_in clientaddr;
	char *endptr;
	pid_t pid;
	pthread_t tid;
	struct ThreadArgs *threadArgs;   
	int yes =1;
	
	//Inicializações
	threadArgs = (struct ThreadArgs *) malloc(sizeof(struct ThreadArgs));
	
	//Argumentos - porta do listener
	if (argc == 2) {
		port = strtol(argv[1], &endptr, 0);
		if (*endptr) {
			fprintf(stderr, "SERVER: NUMERO DE PORTA INVALIDOS.\n");
			exit(EXIT_FAILURE);
		}
		
	} else if ( argc < 2 ) {
		port = SERVER_PORT;
		
	} else {
		fprintf(stderr, "SERVER: ARGUMENTOS INVALIDOS.\n");
		exit(EXIT_FAILURE);
	}
	
	/* 
	 * Create the listening socket
	 * Erro: se retornar -1
	 * @params: AF_INET -> IP versão 4
	 * 			SOCK_STREAM -> conexão orientada protocolo TCP
	 * 			0 -> Protocolo IP
	 */
	if ( (list_s = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {	
		fprintf(stderr, "SERVER: ERRO AO CRIACAO DO SOCKET.\n");				
		exit(EXIT_FAILURE);
	}

	/*  Set all bytes in socket address structure to
        zero, and fill in the relevant data members   */
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(port);

	// lose the pesky "Address already in use" error message
	if (setsockopt(list_s,SOL_SOCKET,SO_REUSEPORT,&yes,sizeof(int)) == -1) {
		perror("ERRO: SETSOCKOPT()\n");
		exit(1);
	}

	/*  Bind our socket addresss to the
	listening socket, and call listen()  */
	if ( bind(list_s, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) {
		fprintf(stderr, "SERVER: ERRO AO CHAMAR BIND()\n");
		exit(EXIT_FAILURE);
	}

	if ( listen(list_s, 1024) < 0 ) {
		fprintf(stderr, "SERVER: ERRO AO CHAMAR LISTEN()\n");
		exit(EXIT_FAILURE);
	}

	while ( 1 ) {
		printf("\nESCUTANDO NA PORTA: %d\n\n", port);
		
		//Esperar por uma conexão
		if ( (conn_s = accept(list_s, 0, 0) ) < 0 ) {
			perror("ERRO: ACEITAR REQUISICAO");
			exit(EXIT_FAILURE);
		} 
	
		threadArgs->socketID = conn_s;

		//Criar uma thread
        if (pthread_create(&tid, NULL, respondToRequestThread, (void *) threadArgs) != 0){
			printf("with thread %ld\n", (long int) tid);
		}
	}
	close(list_s);
	return (EXIT_SUCCESS);
}


/**
 * Function: read_file
 * @params:	 filename, length
 * @description: Função responsável por ler 
 */
char* read_file (const char* filename, size_t* length) {
	int fd;
	struct stat file_info;
	char* buffer;
	
	fd = open (filename, O_RDONLY);

	//Se pode abrir o arquivo
	if (fd > 0 ){

		/* Get information about the file.  */
		fstat (fd, &file_info);
		*length = file_info.st_size;
		
		/* Make sure the file is an ordinary file.  */
		if (!S_ISREG (file_info.st_mode)) {
			/* It's not, so give up.  */
			close (fd);
			return NULL;
		}

		/* Allocate a buffer large enough to hold the file's contents.  */
		buffer = (char*) malloc (*length);
		
		/* Read the file into the buffer.  */
		read (fd, buffer, *length);

		/* Finish up.  */
		close (fd);
		
	} else {
		length = 0;
	}
	return buffer;
}

/**
 * Function: read_request
 * @params:  socket
 * @description:  Read a request from a socket  
 */
char* read_request(int sockd) {

	char buffer[256];
	char *nomeDoArquivo;
	int apontador1 = 1;
	char caractere;
	int i = 0;
	ssize_t bytes_read;
	
	nomeDoArquivo = (char*) malloc (sizeof(char)*200);

	/* Read some data from the client.  */
	bytes_read = read (sockd, buffer, sizeof (buffer) - 1);
	
	if (bytes_read > 0) {
		char method[sizeof (buffer)];
		char url[sizeof (buffer)];
		char protocol[sizeof (buffer)];

		buffer[bytes_read] = '\0';
		sscanf (buffer, "%s %s %s", method, url, protocol);

		while (strstr (buffer, "\r\n\r\n") == NULL)
			bytes_read = read (sockd, buffer, sizeof (buffer));

		printf ("REQUISICAO:\n\tMethod: %s\n\tURL: %s\n\tProtocol: %s\n", method, url, protocol);
		
		caractere = url[1];		
		while (caractere!= ' ' && caractere != '\0' && caractere != '\n' && caractere != '\\') {
			nomeDoArquivo[i] = caractere;
			i++;
			apontador1++;
			caractere = url[apontador1];
		}
		nomeDoArquivo[i] = '\0';
		printf("\nARQUIVO REQUISITADO: \n\t%s\n\n\n", nomeDoArquivo);
	}
	return nomeDoArquivo;
}

/**
 * Function: 
 * @params: socket
 * @description:  Write a line to a socket  
 */

void send_response(int sockd, char *nomeDoArquivo) {
     char *bytes, *notfound, *v, *extensao;
    char tmp[MAX_SIZE];
    ssize_t nwritten;
    size_t file_size = 0;

    bytes = read_file(nomeDoArquivo, &file_size);     
    
	char header[MAX_SIZE];
	if (file_size > 0){
		printf("\nARQUIVO REQUISITADO ENCONTRADO: %s\n", nomeDoArquivo);
		
		//Montando o cabecalho HTTP  
		strcpy(header, "HTTP/1.0 200 OK\r\n");
		strcat(header, "Connection: close\r\n");
		strcat(header, "Data: Sun, 25 Out 2015 17:26:15 GMT\r\n");
		strcat(header, "Server: Supergirls/1.0.0 (Windows)\r\n");
		strcat(header, "Last-Modified: 21 b.c.\r\n");
		strcat(header, "Last-Modified: Sun, 01 Out 2015 17:26:15 GMT\r\n");
		sprintf(tmp,   "Content-Length: %ld\r\n", file_size);
		strcat(header, tmp);
		extensao = strstr (nomeDoArquivo,".");
		
		//Colocar o content type de acordo com a extensao do arquivo
		if (strcmp(extensao, ".png") == 0) {
			strcat(header, "Content-Type: image/png\r\n"); 
		} else if (strcmp(extensao, ".html") == 0) {
			strcat(header, "Content-Type: text/html\r\n"); 
		} else if (strcmp(extensao, ".jpg") == 0) {
			strcat(header, "Content-Type: image/jpg\r\n");
		}
		
		strcat(header, "Accept-Ranges: bytes\r\n");
		strcat(header, "\r\n");
	}else {
		printf("\nARQUIVO REQUISITADO NAO ENCONTRADO: %s\n", nomeDoArquivo);
		bytes = NULL;
		bytes = read_file("files/notfound.html", &file_size);
		
		//Montando o cabecalho HTTP  
		strcpy(header, "HTTP/1.0 404 Not Found\r\n");
		strcat(header, "Connection: close\r\n");
		strcat(header, "Data: Sun, 25 Out 2015 17:26:15 GMT\r\n");
		strcat(header, "Server: Supergirls/1.0.0 (Windows)\r\n");
		sprintf(tmp,   "Content-Length: %ld\r\n", file_size);
		strcat(header, tmp);
		strcat(header, "Content-Type: text/html\r\n"); 
		strcat(header, "Accept-Ranges: bytes\r\n");
		strcat(header, "\r\n");
	}

    printf("%s\n", header);

	// Enviando cabeçallho - header
    if ( (nwritten = send(sockd, header, strlen(header), 0) ) <= 0 ) {
        if ( errno != EINTR )
            exit(EXIT_FAILURE);
    }
	
    // Enviando arquivo
    if ( (send(sockd, bytes, file_size,0)) <= 0 ) {
        if ( errno != EINTR )
            exit(EXIT_FAILURE);
    }
}

/**
 * Function
 * @param:
 * @description:
 */
void respondToRequest (int conn_s) {
	char *nomeDoArquivo;
	nomeDoArquivo = read_request(conn_s);
	send_response(conn_s, nomeDoArquivo);
}

/**
 * Function
 * @param:
 * @description:
 */
void *respondToRequestThread (void *arg) {
	
	int id;
	char *nomeDoArquivo;

	id = *((int *) arg);
	pthread_detach(pthread_self());

	//Atender requisição
	nomeDoArquivo = read_request(id);
	send_response(id, nomeDoArquivo);

	close(id);
	return (NULL);
}
