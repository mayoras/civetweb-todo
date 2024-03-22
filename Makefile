
TARGET=todo

CXX=clang++
SRC=$(addprefix src/, todo.cc common.cc ping.cc task.cc database.cc new.cc)

SRC+= $(addprefix lib/cJSON/, cJSON.cc cJSON_Utils.cc)

CFLAGS=-std=c++11 -Wall -Wextra -Wshadow -pedantic $(COPT) -Iinc -Ilib/cJSON -L./lib
LIBS=-lrt -ldl -lpthread
CIVETWEB_LIBPATH=./lib/libcivetweb.a

FILE_EXISTS := $(or $(and $(wildcard $(CIVETWEB_LIBPATH)),1),0)

ifeq (FILE_EXISTS,0)
	@echo "FAIL: CivetWeb library not found in lib"
	exit 1
else
	LIBS+= -lcivetweb
endif

ifdef VERBOSE
	CFLAGS+= -v
endif

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) -o $@ $(CFLAGS) $(LDFLAGS) $(SRC) $(LIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
