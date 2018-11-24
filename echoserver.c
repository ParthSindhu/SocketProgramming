// Echoserver side application
// For linux systems
// Written in C not C++

#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h> // for socket
#include<sys/socket.h>
#include<netinet/in.h> // sockaddr definition
#include<errno.h> // perror
#include<strings.h>
#include<unistd.h>
#include<arpa/inet.h>

#define ERROR -1
#define MAX_CLIENTS 2
#define MAX_DATA 1024

int main(int argc, char **argv)
{
    int sock,newc;
    int data_len = 1;
    int sent = 0;
    
    char data[MAX_DATA] = "Welocome to my first server!"; // initial message
    
    struct sockaddr_in server,client;
    int sockaddr_len = sizeof(struct sockaddr_in);
    
    if( (sock = socket(AF_INET,SOCK_STREAM,0)) == ERROR) //SOCK_STREAM ==> TCP
    {
        perror("Server Socket :");
        exit(ERROR);
    }
    
    printf("*****Server Application******\n");
    printf("Socket Creation Succesful...\n");
    
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[1]));
    server.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces on the local machine
    
    bzero(&server.sin_zero,8);
    
    printf("Binding now...\n");
    
    if ( (bind(sock , (struct sockaddr*)&server , sockaddr_len)) == ERROR)
    {
        perror("Bind :");
        exit(ERROR);
    }
    
    if (listen(sock , MAX_CLIENTS) != ERROR)
    {
        printf("Started Listening.\n");
        
        while(1)
        {
		
            if ( (newc = accept(sock , (struct sockaddr *)&client , &sockaddr_len)) == ERROR )
            {
                perror("Accept :");
                exit(ERROR);
            }
            
            printf("New Client Connected from port no %d and IP %s",ntohs(client.sin_port),inet_ntoa(client.sin_addr));
            
            sent = send(newc , data , sizeof(data) + 1 , 0);
            
            while(data_len > 0)
            {
                data_len  = recv(newc , data , MAX_DATA , 0);
                
                if (data_len)
                {
                    sent += send(newc , data , data_len , 0);
                    data[data_len] = '\0';
                    printf("Sent Msg: %s",data);
                }
                
            }
            
            printf("Sent %c bytes to: %d", sent , inet_ntoa(client.sin_addr));
            
            close(newc);
            
        }
        
        
	//system ("/bin/stty raw");
        //char c;
        //printf("Press q To Quit");
        //
        //while( (c=getchar())!= 'q')
        //{
	//	
        //    if ( (newc = accept(sock , (struct sockaddr *)&client , &sockaddr_len)) == ERROR )
        //    {
        //        perror("Accept :");
        //        exit(ERROR);
        //    }
        //    
        //    //printf("New Client Connected from port no %d and IP %s",ntohs(client.sin_port),inet_ntoa(client.sin_addr));
        //    
        //    sent = send(newc , data , sizeof(data) + 1 , 0);
        //    
        //    while(data_len > 0)
        //    {
        //        data_len  = recv(newc , data , MAX_DATA , 0);
        //        
        //        if (data_len)
        //        {
        //            sent += send(newc , data , data_len , 0);
        //            data[data_len] = '\0';
        //            printf("Sent Msg: %s",data);
        //        }
        //        
        //    }
        //    
        //    printf("Sent %c bytes to: %d", sent , inet_ntoa(client.sin_addr));
        //    
        //    close(newc);
        //    
        //}
        //
        //system ("/bin/stty cooked");
    }   

	return 0;
}

