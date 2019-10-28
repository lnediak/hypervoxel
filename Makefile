ISWIN = $(WINDIR)
ZERO = 0
VAL = $(ZERO$(ISWIN))
ONE = 1
ONE0 = 0
OSNUM = $(ONE$(VAL))

EMPTY =
CXX0 = c++ --std=c++11 -Wall -Wextra -pedantic -g3 -O2 -c -o $(EMPTY)
CXX1 = cl.exe /std:c++14 /W4 /Zo /O2 /c /Fo
CXX = $(CXX$(OSNUM))

CXXLD0 = c++
CXXLD1 = link.exe
CXXLD = $(CXXLD$(OSNUM))

FIND0 = find -regex $(EMPTY)
FIND1 = dir /s $(EMPTY)
SRCS = TODO
OBJS0 = $(SRCS:.cpp=.o)
OBJS1 = $(SRCS:.cpp=.obj)
OBJS = $(OBJS$(OSNUM))

.SUFFIXES: .cpp .o .obj

.cpp.o:
	$(CXX0)$<

.cpp.obj:
	$(CXX1)$<

MKDEPEND0 = c++ -MM -MF $(EMPTY)
MKDEPEND1 = echo TODO
MKDEPEND = $(MKDEPENT$(OSNUM))

.depend: $(SRCS)
	$(MKDEPEND)$@ $(SRCS)

-include .depend

