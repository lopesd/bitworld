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
	rm -rf $(main) $(main).o $(objects)
