# Makefile for P2C

# Variables definitions
OBJS = datastructures.o exception.o lexycal.o syntax.o semantic.o translator.o p2c.o
INCLUDE = ../include
CFLAGS = -Ofast -std=gnu++11 -c -I$(INCLUDE)
LFLAGS = -Ofast -std=gnu++11
LEXFLAGS = -Ofast -std=gnu++11 -c -I$(INCLUDE)
SYNFLAGS = -Ofast -std=gnu++11 -c -I$(INCLUDE)
SEMFLAGS = -Ofast -std=gnu++11 -c -I$(INCLUDE)
TRAFLAGS = -Ofast -std=gnu++11 -c -I$(INCLUDE)
P2CFLAGS = -Ofast -std=gnu++11 -c -I$(INCLUDE)
EXEFILE = P2C.exe

V ?= @

# Make options
all : check_files debug_none_update $(EXEFILE)

debug : LEXFLAGS += -DDEBUG
debug : SYNFLAGS += -DDEBUG
debug : SEMFLAGS += -DDEBUG
debug : TRAFLAGS += -DDEBUG
debug : P2CFLAGS += -DDEBUG
debug : CFLAGS += -DDEBUG
debug_lexycal : LEXFLAGS += -DDEBUG
debug_syntax : SYNFLAGS += -DDEBUG
debug_semantic : SEMFLAGS += -DDEBUG
debug_translator : TRAFLAGS += -DDEBUG

debug : check_files debug_update $(EXEFILE)
debug_lexycal : check_files debug_lexycal_update $(EXEFILE)
debug_syntax : check_files debug_syntax_update $(EXEFILE)
debug_semantic : check_files debug_semantic_update $(EXEFILE)
debug_translator : check_files debug_translator_update $(EXEFILE)

# Debugging options updating commands
check_files :
	$(V) if [ ! -f .debug.opt ]; then echo "0" > .debug.opt; fi
	$(V) if [ ! -f .debug_lexycal.opt ]; then echo "0" > .debug_lexycal.opt; fi
	$(V) if [ ! -f .debug_syntax.opt ]; then echo "0" > .debug_syntax.opt; fi
	$(V) if [ ! -f .debug_semantic.opt ]; then echo "0" > .debug_semantic.opt; fi
	$(V) if [ ! -f .debug_translator.opt ]; then echo "0" > .debug_translator.opt; fi

debug_update :
	$(V) if [ `cat .debug.opt` = 0 ]; then echo "1" > .debug.opt; fi
	$(V) if [ `cat .debug_lexycal.opt` = 0 ]; then echo "1" > .debug_lexycal.opt; fi
	$(V) if [ `cat .debug_syntax.opt` = 0 ]; then echo "1" > .debug_syntax.opt; fi
	$(V) if [ `cat .debug_semantic.opt` = 0 ]; then echo "1" > .debug_semantic.opt; fi
	$(V) if [ `cat .debug_translator.opt` = 0 ]; then echo "1" > .debug_translator.opt; fi

debug_lexycal_update :
	$(V) if [ `cat .debug.opt` = 0 ]; then echo "1" > .debug.opt; fi
	$(V) if [ `cat .debug_lexycal.opt` = 0 ]; then echo "1" > .debug_lexycal.opt; fi
	$(V) if [ `cat .debug_syntax.opt` = 1 ]; then echo "0" > .debug_syntax.opt; fi
	$(V) if [ `cat .debug_semantic.opt` = 1 ]; then echo "0" > .debug_semantic.opt; fi
	$(V) if [ `cat .debug_translator.opt` = 1 ]; then echo "0" > .debug_translator.opt; fi

debug_syntax_update :
	$(V) if [ `cat .debug.opt` = 0 ]; then echo "1" > .debug.opt; fi
	$(V) if [ `cat .debug_lexycal.opt` = 1 ]; then echo "0" > .debug_lexycal.opt; fi
	$(V) if [ `cat .debug_syntax.opt` = 0 ]; then echo "1" > .debug_syntax.opt; fi
	$(V) if [ `cat .debug_semantic.opt` = 1 ]; then echo "0" > .debug_semantic.opt; fi
	$(V) if [ `cat .debug_translator.opt` = 1 ]; then echo "0" > .debug_translator.opt; fi

debug_semantic_update :
	$(V) if [ `cat .debug.opt` = 0 ]; then echo "1" > .debug.opt; fi
	$(V) if [ `cat .debug_lexycal.opt` = 1 ]; then echo "0" > .debug_lexycal.opt; fi
	$(V) if [ `cat .debug_syntax.opt` = 1 ]; then echo "0" > .debug_syntax.opt; fi
	$(V) if [ `cat .debug_semantic.opt` = 0 ]; then echo "1" > .debug_semantic.opt; fi
	$(V) if [ `cat .debug_translator.opt` = 1 ]; then echo "0" > .debug_translator.opt; fi

debug_translator_update :
	$(V) if [ `cat .debug.opt` = 0 ]; then echo "1" > .debug.opt; fi
	$(V) if [ `cat .debug_lexycal.opt` = 1 ]; then echo "0" > .debug_lexycal.opt; fi
	$(V) if [ `cat .debug_syntax.opt` = 1 ]; then echo "0" > .debug_syntax.opt; fi
	$(V) if [ `cat .debug_semantic.opt` = 1 ]; then echo "0" > .debug_semantic.opt; fi
	$(V) if [ `cat .debug_translator.opt` = 0 ]; then echo "1" > .debug_translator.opt; fi

debug_none_update :
	$(V) if [ `cat .debug.opt` = 1 ]; then echo "0" > .debug.opt; fi
	$(V) if [ `cat .debug_lexycal.opt` = 1 ]; then echo "0" > .debug_lexycal.opt; fi
	$(V) if [ `cat .debug_syntax.opt` = 1 ]; then echo "0" > .debug_syntax.opt; fi
	$(V) if [ `cat .debug_semantic.opt` = 1 ]; then echo "0" > .debug_semantic.opt; fi
	$(V) if [ `cat .debug_translator.opt` = 1 ]; then echo "0" > .debug_translator.opt; fi

# Compiling commands
$(EXEFILE) : $(OBJS)
	$(CXX) $(LFLAGS) $(OBJS) -o $(EXEFILE)

datastructures.o : datastructures.cpp $(INCLUDE)/datastructures.hpp
	$(CXX) $(CFLAGS) datastructures.cpp

exception.o : datastructures.o exception.cpp $(INCLUDE)/exception.hpp
	$(CXX) $(CFLAGS) exception.cpp

lexycal.o : datastructures.o exception.o lexycal.cpp $(INCLUDE)/lexycal.hpp .debug_lexycal.opt
	$(CXX) $(LEXFLAGS) lexycal.cpp

syntax.o :	datastructures.o exception.o syntax.cpp $(INCLUDE)/syntax.hpp .debug_syntax.opt
	$(CXX) $(SYNFLAGS) syntax.cpp

semantic.o : datastructures.o exception.o semantic.cpp $(INCLUDE)/semantic.hpp .debug_semantic.opt
	$(CXX) $(SEMFLAGS) semantic.cpp

translator.o : datastructures.o exception.o translator.cpp $(INCLUDE)/translator.hpp .debug_translator.opt
	$(CXX) $(TRAFLAGS) translator.cpp

p2c.o : datastructures.o exception.o lexycal.o syntax.o semantic.o translator.o p2c.cpp .debug.opt
	$(CXX) $(P2CFLAGS) p2c.cpp

clean:
	rm -f *.o *~ core $(INCLUDE)/*~ *.exe .[!.]*.opt
