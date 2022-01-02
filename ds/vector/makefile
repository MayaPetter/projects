# For using shared library add PATH to LD_LIBRARY_PATH.

# Recipes. GD - debug, GC - no debug.
GD = gcc -ansi -pedantic-errors -Wall -Wextra -g
GC = gcc -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3
OBJ_FLAGS = -fPIC -c
SHARED_LIB_FLAGS = -shared -Wl,-soname,libvector.so

# Dependencies for debug and release executables.
DEPENDENCIES = vector.c vector_test.c

# Dependencies for shared library.
C_DEPENDENCIES = vector.c
OBJ_DEPENDENCIES = vector.o

# Names of the made files.
DEBUG = vector_debug.out
RELEASE = vector.out
LIB = libvector.so

all: $(DEBUG) $(RELEASE) create_objs create_shared_lib remove_objs

# Create debug executable.
$(DEBUG): $(DEPENDENCIES)
	$(GD) $(DEPENDENCIES) -o $(DEBUG)

# Create release executable.
$(RELEASE): $(DEPENDENCIES)
	$(GC) $(DEPENDENCIES) -o $(RELEASE)

# Create shared library.
create_objs: $(C_DEPENDENCIES)
	$(GC) $(OBJ_FLAGS) $(C_DEPENDENCIES)

create_shared_lib: $(OBJ_DEPENDENCIES)
	$(GC) $(SHARED_LIB_FLAGS) $(OBJ_DEPENDENCIES) -o $(LIB)

remove_objs: $(OBJ_DEPENDENCIES)
	rm *.o

