main=main
dep=Cell.o Bit.o CellGroup.o ControlGroup.o UserControlGroup.o Level.o
compile=g++ -c
link=g++ -o
libs=-lsfml-graphics -lsfml-window -lsfml-system

$(main): $(main).o $(dep)
	 $(link) $@ $^ $(libs)

%.o: %.c
	$(compile) $^

clean:
	rm -rf $(main) $(main).o $(dep) *~ *# *~