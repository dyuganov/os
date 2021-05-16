#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

int main(){

    char outputText[] = "Greetings from the client\n";

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

    if (connect(sock, (struct sockaddr*)&socketAddr, sizeof(socketAddr)) == -1){
        perror("connection error");
        close(sock);
        return -1;
    }

    char readWriteBuf[BUFSIZ];
    char tempBuf[BUFSIZ];
    int readCount;
    int writeCount;

    while ((readCount = read(STDIN_FILENO, readWriteBuf, BUFSIZ)) > 0){
        while ((writeCount = write(sock, readWriteBuf, readCount)) < readCount){
            strncpy(tempBuf, readWriteBuf + writeCount, readCount - writeCount);
            readCount -= writeCount;
        }

        if (writeCount == -1){
            perror("writing error");
            return -1;
        }
    }

    if (readCount == -1){
        perror("reading error");
        return -1;
    }

    close(sock);
    return 0;
}
