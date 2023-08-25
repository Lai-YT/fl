SCANNER = scanner
PARSER = parser
CXX = g++
CXXFLAGS = -std=c++14 -g -Wall -Iinclude

OBJS = $(SCANNER).o $(PARSER).o src/visitor.o src/dump_visitor.o src/type.o
DEPS = $(OBJS:.o=.d)

main: %: %.cpp $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS)

$(PARSER).o: %.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) -MMD

%.hpp %.cpp: %.y
	bison -o $*.cpp --defines=$*.hpp $<

$(SCANNER).o: %.o: %.cpp $(PARSER).hpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) -MMD

%.cpp: %.l
	flex -o $@ $<

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) -MMD

test: main
	./main < test.fl

clean:
	rm -f $(SCANNER).cpp $(SCANNER).hpp \
		$(PARSER).cpp $(PARSER).hpp \
		$(OBJS) $(DEPS) main

.PHNOY: clean test

-include $(DEPS)
