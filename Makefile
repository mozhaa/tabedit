CXX = g++
CXXFLAGS = -O2 -g -Wall -Werror -std=c++17 -Iinclude -Iinclude/external -DTSF_IMPLEMENTATION
LDFLAGS = -I/usr/local/include -L/usr/local/lib -lncursesw -lsfml-audio 

EXE = tabedit
SRCDIR = src
BINDIR = obj

OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(BINDIR)/%.o,$(wildcard $(SRCDIR)/*.cpp))

all: $(EXE)

$(EXE): $(BINDIR) $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXE) $(LDFLAGS)
	
$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -MMD -o $@ $<

include $(wildcard $(BINDIR)/*.d)

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -rf $(BINDIR) $(EXE)

.PHONY: clean all
