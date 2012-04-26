CFLAGS=-O0 -g -Wall
CXXFLAGS=$(CFLAGS)
LDFLAGS=-lSDL -lSDL_mixer
SOURCES=`ls *.cpp *.c`
BUILDS=test_polygon test_SDL_mixer
all: .depend $(BUILDS)

.depend:
	fastdep $(SOURCES) > .depend

-include .depend

libgraphoscope.o:\
	audio_buffer.o\
	graphoscope.o
	ld -r $^ -o $@
	
test_polygon:\
	test_polygon.o\
	libgraphoscope.o
	$(CXX) -o $@ $(CXXFLAGS) $(LDFLAGS) $^

test_SDL_mixer:\
	test_SDL_mixer.o
	$(CXX) -o $@ $(CXXFLAGS) $(LDFLAGS) $^

clean:
	rm -f *.o .depend $(BUILDS)
