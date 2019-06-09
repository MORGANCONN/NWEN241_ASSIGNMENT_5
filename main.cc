/**
 * t5test.cc
 * Sample test program for Task 5
 * This test assumes that you have completed implementation of VectorDbTable in Task 4.
 *
 * To compile with your implementation:
 * g++ main.cc dbms2.cc vdb.cc -o t5test
 *
 * If successful, executable file t5test should have been
 * created.
 */

#include <iostream>
#include "vdb.hh"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sstream>

using namespace std;
using namespace dbms2;


int main(void) {
    VectorDbTable *db;
    bool r;

    cout << "Instantiating VectorDbTable..." << endl;
    db = new VectorDbTable();
    cout << "Invoking loadCSV(\"scifi.csv\")..." << endl;
    r = db->loadCSV("scifi.csv");
    cout << "Expected return value: 1" << endl;
    cout << "Actual return value  : " << r << endl;
    cout << "Expected : rows = 25" << endl;
    cout << "Actual   : rows = " << db->rows() << endl;

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        printf("Error creating new socket");
        exit(0);
    }
    struct sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(50000);
    // Bind socket
    if (bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        printf("Error binding socket");
        exit(0);
    }
    // Begin To listen
    if (listen(serverSocket, SOMAXCONN)) {
        printf("Error listening for connections");
        exit(0);
    }
    while(true) {
        // Accept Client
        struct sockaddr_in client_addr = sockaddr_in();
        int addrlen = sizeof(client_addr);
        int clientSocket = accept(serverSocket, (struct sockaddr *) &client_addr, (socklen_t *) &addrlen);
        if (clientSocket < 0) {
            printf("Error accepting connection");
            exit(0);
        } else{
            char msg[] = "Hello Movie Goer!\n";
            ssize_t sendStatus = send(clientSocket, msg, strlen(msg), 0);
            if (sendStatus < 0) {
                printf("Error sending message");
                close(clientSocket);
                exit(0);
            }

        }
        char incoming[100];
        while (true) {
            memset(incoming, 0, 100);
            ssize_t incomingClient = recv(clientSocket, incoming, 100, 0);
            if (incomingClient <= 0) {
                printf("Error receiving message");
                close(clientSocket);
                exit(0);
            }
            // Disconnects The Client And Closes The Server
            if (strcasecmp(incoming, "BYE") == 0 || strcasecmp(incoming, "BYE\n") == 0) {
                char msg[] = "Bye Movie Goer!";
                ssize_t sendStatus = send(clientSocket, msg, strlen(msg), 0);
                if (sendStatus < 0) {
                    printf("Error sending message");
                    close(clientSocket);
                    exit(0);
                }
                close(serverSocket);
                close(clientSocket);
                break;
                // Prints Entire Database
            } else if (strcasecmp(incoming, "GET") == 0 || strcasecmp(incoming, "GET\n") == 0) {
                for (int i = 0; i < db->rows(); i++) {
                    stringstream textBuffer;
                    textBuffer << "Rank: " << i + 1 << " Film Title: " << db->get(i)->title << " Director Name: "
                               << db->get(i)->director
                               << " Date Of Release: " << db->get(i)->year << " Number Of Oscars: "
                               << db->get(i)->numberOfOscars << " Country Of Production: " << db->get(i)->country
                               << endl;
                    ssize_t sendStatus = send(clientSocket, textBuffer.str().c_str(), strlen(textBuffer.str().c_str()),
                                              0);
                    if (sendStatus < 0) {
                        printf("Error sending message");
                        close(clientSocket);
                        exit(0);
                    }
                }
            } else {
                vector<char *> splitIncoming;
                // Splits Incoming Stream
                char *chars_array = strtok(incoming, " ");
                while (chars_array) {
                    splitIncoming.push_back(chars_array);
                    chars_array = strtok(nullptr, " ");
                }
                if (strcasecmp(splitIncoming.at(0), "GET") == 0 || strcasecmp(splitIncoming.at(0), "GET\n") == 0) {
                    // Converts Expected Location Of Rank To Useble Format
                    stringstream filmId(splitIncoming.at(1));
                    int id = 0;
                    filmId >> id;
                    stringstream textBuffer;
                    // Determines If Supplied ID is valid
                    if (id > 0) {
                        textBuffer << "Rank: " << id << " Film Title: " << db->get(id - 1)->title << " Director Name: "
                                   << db->get(id - 1)->director
                                   << " Date Of Release: " << db->get(id - 1)->year << " Number Of Oscars: "
                                   << db->get(id - 1)->numberOfOscars << " Country Of Production: "
                                   << db->get(id - 1)->country
                                   << endl;
                    } else {
                        textBuffer << "Invalid Film Rank" << endl;
                    }
                    ssize_t sendStatus = send(clientSocket, textBuffer.str().c_str(), strlen(textBuffer.str().c_str()),
                                              0);
                    if (sendStatus < 0) {
                        printf("Error sending message");
                        close(clientSocket);
                        exit(0);
                    }
                } else {
                    stringstream textBuffer;
                    textBuffer << "Invalid Command" << endl << "Valid Commands Are:" << endl
                               << "GET | Returns The Greatest 25 Science Fiction Films Of All Time" << endl
                               << "GET rank | Returns The Film At The Requested Rank" << endl
                               << "BYE | Terminates Connection To The Database" << endl;
                    ssize_t sendStatus = send(clientSocket, textBuffer.str().c_str(), strlen(textBuffer.str().c_str()),
                                              0);
                    if (sendStatus < 0) {
                        printf("Error sending message");
                        close(clientSocket);
                        exit(0);
                    }
                }
            }
        }
    }
}
