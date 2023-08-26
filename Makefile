
CXX := g++
CXXFLAGS := -std=c++17 -I ./cs1604/include
battlefield: main.o engine.o units.o field.o
	$(CXX) $(CXXFLAGS)\
	-lstdc++fs -luser32 -lgdi32 -lopengl32 \
	-lgdiplus -lShlwapi -ldwmapi \
	-L ./cs1604/lib\
	engine.o units.o field.o -o battlefield

main.o: main.cpp units.h engine.h field.h olcPixelGameEngine.h
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o
engine.o: engine.h
	$(CXX) $(CXXFLAGS) -c engine.cpp -o engine.o
units.o: units.h
	$(CXX) $(CXXFLAGS) -c units.cpp -o units.o
field.o: field.h
	$(CXX) $(CXXFLAGS) -c field.cpp -o field.o

.PHONY:clean
clean:
	rm -f *.o battlefield