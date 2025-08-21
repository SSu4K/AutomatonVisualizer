CXX = g++
CPPFLAGS = -Wall -pedantic -I$(SOURCEDIR)
INC_SFML = -lsfml-graphics -lsfml-window -lsfml-system

PROJDIR := $(realpath $(CURDIR)/..)
SOURCEDIR := $(PROJDIR)/source
BUILDDIR := $(PROJDIR)/build
VPATH = $(SOURCEDIR)

SRCS := $(shell find $(SOURCEDIR) -name '*.cpp')
OBJS := $(patsubst $(SOURCEDIR)/%.cpp, $(BUILDDIR)/%.o, $(SRCS))

all: main

main: $(BUILDDIR)/main.o $(filter-out $(BUILDDIR)/main.o, $(OBJS))
	$(CXX) $(CPPFLAGS) -o $@ $^ $(INC_SFML)

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) -c $< -o $@

run: main
	./main

clean:
	rm -rf $(BUILDDIR)/*.o $(BUILDDIR)/*.a $(BUILDDIR)/*/*.o $(BUILDDIR)/*/*.a main