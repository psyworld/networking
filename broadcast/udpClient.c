#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <ifaddrs.h>
#include <stdlib.h> 
#include <string.h> //memset
#include <unistd.h>
#include <stdio.h> 
#include <netdb.h>

#define BUF_SIZE 512

struct proto
{
    unsigned short id;
    short flag;
    char  buf[BUF_SIZE];    
};

int inet_aton(const char *cp, struct in_addr * inp);

int main(int argc, char * argv[])
{
    int         s, i;
    char        buf[BUF_SIZE];
    char        message[BUF_SIZE];
    unsigned    optval = 1; 

    const int PORT = strtol(argv[1], NULL, 10);
    struct sockaddr_in saddr_server = {AF_INET, htons(PORT)};

    //create a socket
    if ( (s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        perror("socket");
        exit(1);
    };

    if (setsockopt(s, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) < 0)
    {
        perror("setsockopt");
        exit(1);
    };


    if (inet_aton("255.255.255.255", &saddr_server.sin_addr) == 0) 
    {
        perror("inet_aton()");
        exit(1);
    };

    unsigned short id = 0;

    while(1)
    {

        struct sockaddr_in saddr_server_ = saddr_server;
        socklen_t   slen = sizeof(saddr_server);
        struct proto data_req;
        struct proto data_res;

        data_req.id = id++;

        fgets(data_req.buf, sizeof(data_req.buf), stdin);
        data_req.buf[strlen(data_req.buf)-1] = '\0';

        while(1)
        {
            //send the message
            if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &saddr_server, slen)==-1)
            {
                perror("sendto()");
                exit(1);    
            };
                     
            
            //try to receive some data
            if (recvfrom(s, buf, BUF_SIZE, 0, (struct sockaddr *) &saddr_server, &slen) == -1)
            {
                perror("recvfrom()");
                exit(1);
            };

            if (data_res.id == data_req.id) break;
            else 
                printf("\t Got message with wrond id\n");

            
        }

        printf(data_res.flag ? "YES\n" : "NO\n");
    }
 
    close(s);
    return 0;
}