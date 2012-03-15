main=main
dep=classes/Cell.cpp classes/Bit.cpp classes/Pulser.cpp classes/CellGroup.cpp classes/ControlGroup.cpp classes/UserControlGroup.cpp classes/AIControlGroup.cpp classes/Level.cpp classes/LevelParser.cpp
objects=$(dep:.cpp=.o)
compile=g++ -c
link=g++
libs=-lsfml-graphics -lsfml-window -lsfml-system

$(main): $(objects) main.cpp
	$(link) main.cpp $(objects) $(libs) -o $@

classes/*.o: %.o: %.cpp %.h
	$(compile) $< -o $@

clean:
	rm -rf classes/*~ $(main) $(main).o $(objects)
