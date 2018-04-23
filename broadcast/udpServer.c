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


int palindrome(char s[])
{

    for (int i = 0; i<strlen(s)/2;i++)
    {
        if (s[i] != s[strlen(s)-i-1])
            return 0;
    };

    return 1;
}
 
int main(int argc, char * argv[])
{
    int         s, i;
    char        buf[BUF_SIZE];
    char        message[BUF_SIZE];
    unsigned    optval = 1; 

    const int PORT = strtol(argv[1], NULL, 10);
    struct sockaddr_in saddr_server = {AF_INET, htons(PORT)};
    
    //create an UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        perror("socket");
        exit(1);
    }

    if (inet_aton("255.255.255.255", &saddr_server.sin_addr) == 0) 
    {
        perror("inet_aton()");
        exit(1);
    };

    if ( setsockopt(s, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) < 0)
    {
        perror("setsockopt()");
        exit(1);   
    } 

    struct sockaddr_in saddr_client;
    //bind socket to port
    if (bind(s, (const struct sockaddr*) &saddr_server, sizeof(saddr_server)) == -1 )
    {
        perror("bind");
        exit(1);
    }
     
    //listening 
    while(1)
    {
        printf("Waiting for data...\n");

        unsigned int slen = sizeof(saddr_server);
        struct proto data_req;
        
         
        //try to receive some data
        if ((recvfrom(s, &data_req, sizeof(data_req), 0, (struct sockaddr *) &saddr_client, &slen)) == -1)
        {
            perror("recvfrom()");
            exit(1);
        }
         
        //print client details 
        //printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        


        //reply the client
        data_req.flag = palindrome(data_req.buf);
        printf(data_req.flag ? "YES\n" : "NO\n");

        if (sendto(s, &data_req, sizeof(data_req), 0, (struct sockaddr*) &saddr_client, slen) == -1)
        {
            perror("sendto");
            exit(1);
        }
    }
 
    close(s);
    return 0;
}