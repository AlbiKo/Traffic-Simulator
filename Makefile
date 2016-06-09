
OBJ = main.o SFMLWidget.o
CXXFLAGS=-Wall `pkg-config --cflags gtkmm-3.0` -I SFML-2.3.2/include -I header/
LDFLAGS=-rdynamic
LDLIBS=`pkg-config --libs gtkmm-3.0` -L SFML-2.3.2/lib -lsfml-graphics -lsfml-window -lsfml-system 

VPATH=src:header

.PHONY: debug
debug: CXXFLAGS+=-D DEBUG_MODE
debug: program

.PHONY: release
release: CXXFLAGS+=-D NDEBUG
release: program

program: $(OBJ)
	g++ -o program $(OBJ) $(LDFLAGS) $(LDLIBS)


-include dependencies

SFMLWidget.o: SFMLWidget.h
 
.PHONY: depend clean cleanall
depend:
	g++ -MM src/*.cc > dependencies
clean:
	rm -f *.o
cleanall:
	rm -f program *.o *~


