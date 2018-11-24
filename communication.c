// software to connect to wearable device- PROTOCOL TCP
// server software - to be run on the intermediate device(Assumed rasberry pie) to comunicate with the wearable device
// KEY FUNCTIOINS:
// communicate with wearable device 
// *pass information to the gesture recognition software
// take gesture number and assosiated value from gesture software // there must be a command file with enteries line {CommandNO} {COMMAND} 
// pass information to the interfac

//  * NOT IMPLEMENTED YET

// wearable device should send values as integer, for example for 2.1245 send 21245 (default to 4 decimal places)
// device will receive true after succesful connection and succesful data transfer
// to indicate unsuccesful transfer an ERROR will be sent and device must send data again

#include<stlib.d>
#include<stdio.h>
#include<sys/socket.h> // add the socked headers
#include<sys/types.h>
#include<string.h>
#include<string>
#include<fstream.h> // For command file
#include<unistd.h>
#include<arpa/inet.h>
#include<netinet/in.h> // sockaddr definition

#DEFINE MAC  // Put MAC address of wearable device here
#DEFINE MAX_C  2 // Maximum number of connections ALLOWED 
// #DEFINE MAX_DATA 1024 (only integer sent so not needed) // maximum bytes allowed to be sent
#DEFINE ERROR -1

// Function Prototypes
bool validate();
void gesture_recognition(int a[][]); // launch the gesture recognition program
bool interface(); // launch interface

void main()
{
// Definitions
int sock , cli;
struct sockaddr_in server , client;
int sockaddr_len = sizeof(struct sockaddr_in);
bool val = true;

int sdata = 1 
int rdata;

int findata[6][3];
int received; 

bool rflag = true;
// Creating socket
sock = socket(AF_INET , SOCK_STREAM , 0);

server.sin_family = AF_INET;
server.sin_port = htons(port) ;
server.sin_addr.s_addr = INADDR_ANY;

bzero(&server.sin_zero,8);

// Binding 

if (bind(sock , (struct sockaddr *)server, &sockaddr_len ) == ERROR)
{
    perror("BIND :");
    exit(ERROR);
}

listen(sock , MAX_C);

while(val)
{
	cli = accept(sock , (struct sockaddr*)client ,&sockaddr_len);
	
	if(cli == ERROR)
	{
	    perror("ACCEPT :");
	    exit(ERROR);
	}
 
	val = validate(); // validate weather connection with correct host has been established
		
	if(!val)
	{
		close(); // close the connection
		val = true;
		continue;
	}
}

sdata = htonl(sdata);
send(cli , &sdata , sizeof(sdata),0); // Send 1 to wearable device to indicate succesful connection. 

do
{
	for (int i = 0; i < 6 ; i++)
	{
		for(int j=0; j< 3; j++)
		{
			received = recv(cli, &rdata, sizeof(rdata),0);
			
			rdata = ntohl(rdata);
			
			if(received > 0)
			{
				findata[i][j] = rdata;
			}	
			else
			{
				findata[i][j] = ERROR;
				rflag = false;
			}
		}
	}


	if(rflag)
	{
		sdata = true;
		sdata = htonl(sdata);
		send(cli,&sdata,sizeof(sdata),0); // Indicate to wearable device that data has been received
		rflag = true;
	}
	else
	{
		sdata = ERROR;
		sdata = htonl(sdata);
		send(cli,&sdata,sizeof(sdata),0); // Indicate to wearable device that data not received
	}

}while(!rflag);

int command [2] = thread gesture(gesture_recognition , findata); // light number and command number , make a command file?

if(!interface(command))
{
    perror("Interface fail!");
    exit(ERROR);
}

}

bool validate()
{
	string str_MAC = MAC;
	string str_ip;
	
	ifstream file;
    file.open("/proc/net/arp");
    
    string line ;
    bool flag = false;
    
    while(getline(file,line))
    {
        // reading file line by line
        if(line.find(str_MAC) != string::npos) // if something was found
        {
            str_ip = line.substr(0,line.find(" ") - 1); // provided line doesnt start with a space 
            flag = true;
        }
    }
	
	return flag;
}

void gesture_recognition(int a[3][6])
{
	// send the data over
    // I have no idea how to do this. 
}
 
bool interface(int c[2]) // How to adjust values only. REWRITE FUNCTION
{
ifstream file;
file.open("command");
string line , strn, str;
int n;
bool flag = false;

while(getline(file , line))
{
    strn = line.substr(0 , line.find(" ") - 1);
    n = stoi(strn);
    
    if(n == c[1])
    {
        str = line.substr(line.find(" ") + 1);
        system("./Interface " + strn + " " + str);
        flag = true;
    }
}
return flag;
}