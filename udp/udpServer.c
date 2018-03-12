#include <stdio.h> 
#include <string.h> //memset
#include <stdlib.h> 
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h> 

#define BUF_SIZE 512    
#define PORT     8888   

int palindrome(char s[])
{

    for (int i = 0; i<strlen(s)/2;i++)
    {
        if (s[i] != s[strlen(s)-i-1])
            return 0;
    };

    return 1;
}
 
int main(void)
{
    struct sockaddr_in si_me, si_other;
    int        s, i;
    socklen_t  slen = sizeof(si_other), recv_len;
    char       buf[BUF_SIZE];
     
    //create an UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        perror("socket");
        exit(1);
    }
     
    //clear struct 
    memset((char *) &si_me, 0, sizeof(si_me));
     
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
     
    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        perror("bind");
        exit(1);
    }
     
    //listening 
    while(1)
    {

        memset(buf, '\0', sizeof(buf));

        printf("Waiting for data...\n");
        fflush(stdout);
         
        //try to receive some data
        if ((recv_len = recvfrom(s, buf, BUF_SIZE, 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
            perror("recvfrom()");
            exit(1);
        }
         
        //print client details 
        //printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        
        //reply the client
        if (palindrome(buf)) 
        {
            if (sendto(s, "YES", recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
            {
                perror("sendto()");
                exit(1);
            }
        }    
        else 
        {
            if (sendto(s, "NO", recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
            {
                perror("sendto()");
                exit(1);
            }
        }
    }
 
    close(s);
    return 0;
}