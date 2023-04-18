SEP = ;
EXEC = prog
COMMANDES = commandes.txt

manuel:
	gcc -Wall -c *c $(SEP) gcc -o $(EXEC) *.o $(SEP) ./$(EXEC)

auto:
	gcc -Wall -c *c $(SEP) gcc -o $(EXEC) *.o $(SEP) ./$(EXEC) $(COMMANDES)

clean:
	rm chemin.o $(SEP) rm noeud.o $(SEP) rm shell.o $(SEP) rm $(EXEC)