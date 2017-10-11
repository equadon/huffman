# Directories
BINDIR := bin
BUILDDIR := build
DOCDIR := doc
COVDIR := $(DOCDIR)/coverage
INCLUDEDIR := include
LIBDIR := lib
TESTDIR := tests
SRCDIR := src

# Compiler flags
CC := gcc
DEFAULT_CFLAGS := -Wall -std=gnu11 -MMD -I $(SRCDIR) -I $(INCLUDEDIR)
CFLAGS :=
DEFAULT_LDFLAGS := -L $(BUILDDIR)
LDFLAGS :=
LDFLAGS_TEST := -lcunit -lgcov

# Executable
TARGET := bin/huffman
TEST_TARGET := bin/tests

# Source and object files
SOURCES := $(shell find $(SRCDIR) -type f -name '*.c')
OBJECTS := $(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(SOURCES:.c=.o))

TEST_SOURCES := $(shell find $(TESTDIR) -type f -name '*.c')
TEST_OBJECTS := $(patsubst $(TESTDIR)/%, $(BUILDDIR)/%, $(TEST_SOURCES:.c=.o))

# Dependencies
DEPS := $(OBJECTS:.o=.d)
TEST_DEPS := $(TEST_OBJECTS:.o=.d)

# Force make to run 'make all' when no target is specified
all: debug

# Release target
release: CFLAGS += -D RELEASE -O2
release: clean $(TARGET)

# Debug target
debug: CFLAGS += -D DEBUG -g --coverage
debug: LDFLAGS += -lgcov
debug: $(TARGET)

# Generate object files for src/*.c
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(DEFAULT_CFLAGS) $(CFLAGS) -c $< -o $@

# Generate object files for tests/*.c
$(BUILDDIR)/%.o: $(TESTDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(DEFAULT_CFLAGS) $(CFLAGS) -c -g -D DEBUG $< -o $@

# Link main program
$(TARGET): $(OBJECTS)
	$(CC) $^ -o $(TARGET) $(DEFAULT_LDFLAGS) $(LDFLAGS)

# Link test program
$(TEST_TARGET): $(TEST_OBJECTS) $(filter-out $(BUILDDIR)/main.o, $(OBJECTS))
	$(CC) $^ -o $(TEST_TARGET) $(DEFAULT_LDFLAGS) $(LDFLAGS_TEST)

# Build and run unit tests
test: debug $(TEST_TARGET)
	@./$(TEST_TARGET)

# Build and run unit tests with valgrind
memtest: $(TEST_TARGET)
	@valgrind --leak-check=yes ./$(TEST_TARGET)

# Build documentation
doc:
	@doxygen Doxyfile
	@tools/browser "$(DOCDIR)/html/index.html"

# Code coverage
cov: test
	@echo "Generating HTML on code coverage..."
	@lcov --capture --directory . --output-file $(BUILDDIR)/coverage.data
	@genhtml $(BUILDDIR)/coverage.data --output-directory $(COVDIR)
	@echo "Code coverage saved to: $(COVDIR)/index.html"
	@tools/browser "$(COVDIR)/index.html"

# Clean build files
clean:
	rm -rf $(BUILDDIR) $(DOCDIR)/* $(TARGET) $(TEST_TARGET)

# Fix coding style in project
style:
	@astyle --options="astyle.options" "$(SRCDIR)/*" "$(TESTDIR)/*"

-include $(DEPS)
-include $(TEST_DEPS)

# Clean is a phony target since it's not producing a file
.PHONY: all clean cov debug doc release style test
