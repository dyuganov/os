#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>
#include <poll.h>
#include <stdlib.h>

#define CLIENT_NUMBER 2

void printInUpper(const char* string, int readCount){
    char Char;
    for(int i = 0; i < readCount; i++){
        Char = string[i];
        putchar(toupper(Char));
    }
}

int getNewID(struct pollfd *connectionDescriptors){
    for (int i = 1; i < CLIENT_NUMBER; i++){
        if (connectionDescriptors[i].fd == -1){
            return i;
        }
    }
    return -1;
}

int main(){

    int sock;
    struct sockaddr_un socketAddr;
    memset(&socketAddr, 0, sizeof(socketAddr));
    socketAddr.sun_family = AF_UNIX;
    char* socketPath = "./socket";
    strcpy(socketAddr.sun_path, socketPath);

    if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
        perror("socket");
        return -1;
    }

    /*На всякий случай вызываем функцию unlink,
     * чтобы гарантировать отсутствие сгенерированного
     * имени в файловой системе. */

    unlink(socketPath);

    /*присвоить имя дескриптору */
    if (bind(sock, (struct sockaddr *)&socketAddr, sizeof(socketAddr)) < 0) {
        perror("bind");
        close(sock);
        return -1;
    }

    /* сообщить ядру, что процесс является сервером */
    if (listen(sock, 1) != 0){
        perror("listen");
        unlink(socketAddr.sun_path);
        close(sock);
        return -1;
    }

    struct pollfd connectionDescriptors[CLIENT_NUMBER];
    // initialization
    for (int i = 0; i < CLIENT_NUMBER; i++){
        connectionDescriptors[i].fd = -1;
        connectionDescriptors[i].events = POLLIN;
        connectionDescriptors[i].revents = 0;
    }

    connectionDescriptors[0].fd = sock;
    int takenDescriptorsCount = 1; // one desc in taken for socket

    ssize_t readCount = 0;
    char buffer[BUFSIZ];

    int selectedDescNum = 0;

    while(true){
        if ((selectedDescNum = poll(connectionDescriptors, CLIENT_NUMBER, 10000)) == -1){
            perror("poll(2) error");
            unlink(socketAddr.sun_path);
            close(sock);
            return -1;
        }

        if (selectedDescNum > 0){
            if (connectionDescriptors[0].revents & POLLIN){
                int newClientFD;
                int id = 0;

                if ((newClientFD = accept(sock, NULL, NULL)) == -1){
                        perror("accept");
                        unlink(socketAddr.sun_path);
                        close(sock);
                        return -1;
                }
		 if ((id = getNewID(connectionDescriptors)) != -1){
			close(newClientFD);
		} else {
                    connectionDescriptors[id].fd = newClientFD;
                    printf("connection established\n");
		}
	     }

            for (int i = 1; i < CLIENT_NUMBER; i++){
                if (connectionDescriptors[i].revents & POLLIN){
		    printf("client %d: ", i);
                    readCount = read(connectionDescriptors[i].fd, buffer, BUFSIZ);

                    if (readCount == 0){
                        printf("client %d closed connection\n", i);
                        close(connectionDescriptors[i].fd);
			connectionDescriptors[i].fd = -1;
                    }

                    printInUpper(buffer, readCount);
                    memset(&buffer, 0, sizeof(buffer));
                }
            }
        }
    }
}


