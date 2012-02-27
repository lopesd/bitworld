main=main
dep=classes/Cell.o classes/Bit.o classes/CellGroup.o classes/ControlGroup.o classes/UserControlGroup.o classes/Level.o
compile=g++ -c
link=g++ -o
libs=-lsfml-graphics -lsfml-window -lsfml-system

$(main): $(main).o $(dep)
	 $(link) $@ $^ $(libs)

%.o: %.c
	$(compile) $^

clean:
	rm -rf $(main) $(main).o $(dep) *~ *# classes/*~