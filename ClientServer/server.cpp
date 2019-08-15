#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <strings.h>
#include <stdlib.h>
#include <string>
#include <pthread.h>
#include <cctype>
#include <iomanip>
using namespace std;

void *air_server(void *);

static const int rowNumber = 15;
static const int colNumber = 6;

static char seatMap[rowNumber][colNumber];
void initialize();
string getSeatMapDisplay();

pthread_mutex_t lockthread = PTHREAD_MUTEX_INITIALIZER;

static const int maxClientNumber = 3;
static int connFd[maxClientNumber];

int main(int argc, char *argv[])
{
    initialize();

    int pId, portNo, listenFd;
    socklen_t len; // store size of the address
    bool loop = false;
    struct sockaddr_in svrAdd, clntAdd;

    pthread_t threadA[3];

    if (argc < 2)
    {
        cerr << "Enter: ./server <port>" << endl;
        return 0;
    }

    portNo = atoi(argv[1]);

    if ((portNo > 65535) || (portNo < 2000))
    {
        cerr << "Please enter a port number between 2000 - 65535" << endl;
        return 0;
    }

    // create socket
    listenFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (listenFd < 0)
    {
        cerr << "Cannot open socket" << endl;
        return 0;
    }
    // initialize socket structure
    bzero((char *)&svrAdd, sizeof(svrAdd));

    svrAdd.sin_family = AF_INET;
    svrAdd.sin_addr.s_addr = INADDR_ANY;
    svrAdd.sin_port = htons(portNo);

    // bind socket
    if (bind(listenFd, (struct sockaddr *)&svrAdd, sizeof(svrAdd)) < 0)
    {
        cerr << "Cannot bind" << endl;
        return 0;
    }

    listen(listenFd, maxClientNumber);

    int noThread = 0;

    cout << "Socket created\n" << endl;
    cout << "Server waiting for client to connect ...\n" << endl;
    while (noThread < maxClientNumber)  
    {
        socklen_t len = sizeof(clntAdd);
        //cout << "Waiting for client to connect ...\n" << endl;

        // this is where client connects. svr will hang in this mode until client conn
        connFd[noThread] = accept(listenFd, (struct sockaddr *)&clntAdd, &len);

        if (connFd[noThread] < 0)
        {
            cerr << "Cannot accept connection" << endl;
            return 0;
        }
        else
        {
            cout << "Connection successful\n" << endl;
        }

        int *arg = new int[1];
        *arg = noThread;

        pthread_create(&threadA[noThread], NULL, air_server, arg);

        noThread++;
    }

    for (int i = 0; i < 3; i++)
    {
        pthread_join(threadA[i], NULL);
    }
}

void *air_server(void *i)
{
    int index = *((int *)i);
    pthread_mutex_t lockthread;
    pthread_mutex_lock(&lockthread);

    cout << "*****************************************" << endl;
    cout << "Thread No: " << pthread_self() << endl;
    //cout << "Client No: " << index << endl;
    
    bool loop = false;
    while (!loop)
    {
        string display = getSeatMapDisplay();
        // send seat map
        write(connFd[index], display.c_str(), display.length());

        // get reservation request
        char request[300];
        bzero(request, 300);
        read(connFd[index], request, 300);

        string requestString(request);
        cout << "Client No: " << index << endl;
        // cout << "Received Reservation Request: " << requestString << endl;

        if (requestString == "exit")
        {
            break;
        }

        bool clientResponse = false;
        // TODO: handle client request
        try
        {
            cout << "Received Reservation Request On " << endl;
            int splitIndex = requestString.find("&");
            int row = stoi(requestString.substr(0, splitIndex));
            cout << "Row: " << row << endl;
            int col = stoi(requestString.substr(splitIndex + 1, requestString.length()));
            cout << "Column: " << col << endl;
            cout << "-----------------------------------------" << endl;

            if (row >= 0 && row < rowNumber && col >= 0 && col < colNumber)
            {
                if (seatMap[row][col] == '0')
                {
                    seatMap[row][col] = '1';
                    clientResponse = true;
                }
            }
        }
        catch (exception e)
        {
            // something went wrong, it should return "not reserved back to client"
            cout << "Client Closed the Connection" << endl;
        }

        // send reservation response
        write(connFd[index], clientResponse ? "1" : "0", 1);
    }

    cout << "\nClosing thread and connection" << endl;
    pthread_mutex_unlock(&lockthread);
    close(connFd[index]);
    return 0;
}

void initialize()
{
    for (int i = 0; i < rowNumber; i++)
        for (int j = 0; j < colNumber; j++)
            seatMap[i][j] = '0';
}

string getSeatMapDisplay()
{
    string seatMapDisplay = "  ";
    for (int j = 0; j < colNumber; j++)
    {
        seatMapDisplay.append("  ");
        seatMapDisplay.append(to_string(j));
    }
    seatMapDisplay += "\n";

    for (int i = 0; i < rowNumber; i++)
    {
        seatMapDisplay.append(to_string(i));
        if (i < 10)
        {
            seatMapDisplay.append(" ");
        }

        for (int j = 0; j < colNumber; j++)
        {
            seatMapDisplay.append("  ");
            seatMapDisplay += seatMap[i][j];
        }

        seatMapDisplay.append("\n");
    }

    // cout << seatMapDisplay << endl;

    return seatMapDisplay;
}