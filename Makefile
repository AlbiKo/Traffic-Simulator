
OBJ = main.o Core.o Mappa.o Blocco.o Nodo.o SFMLWidget.o
CXXFLAGS=-Wall `pkg-config --cflags gtkmm-3.0` -I SFML-2.3.2/include -I src/header/
LDFLAGS=-rdynamic
LDLIBS=`pkg-config --libs gtkmm-3.0` -L SFML-2.3.2/lib -lsfml-graphics -lsfml-window -lsfml-system 

VPATH=src/header:src/code

.PHONY: debug
debug: CXXFLAGS+=-D DEBUG_MODE
debug: Traffic Simulator

.PHONY: release
release: CXXFLAGS+=-D NDEBUG
release: Traffic Simulator

Traffic Simulator: $(OBJ)
	g++ -o "Traffic Simulator" $(OBJ) $(LDFLAGS) $(LDLIBS)


-include dependencies

SFMLWidget.o: SFMLWidget.h
 
.PHONY: depend clean cleanall
depend:
	g++ -MM src/code/*.cc > dependencies
clean:
	rm -f *.o
cleanall:
	rm -f program *.o *~


