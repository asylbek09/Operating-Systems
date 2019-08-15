#include <string.h>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <strings.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <vector>
using namespace std;

static const int rowMax = 15;
static const int colMax = 6;

int main(int argc, char *argv[])
{
    int listenFd, portNo, timeOut;
    bool loop = false;
    struct sockaddr_in svrAdd;
    struct hostent *server;

    FILE *iniFile;
    size_t length = 0;
    iniFile = fopen(argv[1], "r");
    char *line, *iniToken;
    const char *ipAddress;

    if (iniFile != NULL)
    {
        getline(&line, &length, iniFile);
        fclose(iniFile);

        const char space[2] = " ";
        strtok(line, space);
        iniToken = strtok(NULL, space);
        portNo = atoi(iniToken);
        strtok(NULL, space); // consume "ip:"
        ipAddress = strtok(NULL, space);
        strtok(NULL, space); // consume "timeout(sec):"
        iniToken = strtok(NULL, space);
        timeOut = atoi(iniToken); // get timeout in ms
    }
    else
    {
        cout << "Could not load file from supplied argument" << endl;
        cout << "Initializing with default settings" << endl;
        portNo = 8080;
        ipAddress = "127.0.0.1";
        timeOut = 5; // 5 attempts
    }

    cout << "Starting new client" << endl;
    cout << "Listening to \nPort: " << portNo << " IP: " << ipAddress << " timeout: " << timeOut << endl;

    // create client socket
    listenFd = socket(AF_INET, SOCK_STREAM, 0);

    if (listenFd < 0)
    {
        cerr << "Cannot open socket" << endl;
        return 0;
    }

    server = gethostbyname(ipAddress);

    if (server == NULL)
    {
        cerr << "Host does not exist" << endl;
        return 0;
    }

    bzero((char *)&svrAdd, sizeof(svrAdd));

    svrAdd.sin_family = AF_INET;

    bcopy((char *)server->h_addr, (char *)&svrAdd.sin_addr.s_addr, server->h_length);

    svrAdd.sin_port = htons(portNo);

    int checker = connect(listenFd, (struct sockaddr *)&svrAdd, sizeof(svrAdd));
    int ctr = 0;
    while (checker != 0 && ctr < timeOut)
    {
        cout << "Attempting to connect to server ..." << endl;
        checker = connect(listenFd, (struct sockaddr *)&svrAdd, sizeof(svrAdd));
        ctr++;
        sleep(5);
    }
    if (checker < 0)
    {
        cerr << "Failed to connect to server!" << endl;
        return 0;
    }

    // send stuff to server
    for (;;)
    {
        char seatMap[1000];
        bzero(seatMap, 1000);
        read(listenFd, seatMap, 1000);
        int choose = 0;
        int row, column;

        cout << "\nHow do you want to choose a seat? \nEnter" << endl;
        cout << "1 for Manual \n0 for Random \n-1 to Exit reservation" << endl;
        cin >> choose;
        if (choose == -1) {
            break;
        } else {
            while (choose != 1 && choose != 0) {
                cout << "1 for Manual \n0 for Random \n-1 to Exit reservation" << endl;
                cin >> choose;
                if (choose == -1) {
                    break;
                }
            }   
        }

        cout << "\nCurrent Seat Map:\n" << endl;
        cout << seatMap << endl;
        cout << "*****************************************" << endl;
        if (choose == 1) {
            cout << "Enter the row number you want to sit: " << endl;
            cin >> row;

            cout << "Enter the column number you want to sit: " << endl;
            cin >> column;
        } else {
            row = rand() % rowMax + 1;
            column = rand() % colMax + 1;
        }

        string reservationRequest;
        reservationRequest.append(to_string(row)).append("&").append(to_string(column));
        write(listenFd, reservationRequest.c_str(), reservationRequest.length());

        char reservationResponse[2];
        bzero(reservationResponse, 1);
        read(listenFd, reservationResponse, 1);
        if (reservationResponse[0] == '1')
        {
            cout << "Reservation successful!" << endl;
        }
        else
        {
            cout << "Reservation failed!" << endl;
        }
    }
}