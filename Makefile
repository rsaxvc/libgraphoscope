CFLAGS=-O0 -g -Wall
CXXFLAGS=$(CFLAGS)
LDFLAGS=-lSDL -lSDL_mixer
SOURCES=`ls *.cpp *.c`
BUILDS=test_polygon_soundcard test_polygon_sdl_gl test_SDL_mixer
all: .depend $(BUILDS)

.depend:
	fastdep $(SOURCES) > .depend

-include .depend

libgraphoscope_soundcard.o:\
	audio_buffer.o\
	graphoscope_soundcard.o
	ld -r $^ -o $@

libgraphoscope_sdl_gl.o:\
	graphoscope_sdl_gl.o
	ld -r $^ -o $@
	
test_polygon_soundcard:\
	test_polygon.o\
	libgraphoscope_soundcard.o
	$(CXX) -o $@ $(CXXFLAGS) $(LDFLAGS) $^

test_polygon_sdl_gl:\
	test_polygon.o\
	libgraphoscope_sdl_gl.o
	$(CXX) -o $@ $(CXXFLAGS) $(LDFLAGS) $^ -lGL -lGLU

test_SDL_mixer:\
	test_SDL_mixer.o
	$(CXX) -o $@ $(CXXFLAGS) $(LDFLAGS) $^

clean:
	rm -f *.o .depend $(BUILDS)
