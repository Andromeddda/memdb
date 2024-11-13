#----------
# Compiler
#----------

CC = g++

#-------------
# Directories
#-------------

INCLUDES = includes
SRCDIR = src
BUILD = build
DEPDIR := $(BUILD)/deps
TESTS = tests
TEST_BUILD = $(BUILD)/tests

#-------
# Flags
#-------

CFLAGS = \
	-Wall \
	-Werror \
	-Wextra \
	-std=c++20 \
	-O0 \
	-g

# Add include directory
CFLAGS += -I $(abspath $(INCLUDES))

GTESTFLAGS = -lgtest -lgmock -pthread

# Ask compiler for dependencies
DEPFLAGS = \
	-MT $@ \
	-MMD \
	-MP \
	-MF $(DEPDIR)/$*.Td

# move temporary depenendecy files to .d files
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

#--------------------------
# Colors (ANSI color codes)
#--------------------------

BRED    = \033[1;31m
BGREEN  = \033[1;32m
BYELLOW = \033[1;33m
GREEN   = \033[1;35m
BCYAN   = \033[1;36m
RESET   = \033[0m

#-------
# Files
#-------

RUN = main
TEST = test

SOURCES = $(notdir $(wildcard $(SRCDIR)/*.cpp))
TEST_SRC = $(notdir $(wildcard $(TESTS)/*.cpp))

# Special object files:
RUN_OBJ = $(BUILD)/$(RUN).o
TEST_OBJ = $(TEST_BUILD)/$(TEST).o

# Object files:
OBJECTS = $(filter-out $(RUN_OBJ), $(SOURCES:%.cpp=$(BUILD)/%.o))
TEST_OBJECTS =  $(filter-out $(TEST_OBJ), $(TEST_SRC:%.cpp=$(TEST_BUILD)/%.o))

# Executable files:
RUN_EXECUTABLE = $(BUILD)/$(RUN)
TEST_EXECUTABLE = $(BUILD)/$(TEST)

#---------------
# Build process
#---------------

all: $(RUN_EXECUTABLE) $(TEST_EXECUTABLE)

# Link object files together
$(RUN_EXECUTABLE) : $(RUN_OBJ) $(OBJECTS)
	@printf "$(BYELLOW)Linking executable test $(BCYAN)$@$(RESET)\n"
	$(CC) $(LDFLAGS) $^ -o $@

# Link object files together
$(TEST_EXECUTABLE) : $(TEST_OBJ) $(TEST_OBJECTS)
	@printf "$(BYELLOW)Linking executable test $(BCYAN)$@$(RESET)\n"
	$(CC) $(LDFLAGS) $^ $(GTESTFLAGS) -o $@

# Build object files
$(BUILD)/%.o: $(SRCDIR)/%.cpp
$(BUILD)/%.o: $(SRCDIR)/%.cpp $(DEPDIR)/%.d Makefile | $(DEPDIR)
	@printf "$(BYELLOW)Building object file $(BCYAN)$@$(RESET)\n"
	@mkdir -p $(BUILD)
	$(CC) $(DEPFLAGS) $(CFLAGS) $< -c -o $@
	$(POSTCOMPILE)

# Build tests
$(TEST_BUILD)/%.o: $(TESTS)/%.cpp
$(TEST_BUILD)/%.o: $(TESTS)/%.cpp $(DEPDIR)/%.d Makefile | $(DEPDIR)
	@printf "$(BYELLOW)Building tests $(BCYAN)$@$(RESET)\n"
	@mkdir -p $(BUILD)
	@mkdir -p $(TEST_BUILD)
	$(CC) $(DEPFLAGS) $(CFLAGS) $< -c $(GTESTFLAGS) -o $@
	$(POSTCOMPILE)


# Create directory
$(DEPDIR):
	@mkdir -p $(DEPDIR)

# Dependency files
DEPFILES := $(SOURCES:%.cpp=$(DEPDIR)/%.d) $(TEST_SRC:%.cpp=$(DEPDIR)/%.d)

# Mention every .d file as a target so that Make won't fail if it doesn't exist
$(DEPFILES):

# Include all created dependencies in current makefile
include $(wildcard $(DEPFILES))

#-------------------
# Passing arguments
#-------------------

ifeq (solved, $(firstword $(MAKECMDGOALS)))
  SOLVED_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(SOLVED_ARGS):;@:)
endif


ifeq (stash, $(firstword $(MAKECMDGOALS)))
  STASH_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(STASH_ARGS):;@:)
endif


#-----------------
# Run the program
#-----------------

tests: $(TEST_EXECUTABLE) Makefile
	./$<

run: $(RUN_EXECUTABLE) Makefile
	./$<

gdb: $(RUN_EXECUTABLE) Makefile
	gdb ./$<

#-------
# Other
#-------

clean:
	@printf "$(BYELLOW)Cleaning build and resource directories$(RESET)\n"
	rm -rf $(BUILD)

# List of non-file targets:
.PHONY: clean run gdb tests