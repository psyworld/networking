#include <arpa/inet.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> //memset
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 512    //Max length of buffer
#define PORT     8888   //The port on which to send data
#define SERVER   "127.0.0.1"
 
int main(void)
{
    struct      sockaddr_in si_other;
    int         s, i;
    socklen_t   slen = sizeof(si_other);
    char        buf[BUF_SIZE];
    char        message[BUF_SIZE];
 

    //create a socket
    if ( (s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        perror("socket");
        exit(1);
    }
 
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port   = htons(PORT);
     
    if (inet_aton(SERVER , &si_other.sin_addr) == 0) 
    {
        perror("inet_aton()");
        exit(1);
    }
 
    while(1)
    {
        printf("Enter message : ");
        scanf("%s", message);
         
        //send the message
        if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
        {
            perror("sendto()");
            exit(1);    
        }
                 
        memset(buf,'\0', BUF_SIZE);
        
        //try to receive some data
        if (recvfrom(s, buf, BUF_SIZE, 0, (struct sockaddr *) &si_other, &slen) == -1)
        {
            perror("recvfrom()");
            exit(1);
        }
         
        puts(buf);
    }
 
    close(s);
    return 0;
}