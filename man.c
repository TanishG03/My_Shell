

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "headers.h"

#define MAX_BUFFER_SIZE 4096



// Function to extract and print the text content within HTML tags
void printTextContent(const char *html) {
    int inTag = 0;
    for (int i = 0; html[i]; i++) {
        if (html[i] == '<') {
            // Entering an HTML tag, set inTag to 1
            inTag = 1;
        } else if (html[i] == '>') {
            // Exiting an HTML tag, set inTag to 0
            inTag = 0;
        } else if (!inTag) {
            // Print the character if not inside an HTML tag
            putchar(html[i]);
        }
    }
}


int man(char * token) {
    int argc=0;
    char * args[100];
    // printf("%s\n",token);
    args[argc]=token;
    argc++;

    while ((token = strtok(NULL, " \t\n")) != NULL) {
                    args[argc] = token;
                    argc++;
    }

    if (argc != 2) {
        fprintf(stderr, "Not Valid Arguements\n");
        return 0;
    }
    // printf("%s\n",args[1]);
    char *command_name = args[1];
    struct hostent *server;
    struct sockaddr_in server_addr;
    int sockfd, portno = 80;

    // DNS resolution
    server = gethostbyname("man.he.net");
    if (server == NULL) {
        fprintf(stderr, "Error: Could not resolve host\n");
        exit(1);
    }

    // Initialize socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        return 0;
    }

    // Set up server address
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portno);
    bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error connecting");
        return 0;
    }

    // Send GET request
    char request[MAX_BUFFER_SIZE];
    snprintf(request, sizeof(request), "GET /?topic=%s&section=all HTTP/1.1\r\nHost: man.he.net\r\n\r\n", command_name);

    if (write(sockfd, request, strlen(request)) < 0) {
        perror("Error writing to socket");
        return 0;
    }

    // Read and print response
    char buffer[MAX_BUFFER_SIZE];
    int bytes_read;
    int line_count = 0;

     while ((bytes_read = read(sockfd, buffer, sizeof(buffer))) > 0) {
        printTextContent(buffer);

        // Check for "No matches" in any line
        if (strstr(buffer, "No matches") != NULL) {
            fprintf(stderr, "ERROR: No matches for \"%s\" command\n", command_name);
            close(sockfd);
            return 0;
        }
    }
    if (bytes_read < 0) {
        perror("Error reading from socket");
        return 0;
    }

    close(sockfd);

    return 1;
}
