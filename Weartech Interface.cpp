// control philips hue lights using this program. uses curl, so only for linux/mac systems.
// USAGE : ./Interface {LightNO} {ON/OFF} {/BRIghtness/HUE/SATuration} {ADJUST/RETURN} {IF ADJUST then Value} {username?}

#include<stlib.h>
#include<fstream>
#include<string>
#include<ctype.h>

using namespace std;

#DEFINE MAC // give mac of bridge here

void main(int argc, char *argv[])
{
    // Finding ip of the bridge using mac address
    
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

    if(flag)
    {
        // ip address was found
        // USER COMMANDS 
        // LNO - Light Number
        // SAT - saturation
        // BRI - Brightness
        // HUE - Hue
        // USAGE : ./Interface {LightNO} {ON/OFF} {/BRIghtness/HUE/SATuration} {ADJUST/RETURN} {IF ADJUST then Value} {username?}
        // have to make username too , check arv
        // delete / create light bulb?
        int num = (argc > 5 ? 5 : argc);
        bool flag3 = true;
        string username;
        
        if(argc < 3)
        {
            cerr("Insufficient Arguments");
            exit(-1);
        }
        for(int j=1;j < num ; j = 5) // checks weather the numerical values are numerical or not.
        {   
            for(int i = 0;i < arg[j].length() ;i++)
            {
                if(!isdigit(argv[j][i])) 
                    flag3 = false;
            }
        }
        if(!flag3)
        {
            cerr("Invalid Arguments");
            exit(-1);
        }
        
        bool flag2 = false;
        
        if (argc == 7)
        {
            argc--; // if username is provided
            flag2 = true; // flag indicates wheather username is provided by the user
        }
        
        if (!flag2)
        {
            // assign username
        }
        
        string URL = "http://" + str_ip + "/api/" + username + "/lights" ; // string to hold the URL
        
        if(argv[4] == "RETURN" && argc == 5)
        {
            URL += "/" + arg[1];
            system("curl" + URL);
        }
        else if(argv[4] == "ADJUST" && argc = 6)
        {
            URL += "/" + arg[1] + "/state"; 
            
            string command;
            
            if(argv[2] == "OFF")
            {
                command = "on=false ";
            }
            else if (argv[2] == "ON")
            {
                command = "on=true&";
                
                if(argv[3] == "SAT")
                {
                    command = command + "sat=" + argv[5]; 
                }
                
                else if(argv[3] == "BRI") 
                {
                    command = command + "bri=" + argv[5];
                }
                
                else if(argv[3] == "HUE") 
                {
                    command = command + "sat=" + argv[5];
                }
            }
            system("curl -X PUT -d " + command + URL);
        }
        else
        {
            // invalid arguments
            cerr("Invalid arguments");
            exit(-1);
        }
    }
    else
    {
        cerr("IP Not found");
        exit(-1);
    }
}