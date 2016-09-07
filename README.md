# Pipe Reading and Writing in C
# Created by Dominick Forlenza

----Basic Pipe reading and writing in C--------

|--Checker.c--|

   --Checker is responsible for checking whether the divisor and dividend supplied to it by
	the Coordinator through execlp() are divisible. The checker will print whether the numbers are divisible and will return 1 if they are divisible and 0 if they are not. Checker now reads the shared memory
        ID from coordintor.


|--Coordinator.c--|

   --The Coordinator program is responsible for launching 4 processes that it will
	then load using the Checker program. This program will fork() to launch
	processes, execlp() to replace the program driving the process and supply the
	arguments, and wait() to wait for process completion. Coordinator writes the shared memory ID to a
    pipe and reads the return value from checker via pipes and shared memory.

|--Makefile--|

   --The Makefile runs the following:
     --"make all"
     --"make clean"
     --"make package"



To compile:
    Type "make all" in terminal

To run:
    ./coordinator <divisor> <dividend1> <dividend2> <dividend3> <dividend4>

Example:
    ./coordinator 3 8 15 21 45
