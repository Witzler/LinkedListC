This is a small implementation of a double linked list in C, which stores integers.  
The real file is firstTask.c  
Use:   
gcc -std=c99 -Wall -pedantic -o firstTask firstTask.c  
./firstTask  
Use Control+C to stop the program in Linux.  
  
  
Example:  
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
==================  
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
==================  
Cleaning up memory  

