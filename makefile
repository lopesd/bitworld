main=main
dep=classes/Cell.cpp classes/Bit.cpp classes/CellGroup.cpp classes/ControlGroup.cpp classes/UserControlGroup.cpp classes/Level.cpp
objects=$(dep:.cpp=.o)
compile=g++ -c
link=g++
libs=-lsfml-graphics -lsfml-window -lsfml-system

$(main): $(objects) main.cpp
	$(link) main.cpp $(objects) $(libs) -o $@

classes/*.o: %.o: %.cpp %.h
	$(compile) $< -o $@

clean:
<<<<<<< HEAD
	rm -rf $(main) $(main).o $(objects)
=======
	rm -rf $(main) $(main).o $(objects)
>>>>>>> fda10f06b89de4dfb230d3a678443bf9c6789ac0
