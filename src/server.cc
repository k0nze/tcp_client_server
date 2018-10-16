#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <errno.h>

#define MAX_CLIENTS 10

/**
 * @brief TCP Uses 2 types of sockets, the connection socket and the listen socket.
 * The Goal is to separate the connection phase from the data exchange phase.
 * @authors Suyash (https://github.com/suyash), Konstantin Luebeck (University of Tuebingen, Embedded Systems), Alexander Jung (University of Tuebingen, Embedded Systems)
 * Multiple client extension adapted from GeeksforGeeks:
 * (https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/)
 */

int main(int argc, char *argv[]) {

    int opt = 1;

    // port to start the server on
    int SERVER_PORT = 8877;

    int sd, max_sd, activity;

    // socket address used for the server
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;

    // htons: host to network short: transforms a value in host byte
    // ordering format to a short value in network byte ordering format
    server_address.sin_port = htons(SERVER_PORT);

    // htonl: host to network long: same as htons but to long
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    fd_set readfds;
    int client_socket[MAX_CLIENTS];

    for(int i = 0; i < MAX_CLIENTS; i++) {
        client_socket[i] = 0;
    }

    // create a TCP socket, creation returns -1 on failure
    int master_socket;
    if((master_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        printf("could not create listen socket\n");
        return 1;
    }

    if(setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR,
                  (char*)&opt, sizeof(opt)) < 0) {
        printf("Could not set sockopt\n");
        return 1;
    }

    // bind it to listen to the incoming connections on the created server
    // address, will return -1 on error
    if((bind(master_socket, (struct sockaddr *)&server_address,
              sizeof(server_address))) < 0) {
        printf("could not bind socket\n");
        return 1;
    }

    int wait_size = 16;  // maximum number of waiting clients, after which
                         // dropping begins
    if(listen(master_socket, wait_size) < 0) {
        printf("could not open socket for listening\n");
        return 1;
    }

    // socket address used to store client address
    struct sockaddr_in client_address;
    int client_address_len = sizeof(client_address);

    int sock;

    // run indefinitely
    while(true) {

        // clear socket set
        FD_ZERO(&readfds);

        // add master socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        // add client sockets to set
        for(int i = 0; i < MAX_CLIENTS; i++) {
            // socket descriptor
            sd = client_socket[i];

            // if valid sd then add to read list
            if(sd > 0) {
                FD_SET(sd, &readfds);
            }

            // highest file descriptor number, needed for select function
            if(sd > max_sd) {
                max_sd = sd;
            }
        }

        activity = select(max_sd+1, &readfds, NULL, NULL, NULL);

        if((activity < 0) && (errno!=EINTR)) {
            printf("Select error!\n");
        }

        if(FD_ISSET(master_socket, &readfds)) {
            // open a new socket to transmit data per connection

            if((sock = accept(master_socket, (struct sockaddr *)&client_address, (socklen_t*) &client_address_len)) < 0) {
                printf("could not open a socket to accept data\n");
                return 1;
            }
            printf("Client connected, ip: %s, port: %d\n",
                   inet_ntoa(client_address.sin_addr),
                   ntohs(client_address.sin_port));

            for(int i = 0; i < MAX_CLIENTS; i++) {
                if(client_socket[i] == 0) {
                    client_socket[i] = sock;
                    printf("Added to list of sockets at position %d\n", i);
                    break;
                }
            }
        }

        for(int i = 0; i < MAX_CLIENTS; i++) {
            sd = client_socket[i];

            int n = 0;
            int len = 0, maxlen = 100;
            char buffer[maxlen];
            char *pbuffer = buffer;

            if(FD_ISSET(sd, &readfds)) {
                if((n = recv(sd, pbuffer, maxlen, 0)) == 0) {

                    //Somebody disconnected , get his details and print
                    getpeername(sd , (struct sockaddr*)&client_address, (socklen_t*)&client_address_len);
                    printf("Client disconnected, ip %s, port %d\n",
                           inet_ntoa(client_address.sin_addr),
                           ntohs(client_address.sin_port));

                    //Close the socket and mark as 0 in list for reuse
                    close(sd);
                    client_socket[i] = 0;

                } else {
                    pbuffer += n;
                    maxlen -= n;
                    len += n;

                    buffer[n] = '\0';
                    printf("received: '%s'\n", buffer);

                    std::string bounce = "Bounce message from client: ";
                    bounce += buffer;

                    // echo received content back
                    send(sd, bounce.c_str(), strlen(bounce.c_str()), 0);
                    // send(sock, data_to_return, strlen(data_to_return), 0);
                }
            }
        }

    }

    close(master_socket);
    return 0;
}
