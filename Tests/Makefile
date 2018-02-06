TARGET = Tests.app
TARGET_ALL = $(TARGET)
LIBS =  -lm -lsqlite3 -lpthread
INCLUDE_DIRS = './include/' '../Common/include/' '../Client/include/' '../Server/include/'
CC = gcc
CFLAGS = -g -Wall
CFLAGS += $(foreach includedir,$(INCLUDE_DIRS),-I$(includedir))

.PHONY: default all clean tests

default: $(TARGET)
all: $(TARGET_ALL)
test : $(TARGET_TEST)

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
OBJECTS += $(patsubst %.c, %.o, $(wildcard ../Common/*.c))
OBJECTS += $(patsubst %.c, %.o, $(wildcard ../Client/serverHandlerCore.c))
OBJECTS += $(patsubst %.c, %.o, $(wildcard ../Client/clientCore.c))
OBJECTS += $(patsubst %.c, %.o, $(wildcard ../Client/serverManager.c))
OBJECTS += $(patsubst %.c, %.o, $(wildcard ../Server/semaphores.c))
%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@ 

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC)  $(OBJECTS) -Wall  $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f ../Common/*.o
	-rm -f ../Client/*.o
	-rm -f ../Server/*.o
	-rm -f $(TARGET)
	

.PHONY: all clean

