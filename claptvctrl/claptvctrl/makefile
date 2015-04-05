
INCDIR = include
SRCDIR = source
BINDIR = bin
LIBDIR = lib
OBJDIR = obj



TARGET = claptvctrl
LIBS = -lasound -pthread /usr/lib/libiguanaIR.so


CC = gcc

### Debug
#CFLAGS = -g -Wall
### Release
CFLAGS = -O3 -Wall

### To compile for PC, use: make platform=pc
### To compile for the, Pi use: make
ifeq "$(platform)" "pc"
CFLAGS += -D_PC
$(info Building with PC settings...)
else
$(info Building with Rasberry Pi settings...)
endif

.PHONY: default all clean

default: $(BINDIR)/$(TARGET)
all: default
	@echo All Done!

OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(wildcard $(SRCDIR)/*.c))
HEADERS = $(wildcard $(INCDIR)/*.h)


$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	@echo Compiling $<...
	@$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

.PRECIOUS: $(BINDIR)/$(TARGET) $(OBJECTS)

$(BINDIR)/$(TARGET): $(OBJECTS)
	@echo Linking...
	@$(CC) $(OBJECTS) -Wall $(LIBS) -O3 -o $@

clean:
	@echo Cleaning...
	@-rm -f $(OBJDIR)/*.o
	@-rm -f $(BINDIR)/$(TARGET)
	@echo Done!


