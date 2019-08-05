/*
Asylbek Dzhapashov
CS470
Lab1
01/15/2019
*/

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

void help() {
    cout << "\t\n * HELP PAGE * \n\n";
    cout << " * prompt:\t" << "Enter <prompt <name>> to see new Shell name\n" << endl;
    cout << " * cpuinfo:\t" << "Enter <cpuinfo -c> to see CPU Clock Speed\n";
    cout << " * cpuinfo:\t" << "Enter <cpuinfo -t> to see CPU Type\n";
    cout << " * cpuinfo:\t" << "Enter <cpuinfo -n> to see Number of Cores in CPU\n\n";
    cout << " * meminfo:\t" << "Enter <meminfo -t> to see Total RAM in the system\n"; 
    cout << " * meminfo:\t" << "Enter <meminfo -u> to see Used RAM in the system\n";
    cout << " * meminfo:\t" << "Enter <meminfo -c> to see Size of L2 Cache\n\n";
    cout << " * exit:\t" << "Enter <exit> to quit from shell or <exit -h> for Help Page\n\n";
    cout << " * system:\t" << "Enter <____> Linux Systems Calls\n\n";
}

int main() {

    string command;
    string name = "#cwushell~>";

    cout <<"\n * CWU Mini-Shell * \n\n * by Asylbek Dzhapashov * \n\n";

    while (command != "exit") {
        if (command == "prompt" || command == "help" || command == "cpuinfo" || command == "meminfo" || command == "man") {
            help();
        }
        else if (command.compare("exit -h") == 0) {
            help();
        }
        else if (command.compare("prompt") == 0) {
            name;
        }
        else if (strstr(command.c_str(), "prompt") != NULL) {
            name = "#" + command.erase(0, 7) + "~>"; 
        }
        // CPU Clock speed 
        else if (command.compare("cpuinfo -c") == 0) {
            system("lscpu | grep 'MHz'");
        }
        // CPU Type
        else if (command == "cpuinfo -t") {
            system("lscpu | grep 'name'");
        }
        // CPU number of cores
        else if (command == "cpuinfo -n") {
            system("cat /proc/cpuinfo | grep 'cores'");
        }
        // Total RAM in the system
        else if (command == "meminfo -t") {
            system("grep MemTotal /proc/meminfo | awk '{print $2 / 1024*1024*1024}'");
        }
        // Used RAM
        else if (command == "meminfo -u") {
            system("cat /proc/meminfo | grep 'MemAvailable'");
        }
        // Size of L2  cache/core in bytes
        else if (command == "meminfo -c") {
            system("lscpu | grep 'L2'");
        }
        else {
           system(command.c_str());
        }

        cout << name;
        getline(cin, command);
    }
    return 0;
}