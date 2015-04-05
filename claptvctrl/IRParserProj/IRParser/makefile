#Compiler/Linker settings
CC = g++
CFLAGS = -c -Wall
LFLAGS =

#Directories
INCDIR = include
SRCDIR = source
OBJDIR = obj
BINDIR = bin

#Paths
SOURCES    = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS    = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES)) 
EXECUTABLE = IRParser


##### INFO #######
$(info GNU Make Started...)


##### Targets ######
all : $(BINDIR)/$(EXECUTABLE)

$(BINDIR)/$(EXECUTABLE) : $(OBJECTS)
	@echo Building $@...
	@echo Linking $(OBJECTS)...
	@$(CC) $(LFLAGS) $(OBJECTS) -o $@
	@echo Build Successful!

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@echo Compiling $<...
	@$(CC) $(CFLAGS) -I$(INCDIR) $< -o $@


.PHONY: clean

clean:
	@echo Cleaning Solution...
	@rm -rf $(OBJDIR)/*o $(BINDIR)/$(EXECUTABLE)

