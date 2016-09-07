all: checker coordinator

Checker.o: Checker.c 
	g++ -Wall -g -c Checker.c

checker: Checker.o 
	g++ -Wall -g Checker.o -o checker

Coordinator.o: Coordinator.c 
	g++ -Wall -g -c Coordinator.c

coordinator: Coordinator.o 
	g++ -Wall -g Coordinator.o -o coordinator

clean:
	rm *.o checker coordinator

package:
	tar -cvf Forlenza_Dominick.tar *.c Makefile README.txt
