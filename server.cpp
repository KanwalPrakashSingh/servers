/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <iostream>
#include <thread>
#include <vector>
using namespace std;


/***

a simple tcp socket server in c++ 
http://www.linuxhowtos.org/C_C++/socket.htm

***/


/***

// struct definitiion as in netinet/in.h
struct sockaddr_in
   {
     short   sin_family; // must be AF_INET 
     u_short sin_port;
     struct  in_addr sin_addr;
     char    sin_zero[8]; // Not used, must be zero 
   };

***/

int MAX_LIMIT = 100000 ;// these many numbers can be saved in the array
int CLIENTS ; //number of exchanges
mutex g_lock; 

void error(const char *msg){
    perror(msg);
    exit(1);
}

void handle_server(int thread_id,int &socket_fd){
    char buffer[256];
    int n; //this stores the status of read/write
    while(buffer[0] != '0'){
        if (socket_fd < 0){
          error("ERROR on accept");
        }
        bzero(buffer,256);
        g_lock.lock();
        n = read(socket_fd,buffer,255);
        g_lock.unlock();
        if (n < 0) {
            cout<<"thread_id "<<thread_id<<endl;
            error("ERROR reading from socket");
        }
        printf("  scoket_fd %d\n",socket_fd);
        printf("thread id : %d , Here is the message: %s\n",thread_id,buffer);
        string server_to_client = "I got your message";
        //writing the message that we received a message from client
        // n = write(socket_fd,server_to_client.c_str(), server_to_client.length());
        // cout<<"scoket fd afer server write "<<socket_fd<<endl;
        // if (n < 0){
        //     error("ERROR writing to socket");
        // }
    }
    
    g_lock.lock();
    CLIENTS--;
    close(socket_fd);
    printf("closed socket file\n");
    g_lock.unlock();
}

int main(int argc, char *argv[]){
    

    char buffer[256];
    int socket_fd, new_socket_fd, portno;
    socklen_t cli_len;
    struct sockaddr_in serv_addr; //server address and client address structs , see comments in the top for structure
    int n; 
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        error("ERROR opening socket");
    }
    bzero((char *) &serv_addr, sizeof(serv_addr)); //clearing 
    portno = atoi(argv[1]);

    // initialize the server address structure 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    
    
    if (::bind(socket_fd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){ 
          error("ERROR on binding");
    }
    listen(socket_fd,5);
    cout<<"enter the number of clients/exchanges "<<endl;
    cin>>CLIENTS;
    //thread threads_container[CLIENTS];
    // keep running till a 0 is read from file 
    int thread_id=0;
    while(CLIENTS > 0){
        struct sockaddr_in cli_addr;
        cli_len = sizeof(cli_addr);
        new_socket_fd = accept(socket_fd, (struct sockaddr *) &cli_addr,&cli_len); //accepting the connection
        //handle_server(thread_id,new_socket_fd);
        thread t(handle_server,thread_id ,ref(new_socket_fd));
        t.join();
        
        thread_id++;
        cout<<"clients active : "<< CLIENTS<<endl;
    }
    cout<<"now printing the total list "<<endl;
    close(socket_fd);
return 0; 
}

