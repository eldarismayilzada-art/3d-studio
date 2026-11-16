#
#  Linux and MS Windows (MSYS2/MinGW) Makefile
#  Computer Graphics course, Dept Computing Science, Umea University
#
#  Targets:
#    make               build everything (checkpoint + ws1main)
#    make checkpoint    build only the GLFW/OpenGL/glad config checker
#    make ws1main       build only workshop 1
#    make clean         remove the build directory
#
#  Binaries end up in $(BUILD_DIR)/
#

BUILD_DIR = build

# ---- Sources ---------------------------------------------------------------
GLAD_DIR   = third_party/glad
GLAD_SRC   = $(GLAD_DIR)/src/glad.c

CHECK_SRCS = $(wildcard tools/glcheckpoint/*.cpp)
WS1_SRCS   = $(wildcard ws1/src/*.cpp)
WS1_SHADER_DIR = ws1/shaders/

# ---- Objects and dependency files -----------------------------------------
# build/<path of source>.o, so the source tree stays clean
GLAD_OBJ   = $(GLAD_SRC:%.c=$(BUILD_DIR)/%.o)
CHECK_OBJS = $(CHECK_SRCS:%.cpp=$(BUILD_DIR)/%.o) $(GLAD_OBJ)
WS1_OBJS   = $(WS1_SRCS:%.cpp=$(BUILD_DIR)/%.o) $(GLAD_OBJ)

# gcc writes the header dependencies of each source into a .d file (-MMD)
DEPS = $(sort $(CHECK_OBJS:%.o=%.d) $(WS1_OBJS:%.o=%.d))

# ---- Tools and flags -------------------------------------------------------
CXX = g++
CC  = gcc

DBFLAGS = -O0 -g3 -ggdb3 -fno-inline -Wfatal-errors
#DBFLAGS = -O2
IFLAGS  = -I$(GLAD_DIR)/include

ifeq ($(OS), Windows_NT)
# -DWINDOWS_BUILD needed to deal with Windows use of \ instead of / in paths
DEFS      = -DWINDOWS_BUILD
EXE       = .exe
GLFLAGS   =
LDLIBS    = -static -lglfw3 -lopengl32 -lgdi32 -luser32 -lkernel32
else
DEFS      =
EXE       =
GLFLAGS   = $(shell pkg-config --cflags glfw3)
# glad loads libGL at runtime with dlopen, hence -ldl (no need to link GL/GLEW)
LDLIBS    = $(shell pkg-config --libs glfw3) -ldl
endif

CXXFLAGS = $(DBFLAGS) -Wall -std=c++17 $(DEFS) $(IFLAGS) $(GLFLAGS) -MMD -MP
CFLAGS   = $(DBFLAGS) -Wall $(DEFS) $(IFLAGS) -MMD -MP

CHECK_BIN = $(BUILD_DIR)/glcheckpoint$(EXE)
WS1_BIN   = $(BUILD_DIR)/ws1main$(EXE)

# ---- Rules -----------------------------------------------------------------
.PHONY: all checkpoint ws1 clean

all: checkpoint ws1

checkpoint: $(CHECK_BIN)
ws1:  $(WS1_BIN)

$(CHECK_BIN): $(CHECK_OBJS)
	mkdir -p $(@D)
	$(CXX) $^ -o $@ $(LDLIBS)

checkpoint_run: checkpoint
	$(CHECK_BIN)

$(WS1_BIN): $(WS1_OBJS)
	mkdir -p $(@D)
	$(CXX) $^ -o $@ $(LDLIBS)

ws1_run: ws1
	$(WS1_BIN) $(WS1_SHADER_DIR)

# Pull in the generated header dependencies (silently skipped on first build)
-include $(DEPS)

$(BUILD_DIR)/%.o : %.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# glad.c is plain C, so compile it with the C compiler
$(BUILD_DIR)/%.o : %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
