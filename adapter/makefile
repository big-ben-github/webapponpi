#
TARGET := adapter

INC = -I ./inc

SRC_DIR = ./src
SRCS = $(wildcard $(SRC_DIR)/*.c)
#SRCS = $(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.c))

OBJ_DIR = ./obj
OBJS = $(addprefix $(OBJ_DIR)/, $(patsubst %.c,%.o,$(notdir $(SRCS))))

LIBS = -L ./lib

# 
CC       := gcc
CPP      := g++
DEFINES  :=
LDFLAGS  := -lpthread -lwiringPi -lm
CFLAGS   := -Wall -O3 $(DEFINES)
# -g -Wall -O3 $(DEFINES) -Wp -MD
CPPFLAGS := 

# 
.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)
# $(INCLUDE) $(LIBS) $(LDFLAGS)
	
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -o $@ -c $^ $(INC) $(LIBS) $(LDFLAGS) $(CFLAGS)

clean:
	rm -rf $(TARGET)
	rm -fr $(OBJS)
