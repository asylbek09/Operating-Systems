/*
Asylbek Dzhapashov
CS470
Lab2
01/29/2019
*/
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <string>
#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

vector<int> merge_sort(const vector<int> &input)
{
    int pipe1[2], pipe2[2];
    pid_t child1Pid, child2Pid;

    if (input.size() <= 1)
        return input;
    vector<int> output(input.size());

    //Split Vector//
    int midpoint = 0.5 * input.size();
    vector<int> input_left(input.begin(), input.begin() + midpoint);
    vector<int> input_right(input.begin() + midpoint, input.end());
    vector<int> received_input_left;
    vector<int> received_input_right;

    if (pipe(pipe1) == -1)
    {
        perror("pipe1");
        exit(EXIT_FAILURE);
    }

    if (pipe(pipe2) == -1)
    {
        perror("pipe2");
        exit(EXIT_FAILURE);
    }

    child1Pid = fork();
    if (child1Pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (child1Pid == 0) //Child process
    {
        cout << "       left child PID: " << getpid() << endl;
        input_left = merge_sort(input_left);
        int send_array[input_left.size()];
        for (int i = 0; i < input_left.size(); i++){
            send_array[i] = input_left.at(i);
            cout << "       left: " << input_left.at(i) << endl;
        }
        
        write(pipe1[1], send_array, sizeof(send_array));
        exit(EXIT_SUCCESS);
    }
    else // Parent process
    {
        int received_arr[input_left.size()];
        wait(NULL);
        read(pipe1[0], &received_arr, sizeof(received_arr));
        for (int i = 0; i < input_left.size(); i++)
        {
            received_input_left.push_back(received_arr[i]);
        }
    }

    child2Pid = fork();
    if (child2Pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (child2Pid == 0) //Child process
    {
        cout << "       right child PID: " << getpid() << endl;
        input_right = merge_sort(input_right);
        int send_array[input_right.size()];
        for (int i = 0; i < input_right.size(); i++){
            send_array[i] = input_right.at(i);
            cout << "       right: " << input_left.at(i) << endl;
        }

        write(pipe2[1], send_array, sizeof(send_array));
        exit(EXIT_SUCCESS);
    }
    else // Parent process
    {
        int received_arr[input_right.size()];
        wait(NULL);
        read(pipe2[0], &received_arr, sizeof(received_arr));
        for (int i = 0; i < input_right.size(); i++)
        {
            received_input_right.push_back(received_arr[i]);
        }
    }
    
    merge(received_input_left.begin(), received_input_left.end(), received_input_right.begin(), received_input_right.end(), output.begin());

    return output;
}

vector<int> readFile(string fileName, char separator)
{
    string line;
    ifstream myfile(fileName);
    if (!myfile.is_open())
    {
        exit(1);
    }

    if (myfile.good())
    {
        getline(myfile, line);
    }
    cout << " File contents:  " << line << endl;
    cout << " ***************************************" << endl;
    myfile.close();

    vector<int> list;

    int j = 0;

    // Traverse the string
    for (int i = 0; line[i] != '\0'; i++)
    {
        if (line[i] == separator)
        {
            list.push_back(j);
            j = 0;
        }
        else
        {
            j = j * 10 + (line[i] - 48);
        }
    }

    list.push_back(j);

    return list;
}

int main(int argc, char *argv[])
{
    //string delimiter = argv[2];
    char delimiter[5];
    strcpy(delimiter, argv[2]);
    //char delim = delimiter[0];
    //cout << "delimiter position: " << delimiter << endl;
    
    cout << "\n Input file:  " << argv[1] << endl;
    cout << " ***************************************" << endl;
    vector<int> list = readFile(argv[1], delimiter[0]);

    cout << " Delimiter:  ( " << delimiter << " )" << endl;
    cout << " ***************************************" << endl;
    cout << " Initial array:  ";
    for (int i = 0; i < list.size(); i++)
    {
        cout << list.at(i) << " ";
    }
    cout << endl;
    cout << " ***************************************" << endl;

    cout << " Intermediate pid's of children \n while performing sorting .... \n" << endl;
    vector<int> sortedList = merge_sort(list);

    cout << "\n ***************************************" << endl;
    cout << " Sorted array:  ";
    for (int i = 0; i < sortedList.size(); i++)
    {
        cout << sortedList.at(i) << " ";
    }
    cout << "\n" << endl;

    return 1;
}