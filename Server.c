#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define BUFSIZE 1025
#define PORT_NO 2001

int main(int argc, char *argv[] ){

  printf("\e[1;1H\e[2J"); //clears the screen on linux

  int server_socket;    //file leirok
  int client_socket;
  int client_socket2;
  

  char on = 1;
  char server_message[BUFSIZE] = "Hello Client1";
  char server_message2[BUFSIZE] = "Hello Client2";
  char server_langChoose[BUFSIZE]= "Choose language: magyar or MAGYAR?";
  char client_response[BUFSIZE];
  char client_response2[BUFSIZE];
  char client_reactionBuffer[BUFSIZE];
  char client_reactionBuffer2[BUFSIZE];
  char client_langResponse[BUFSIZE];
  char client_langResponse2[BUFSIZE];
  char vote_message[BUFSIZE] = "Please vote: 'igen', 'nem', 'tartozkodom' !";
  char vote_response[BUFSIZE];
  char vote_response2[BUFSIZE];
  int message;



  //specify the server adress
  struct sockaddr_in server_adress;  //socket name (addr) of server
  struct sockaddr_in client;         //socket name of client
  struct sockaddr_in client2;        //socekt name of client
  server_adress.sin_family = AF_INET;  
  server_adress.sin_port = htons(PORT_NO);    
  server_adress.sin_addr.s_addr = inet_addr("127.0.0.1");

  int server_size = sizeof server_adress;
  int client_size = sizeof client_socket;
  int client_size2 = sizeof client_socket2;

  //create a socket
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0) {
     fprintf(stderr, "Can't create to the socket\n");
     exit(1);
  }

  //UI
  printf("Server initialized on port %i\n",PORT_NO);

  setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);

  //bind the socket to our specified IP and port
  bind(server_socket, (struct sockaddr *) &server_adress, sizeof(server_adress));
  if (server_socket < 0) {
     fprintf(stderr, "Can't bind to the socket\n");
     exit(2);
  }
  listen(server_socket, 10);
  if (server_socket < 0) {
     fprintf(stderr, "Can't listen to the socket\n");
     exit(3);
  }


  //Accept the first client
  client_socket = accept(server_socket, (struct sockaddr *) &client, &client_size);
  if (server_socket < 0) {
     fprintf(stderr, "Can't accept socket\n");
     exit(4);
  }
  //send the greating message
  send(client_socket, server_message, sizeof(server_message), 0);
  printf("Greating sent to client1\n"); //UI
  //send the question message
  send(client_socket, server_langChoose, sizeof(server_langChoose), 0);
  printf("Question sent to Client1\n");
  //recieve the answear
  recv(client_socket, client_langResponse, sizeof(client_langResponse),0);


  //accept the second client
  client_socket2 = accept(server_socket, (struct sockaddr *) &client2, &client_size2);
  if (server_socket < 0) {
     fprintf(stderr, "Can't accept socket\n");
     exit(4);
  }
  //send the greating message
  send(client_socket2, server_message2, sizeof(server_message2), 0);
  printf("Greating sent to Client2\n"); //UI
  //send the question message
  send(client_socket2, server_langChoose, sizeof(server_langChoose), 0);
  printf("Question sent to Client2\n");
  //recieve the answear
  recv(client_socket2, client_langResponse2, sizeof(client_langResponse2),0);

  if((strstr(client_langResponse, "magyar")!= NULL) ){
        printf("Client1 choosed: magyar\n" );

      }else if((strstr(client_langResponse, "MAGYAR")!= NULL)){
        printf("Client1 choosed: MAGYAR\n");

        }
        if((strstr(client_langResponse2, "magyar") != NULL)){
          printf("Client2 choosed: magyar\n");
        }else{
          printf("Client2 choosed: MAGYAR\n");
        }
  

  while(1){

    //send the vote massage to the clients
    send(client_socket, vote_message, sizeof(vote_message), 0);
    printf("Vote message sent to Client1!\n");
    send(client_socket2, vote_message, sizeof(vote_message), 0);
    printf("Vote message sent to Client2!\n");
    //recieve the answear
    recv(client_socket, vote_response, sizeof(vote_response), 0);
    recv(client_socket2, vote_response2, sizeof(vote_response2), 0);

    if((strstr(vote_response, "igen") != NULL) && (strstr(vote_response2, "igen") != NULL)){
      printf("Everybody voted 'igen', the conversation is over!\n\n");


      message = send(client_socket2, "The conversation is over!\n", BUFSIZE, 0);
      if(message < 0) {
         fprintf(stderr, "Can't send message\n");
         exit(5);
      }
      message = send(client_socket, "The conversation is over!\n", BUFSIZE, 0);
      if(message < 0) {
         fprintf(stderr, "Can't send message\n");
         exit(5);
      }
      break;

    }else if((strstr(vote_response, "nem") != NULL) && (strstr(vote_response2, "nem") != NULL)){
      printf("Nobody wants to finish the conversation, so the conversation continues!\n\n");

      message = send(client_socket2, "The conversation continues!\n", BUFSIZE, 0);
      if(message < 0) {
         fprintf(stderr, "Can't send message\n");
         exit(5);
      }
      message = send(client_socket, "The conversation continues!\n", BUFSIZE, 0);
      if(message < 0) {
         fprintf(stderr, "Can't send message\n");
         exit(5);
      }
      
    }else if((strstr(vote_response, "tartozkodom") != NULL) && (strstr(vote_response2, "tartozkodom") != NULL)){
      printf("Nobody has decision, so the conversation continues!\n");

      message = send(client_socket2, "The conversation continues!\n", BUFSIZE, 0);
      if(message < 0) {
         fprintf(stderr, "Can't send message\n");
         exit(5);
      }
      message = send(client_socket, "The conversation continues!\n", BUFSIZE, 0);
      if(message < 0) {
         fprintf(stderr, "Can't send message\n");
         exit(5);
      }
    }else{
      printf("No decision was made, the conversation continues!\n");
      message = send(client_socket2, "The conversation continues!\n", BUFSIZE, 0);
      if(message < 0) {
         fprintf(stderr, "Can't send message\n");
         exit(5);
      }
      message = send(client_socket, "The conversation continues!\n", BUFSIZE, 0);
      if(message < 0) {
         fprintf(stderr, "Can't send message\n");
         exit(5);
      }
    }

   

    //Recive greating message and/or reaction from Client1

    recv(client_socket, &client_reactionBuffer, sizeof(client_reactionBuffer), 0);

    printf("Client1 sent the following reaction:\n%s\n", client_reactionBuffer);

    recv(client_socket, &client_response, sizeof(client_response), 0);

    printf("Client1 sent the following message:\n%s\n", client_response);

  //"translation"
  
  if((strstr(client_langResponse, "magyar") != NULL)){
    int i = 0;
    while(client_reactionBuffer[i]){
      client_reactionBuffer[i] = toupper(client_reactionBuffer[i]);
      i++;
    }
  }
  else{
    int i = 0;
    while(client_reactionBuffer[i]){
      client_reactionBuffer[i] = tolower(client_reactionBuffer[i]);
      i++;
    }
    
  }

  if((strstr(client_langResponse, "magyar") != NULL)){
    int i = 0;
    while(client_response[i]){
      client_response[i] = toupper(client_response[i]);
      i++;
    }
  }
  else{
    int i = 0;
    while(client_response[i]){
      client_response[i] = tolower(client_response[i]);
      i++;
    }
    
  }
    

    //Send a message and/or reaction to Client2
    message = send(client_socket2, client_reactionBuffer, BUFSIZE, 0);
    if(message < 0) {
       fprintf(stderr, "Can't send reaction\n");
       exit(5);
    }
    message = send(client_socket2, client_response , BUFSIZE, 0);
    if(message < 0) {
       fprintf(stderr, "Can't send message\n");
       exit(5);
    }
  
    //Recive greating message and reaction from Client2
    
    recv(client_socket2, &client_reactionBuffer2, sizeof(client_reactionBuffer2), 0);

    printf("Client2 sent the following reaction:\n%s\n", client_reactionBuffer2);
    //Recive greating message from Client2
    recv(client_socket2, &client_response2, sizeof(client_response2), 0);

    printf("Client2 sent the following message:\n%s\n", client_response2);

    //"translation"

    if((strstr(client_langResponse2, "magyar") != NULL)){
    int i = 0;
    while(client_reactionBuffer2[i]){
      client_reactionBuffer2[i] = toupper(client_reactionBuffer2[i]);
      i++;
    }
    }
    else{
     int i = 0;
     while(client_reactionBuffer2[i]){
       client_reactionBuffer2[i] = tolower(client_reactionBuffer2[i]);
       i++;
    }
    
    }

    if((strstr(client_langResponse2, "magyar") != NULL)){
      int i = 0;
      while(client_response2[i]){
        client_response2[i] = toupper(client_response2[i]);
        i++;
      }
    }
    else{
     int i = 0;
     while(client_response2[i]){
       client_response2[i] = tolower(client_response2[i]);
       i++;
    }
    
    }


    //Send a message and reaction to Client1
    message = send(client_socket, client_reactionBuffer2 , BUFSIZE, 0);
    if(message < 0) {
       fprintf(stderr, "Can't send reaction and/or message\n");
       exit(5);
    }
     message = send(client_socket, client_response2 , BUFSIZE, 0);
    if(message < 0) {
       fprintf(stderr, "Can't send reaction and/or message\n");
       exit(5);
    }
    
  }

  //close sockets
  close(server_socket);
  close(client_socket);
  close(client_socket2);

  return 0;
}
