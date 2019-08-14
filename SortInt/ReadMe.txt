Asylbek Dzhapashov
CS470
Lab2
01/29/2019

C++ program under Linux OS to sort integer numbers using fork() and pipe().

To run this program 
    type: 
        1. g++ -o lab2 lab2.cpp
        // command line accepts three arguments as:
        2. <./lab2> <fileName.txt> "<delimiter>"
        //where delimiters are: ( , ), ( ; ), ( - ), ( space ), ( char )

    Program takes a file as an input and displays the integers as string with delimiters.

    Then it converts string into vector and displays it.
    
    While performing Sorting using fork() it diplays pid's and intermediate results of children.
    
    Then it merges to parent with pipe(). 

    Finally it displays sorted vector. 
