TARGET = Client.app
TARGET_ALL = $(TARGET)
LIBS =  -lm -lsqlite3 -lpthread
INCLUDE_DIRS = './include/' '../Common/include/'
CC = gcc
CFLAGS = -g -Wall
CFLAGS += $(foreach includedir,$(INCLUDE_DIRS),-I$(includedir))

.PHONY: default all clean tests

default: $(TARGET)
all: $(TARGET_ALL)
test : $(TARGET_TEST)

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
OBJECTS += $(patsubst %.c, %.o, $(wildcard ../Common/*.c))

%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@ 

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC)  $(OBJECTS) -Wall  $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f ../Common/*.o
	-rm -f $(TARGET)
	

.PHONY: all clean
