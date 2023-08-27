
CXX := g++
CXXFLAGS := -std=c++17 -I ./cs1604/include
battlefield: main.o engine.o units.o field.o utility.o
	$(CXX) -std=c++17 \
	main.o engine.o units.o field.o utility.o -o battlefield \
	-lstdc++fs -luser32 -lgdi32 -lopengl32 \
	-lgdiplus -lShlwapi -ldwmapi \
	-L ./cs1604/lib\


main.o: main.cpp units.h engine.h field.h olcPixelGameEngine.h utility.h
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o
engine.o: engine.h
	$(CXX) $(CXXFLAGS) -c engine.cpp -o engine.o
units.o: units.h
	$(CXX) $(CXXFLAGS) -c units.cpp -o units.o
field.o: field.h
	$(CXX) $(CXXFLAGS) -c field.cpp -o field.o
utility.o: utility.h
	$(CXX) $(CXXFLAGS) -c utility.cpp -o utility.o
.PHONY:clean
clean:
	rm -f *.o battlefield