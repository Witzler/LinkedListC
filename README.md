This is a small implementation of a couple of tasks in C.  
Firstly there is a double linked list, which stores integers.  
Secondly we got some code processes, including forks, wait, exec, redirection and pipes.  
Thirdly there is a programm redirecting an incoming process with (0,*) parameters.  
The files are firstTask, factiwait1, redirect.c and factiwait1.  
An example usage of the first task is:   
gcc -std=c99 -Wall -pedantic -o firstTask firstTask.c  
./firstTask  
Use Control+C to stop the program in Linux or:  
Eingabe: 1 3 2  
Ausgabe: 1 2 3  
Eingabe: 1 4  
Ausgabe: 1 2 3 4  
Eingabe: 10 9 1 8  
Ausgabe: 1 2 3 4 8 9 10  
Eingabe: 7foo 7 bar7  
Ausgabe: 1 2 3 4 7 8 9 10  
Eingabe: foo bar  
Ausgabe: 1 2 3 4 7 8 9 10  
Eingabe: ^C  
Cleaning up memory  
  
Input: 1 3 2  
Output: 1 2 3  
Input: 1 4  
Output: 1 2 3 4  
Input: 10 9 1 8  
Output: 1 2 3 4 8 9 10  
Input: 7foo 7 bar7  
Output: 1 2 3 4 7 8 9 10  
Input: foo bar  
Output: 1 2 3 4 7 8 9 10  
Input: ^C  
Cleaning up memory  
  
  
factiwait will get receive programs and split them up for two forked child processes. The parent waits until both are done.  
redirect will redirect a process with its parameters to a different file if wished. "-" means the default out/exit shall be used.  
Instructions for the factiwait and redirect are commented
on the top of the file.  
The redirect commands will place files in tmp file outside of the project.  
