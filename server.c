
// Attempt at server side application
// For linux systems
// Written in C not C++

#include<stdli.h>
#include<stdio.h>
#include<sys/types.h> // for socket
#include<sys/socket.h>
#include<netinet/in.h> // sockaddr definition
#include<errno.h> // perror

#include<arpa/inet.h>

void main
{
    int sock,cli;
    int port = 80;
    
    char message[200] = "Welocome to my first server!"; // initial message
    
    struct sockaddr_in server,client;
    
    if( (sock = socket(AF_INET,SOCK_STREAM,0)) == -1) //SOCK_STREAM ==> TCP
    {
        perror("Socket :");
        exit(-1);
    }
    
    printf("*****Server Application******\n");
    printf("Socket Creation Succesful...\n");
    printf("Enter Port number(Default = 80):\n");
    scanf("%u" , port);
    
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;
    
    bzero(&server.sin_zero,8)
    
    printf("Binding now...\n");
    
    if ( (bind(sock , (struct sockaddr*)&server , sizeof(server))) == -1)
    {
        perror("Bind :");
        exit(-1);
    }
    
    if (listen(sock , 5) != -1)
    {
        printf("Started Listening.\n");
        
        system ("/bin/stty raw");
        char c;
        printf("Press q To Quit");
        
        while( (c=getchar())!= 'q'))
        {
            if ( (cli = accept(sock , (struct sockaddr *)client , &sizeof(client))) == -1 )
            {
                perror("Accept :");
                exit(-1);
            }
            
            sent = send(cli , message , sizeof(message) + 1 , 0);
            
            printf("Sent %c bytes to: %d", sent , inet_ntoa(client.sin_addr));
            
            close(cli);
            
        }
        
        system ("/bin/stty cooked");
    }   
    
    
}