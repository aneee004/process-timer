
CONTENTS OF THIS FILE
---------------------
 * Overview
 * System requirements
 * Running the project
 
OVERVIEW
------------
The aim of this project is to develop a program that can run Linux/UNIX based
CLI commands, and display the elapsed time for running the command.

SYSTEM REQUIREMENTS
------------
 * Linux operating system with kernel v4.4.0 or above
   Hint: To check your kernel version run:
   $ uname -srm
 * gcc compiler v7.5.0 or above

RUNNING THE PROJECT
----------------
 * Compile the project with the following command.
   $ gcc Asgn1-me17btech11007.c -o time -lrt
 * Execute your command as illustrated below.
   $ ./time <command>
   Example:
   * $ ./time mkdir dev
   * $ ./time ls
   * $ ./time ls -ll
