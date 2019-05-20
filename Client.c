#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define BUFSIZE 1025
#define REACTION_BUFSIZE 257
#define PORT_NO 2001

int main(int argc, char *argv[] ){

  printf("\e[1;1H\e[2J"); //clears the screen on linux

  int network_socket;
  char buffer[BUFSIZE];
  char reactionBuffer[REACTION_BUFSIZE];
  char langResponseBuffer[BUFSIZE];
  char voteResponseBuffer[BUFSIZE];
  char server_addr[16];
  int connection_status;
  int message;
  int answearMessage;
  int reactionMessage;
  int voteMessage;
  char server_response[BUFSIZE];
  char server_langChoose[BUFSIZE];
  char server_voteResponse[BUFSIZE];

  //specify an address for the socket
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT_NO); //htons converts normal looking port to some C magic
  server_address.sin_addr.s_addr = INADDR_ANY;

  //create a socket
  network_socket = socket(AF_INET, SOCK_STREAM, 0); //(domain, type, protocoll)
  
  if (network_socket < 0) {
    fprintf(stderr, "Can't create to the socket\n");
    exit(1);
  }

  //This makes the first argument the server IP to connect to
  sprintf(server_addr, "%s", argv[1]);
  inet_addr(server_addr);

  //Connect to server
  connection_status = connect(network_socket, (struct sockaddr *) &server_address,sizeof(server_address));

  //Check for error with the connection
  if(connection_status == -1){
    fprintf(stderr, "Can't connect to the socket\n");
    exit(2);
  }

  //Recive greating message from the server
  recv(network_socket, &server_response, sizeof(server_response), 0);
  recv(network_socket, &server_langChoose, sizeof(server_langChoose), 0);

  //print out the server response
  printf("The server sent the following message and question:\n%s\n%s\n", server_response, server_langChoose);

  printf("Answear: ");
  fgets(langResponseBuffer, BUFSIZE, stdin);
  answearMessage=send(network_socket, langResponseBuffer, BUFSIZE,0);
  //if this is Client2 it will skip the writing part of the endless loop in the first cycle
  //so it will just print Client1's message
  if (strstr(server_response, "Hello Client2") != NULL){
    goto kliens2_kezdete;
  }

  //Chatting starts here
  while(1){
    recv(network_socket, &server_voteResponse, sizeof(server_voteResponse), 0);
    printf("The server sent the following message! :\n%s\n", server_voteResponse);
    fgets(voteResponseBuffer, BUFSIZE, stdin);
    voteMessage = send(network_socket, voteResponseBuffer, BUFSIZE, 0);
    recv(network_socket, &server_voteResponse, sizeof(server_voteResponse), 0);
    if (strstr(server_voteResponse, "The conversation is over!") != NULL){
      printf("%s",server_voteResponse);
      break;
      
    }
    else{
      printf("%s",server_voteResponse);
    }
    
    printf("Reagálás:ASDASDASD ");
    fgets(reactionBuffer,REACTION_BUFSIZE,stdin);
    reactionMessage=send(network_socket, reactionBuffer, REACTION_BUFSIZE,0);
    if(message < 0) {
       fprintf(stderr, "Can't send messageLUL\n");
       exit(3);
    }

    //Client's message
    printf("Your message: "); //UI
    fgets(buffer,BUFSIZE,stdin); //Get input from standard input

    //Send a message
    message = send(network_socket, buffer, BUFSIZE, 0);
    if(message < 0) {
       fprintf(stderr, "Can't send messageLEL\n");
       exit(3);
    }
    
    //Recive message from the server
    //recv(network_socket, &server_response, sizeof(server_response), 0);
    recv(network_socket, &server_response, sizeof(server_response), 0);
    printf("The other client sent the following reaction: \n\n%s\n", server_response);

    recv(network_socket, &server_response, sizeof(server_response), 0);
    
    //print out the server response
    printf("The other client sent the following message: \n\n%s\n", server_response);
    
   
  }

     while(1){
  kliens2_kezdete://goto label

    if (strstr(server_response, "Hello Client1") != NULL){
    break;
    } 

    printf("Hello client 2!!!!!\n");
  
    recv(network_socket, &server_voteResponse, sizeof(server_voteResponse), 0);
    printf("The server sent the following message! :\n%s\n", server_voteResponse);
    fgets(voteResponseBuffer, BUFSIZE, stdin);
    voteMessage = send(network_socket, voteResponseBuffer, BUFSIZE, 0);
    recv(network_socket, &server_voteResponse, sizeof(server_voteResponse), 0);
    if (strstr(server_voteResponse, "The conversation is over!") != NULL){
      printf("%s",server_voteResponse);
      break;
      
    }
    else{
      printf("%s",server_voteResponse);
    }

    
    recv(network_socket, &server_response, sizeof(server_response), 0);
    printf("The other client sent the following reaction: \n\n%s\n", server_response);

    recv(network_socket, &server_response, sizeof(server_response), 0);
    
    //print out the server response
    printf("The other client sent the following message: \n\n%s\n", server_response);

    printf("Reagálás: ");
    fgets(reactionBuffer,REACTION_BUFSIZE,stdin);
    reactionMessage=send(network_socket, reactionBuffer, REACTION_BUFSIZE,0);
    if(message < 0) {
       fprintf(stderr, "Can't send message\n");
       exit(3);
    }

    //Client's message
    printf("Your message: "); //UI
    fgets(buffer,BUFSIZE,stdin); //Get input from standard input

    //Send a message
    message = send(network_socket, buffer, BUFSIZE, 0);
    if(message < 0) {
       fprintf(stderr, "Can't send message\n");
       exit(3);
    }
    printf("Eküldve!\n");
    
  }
  
  



  //close socket
  close(network_socket);
  return 0;
}