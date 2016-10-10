BLOCCHI = Blocco.o Rettilineo.o Curva.o Incroci.o Incrocio3.o Incrocio4.o TipoBlocco.o
MAPPA = Mappa.o $(BLOCCHI) Direzione.o Direzionatore.o
LISTE = Blocco_List.o GraphNode_List.o Direzione_List.o Vector2i_List.o Macchina_List.o MacchinaPtr_List.o
OBJ = main.o Core.o Macchina.o Graph.o  GraphNode.o $(MAPPA) $(LISTE) SFMLWidget.o
CXXFLAGS=-Wall `pkg-config --cflags gtkmm-3.0` -std=c++11 -I ../src/header/
LDLIBS=`pkg-config --libs gtkmm-3.0` -lsfml-graphics -lsfml-window -lsfml-system

VPATH=../src/header:../src/code

.PHONY: debug
debug: CXXFLAGS+=-D DEBUG_MODE
debug: Traffic-Simulator

.PHONY: release
release: CXXFLAGS+=-D NDEBUG
release: Traffic-Simulator

Traffic-Simulator: $(OBJ)
	g++ -std=c++11 -o ../bin/Traffic-Simulator $(OBJ) $(LDLIBS)


-include dependencies

SFMLWidget.o: SFMLWidget.h

.PHONY: depend clean cleanall
depend:
	g++ -MM ../src/code/*.cc > dependencies
clean:
	rm -f *.o
cleanall:
	rm -f ../bin/Traffic-Simulator *.o *~
	rm -rf ../doc/html
