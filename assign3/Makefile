# Makefile 
# CSCI 480
# Assignment 3

# we assume the pic directory locates one level above,
# change PIC_PATH if this is not the case
PIC_PATH = $(abspath $(CURDIR)/lib/pic)

INCLUDE = -I$(PIC_PATH) -I/lib/
LIBRARIES = -L$(PIC_PATH) -framework OpenGL -framework GLUT -lpicio $(PIC_PATH)/libjpeg.a

COMPILER = clang++
COMPILERFLAGS = -O3 $(INCLUDE) -Wno-deprecated-declarations -std=c++11

SCENES = $(wildcard scene/*.scene)
SCENE_RENDER = $(addprefix img/,$(notdir $(SCENES:.scene=.jpg)))

PROGRAM = assign3
SOURCE = $(wildcard src/*.cpp)
HEADERS = $(wildcard src/*.h)
OBJECT = $(addprefix obj/,$(notdir $(SOURCE:.cpp=.o)))


obj/%.o: src/%.cpp $(HEADERS)
	$(COMPILER) -c $(COMPILERFLAGS) -c -o $@ $<

all: $(PROGRAM)

$(PROGRAM): $(OBJECT)
	$(COMPILER) $(COMPILERFLAGS) -o $(PROGRAM) $(OBJECT) $(LIBRARIES)

clean:
	rm obj/*.o  $(PROGRAM)

img/%.jpg: scene/%.scene $(PROGRAM)
	./$(PROGRAM) $< $@ --nodisplay

render: $(SCENE_RENDER)
	$(info Rendered all scene files)
