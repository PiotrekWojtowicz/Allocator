#
#	This is the makefile for the simple allocator project 
#					made by Piotr Wojtowicz
#				Cracow | Jagiellonian Univeristy
#
#

#
#Save the object files
#
.PRECIOUS: %.o

#
#Linker and compiler //both gcc
#
CC = gcc
LD = $(CC)

#
#Static library archv
#
AR = ar

#
#Compilation flags
#
FLAGS = -Wall --pedantic
#Since we're gonna use the static lib
#for the project
AFLAGS = rc

#
#Since it's not a huge project i won't bother
#myself with distinct directories
#
#Directories
WHEREAMI = /home/user_me/Allocator
ST_LIB_DIR = $(WHEREAMI)

#
#If necessary make changes here [$$]
#
#Static lib files
LIB_NAME = my_alloc
ST_LIB_NAME = lib$(LIB_NAME).a

#
#Store the library source files
#
LIB_C_FILES = $(foreach D, $(ST_LIB_DIR), $(wildcard $(D)/lib*.c))
#
#Store the general prefix
#If necessary make changes here [$$]
#
ALLOCATOR = allocator
#
#Tho just one program source file, this line saves us work
#in case we wanted to add some features
#
SRC_C_FILES = $(foreach D, $(WHEREAMI), $(wildcard $(D)/$(ALLOCATOR)*.c))

#
#Objects files
#
SRC_O_FILES = $(patsubst %.c, %.o, $(SRC_C_FILES))
LIB_O_FILES = $(patsubst %.c, %.o $(LIB_C_FILES))

#
#Objects files creator
#
%.o: %.c
	$(CC) $(FLAGS) -c -o $@ $<

#
#Binaries
#
#No need for any loops, just a basic var
ALLOCATOR_BINARY = $(ALLOCATOR).x

#
#Create static library
#
$(ST_LIB_DIR)/$(ST_LIB_NAME): $(LIB_O_FILES)
	$(AR) $@ $^

#
#Finally create the binaries
#
$(ALLOCATOR_BINARY): $(SRC_O_FILES) $(ST_LIB_DIR)/$(ST_LIB_NAME)
	$(LD) -o $@ $(SRC_O_FILES) -L$(ST_LIB_DIR) -l$(LIB_NAME) $(FLAGS) 