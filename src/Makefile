# Compiler and Linker Config
CC=g++
LD=g++

# Compiler Flags
CFLAGS_COMMON=-Wall -Wextra -std=c++17 -I/usr/include/mysql -I/usr/include/mysql-cppconn-8/ -D_GLIBCXX_USE_CXX11_ABI=1
CFLAGS_DEBUG=-g -D_DEBUG -DSTATIC_CONCPP
CFLAGS_RELEASE=-O3 -DNDEBUG -DSTATIC_CONCPP

# Linker Flags
LFLAGS_COMMON=-L/usr/lib/mysql -L/usr/local/lib
LFLAGS_DEBUG=
LFLAGS_RELEASE=

# MySQL Dependencies
MYSQL_LIBS=-lmysqlcppconn8-static -lssl -lcrypto -pthread -lresolv

# Source and Object Files
SOURCES=$(wildcard *.cpp)
OBJECTS_DEBUG=$(SOURCES:.cpp=_debug.o)
OBJECTS_RELEASE=$(SOURCES:.cpp=_release.o)

# Targets
.PHONY: all clean debug release

all: debug release

debug: CFLAGS=$(CFLAGS_COMMON) $(CFLAGS_DEBUG)
debug: LFLAGS=$(LFLAGS_COMMON) $(LFLAGS_DEBUG)
debug: $(OBJECTS_DEBUG)
	$(LD) $(OBJECTS_DEBUG) -o program_debug $(LFLAGS) $(MYSQL_LIBS)

release: CFLAGS=$(CFLAGS_COMMON) $(CFLAGS_RELEASE)
release: LFLAGS=$(LFLAGS_COMMON) $(LFLAGS_RELEASE)
release: $(OBJECTS_RELEASE)
	$(LD) $(OBJECTS_RELEASE) -o program_release $(LFLAGS) $(MYSQL_LIBS)

%_debug.o: %.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

%_release.o: %.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f *.o program_debug program_release