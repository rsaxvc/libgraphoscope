CFLAGS=-O0 -g
SOURCES=`ls *.cpp *.c`

all: .depend test_circle test_line

.depend:
	fastdep $(SOURCES) > .depend

-include .depend

test_line:\
	test_line.o\
	graphoscope.o
	$(CXX) -o $@ $(CXXFLAGS) $(LDFLAGS) $^

test_circle:\
	test_circle.o\
	graphoscope.o
	$(CXX) -o $@ $(CXXFLAGS) $(LDFLAGS) $^

clean:
	rm -f *.o .depend
