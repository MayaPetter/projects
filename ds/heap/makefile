# For using shared library add PATH to LD_LIBRARY_PATH.

# Recipes. GD - debug, GC - no debug.
GD = gcc -ansi -pedantic-errors -Wall -Wextra -g
GC = gcc -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3
OBJ_FLAGS = -fPIC -c
SHARED_LIB_FLAGS = -shared -Wl,-soname,libheap.so

# Dependencies for debug and release executables.
DEPENDENCIES = heap.c heap_test.c ../vector/vector.c
INCLUDES = -I ../vector/
DLL_FLAGS = -L../vector/ -lvector

# Dependencies for shared library.
OBJ_DEPENDENCIES = heap.o

# Names of the made files.
DEBUG = heap_debug.out
RELEASE = heap.out
LIB = libheap.so

all: $(DEBUG) $(RELEASE) create_objs create_shared_lib remove_objs

# Create debug executable.
$(DEBUG): $(DEPENDENCIES)
	$(GD) $(DEPENDENCIES) $(INCLUDES) $(DLL_FLAGS) -o $(DEBUG)

# Create release executable.
$(RELEASE): $(DEPENDENCIES)
	$(GC) $(DEPENDENCIES) $(INCLUDES) $(DLL_FLAGS) -o $(RELEASE)

# Create shared library.
create_objs: $(DEPENDENCIES)
	$(GC) $(INCLUDES) $(OBJ_FLAGS) $(DEPENDENCIES)

create_shared_lib: $(OBJ_DEPENDENCIES)
	$(GC) $(INCLUDES) $(SHARED_LIB_FLAGS) $(OBJ_DEPENDENCIES) -o $(LIB)

remove_objs: $(OBJ_DEPENDENCIES)
	rm *.o

