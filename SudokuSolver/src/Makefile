CC=gcc
# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS=-g -Wall -I.
LIBS=-lm
# SRCDIR=src
OBJDIR=obj
Target=SudokuSolver

DEPS = board_globals.h log.h algorithms.h read_and_display.h validate.h

# OBJ = SudokuSolver.o board_globals.o log.o algorithms.o read_and_display.o validate.o 
# OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
# OBJ := $(patsubst %.c,%.o,$(wildcard *.c))
# OBJ := $(patsubst %,$(ODIR)/%,$(_OBJ))

OBJS = $(patsubst %.c,../$(OBJDIR)/%.o,$(wildcard *.c))


all: say_hello generate run # clean

from_scratch: say_hello clean generate run

say_hello:
	@echo "Hello Jordan"
	
../$(OBJDIR)/%.o: %.c $(DEPS)
# %.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
generate: $(OBJS)
	@echo "I will now try to build the soduko solver for you..."
	# $(CC) -o sudoku SudokuSolver.c 
	$(CC) -o ../$(Target) $^ $(CFLAGS) $(LIBS)

run: 
	@echo "Running :)" 
	@echo " "
	../$(Target)

.PHONY: clean

clean:
	@echo "Cleaning..."
	
	# rm file-{1..10}.txt
	# rm *.txt

	rm $(Target) -f ../$(OBJDIR)/*.o 





