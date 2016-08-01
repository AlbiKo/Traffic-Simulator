
OBJ = main.o Core.o Mappa.o Blocco.o Rettilineo.o Curva.o Incrocio3.o Incrocio4.o Nodo.o SFMLWidget.o
CXXFLAGS=-Wall `pkg-config --cflags gtkmm-3.0` -I ../src/header/
LDFLAGS=-rdynamic
LDLIBS=`pkg-config --libs gtkmm-3.0` -lsfml-graphics -lsfml-window -lsfml-system

VPATH=../src/header:../src/code

.PHONY: debug
debug: CXXFLAGS+=-D DEBUG_MODE
debug: Traffic-Simulator

.PHONY: release
release: CXXFLAGS+=-D NDEBUG
release: Traffic-Simulator

Traffic-Simulator: $(OBJ)
	g++ -o ../bin/Traffic-Simulator $(OBJ) $(LDFLAGS) $(LDLIBS)


-include dependencies

SFMLWidget.o: SFMLWidget.h

.PHONY: depend clean cleanall
depend:
	g++ -MM ../src/code/*.cc > dependencies
clean:
	rm -f *.o
cleanall:
	rm -f ../bin/Traffic-Simulator *.o *~
