CXX = 				g++

DEBUG_CXXFLAGS =  	-fdiagnostics-color=always \
					-fdiagnostics-show-template-tree \
					-fdiagnostics-path-format=inline-events \
					-fdiagnostics-show-caret \
					-ggdb3 \
					-O0 \
					-Wall \
					-Wextra \
					-Wshadow \
					-Wconversion \
					-Wmisleading-indentation \
					-fsanitize=address \
					-fsanitize=leak \
					-fsanitize=undefined \
					-Werror \
					-pedantic \
					-mavx \
					-fbounds-check \
					-fconcepts-diagnostics-depth=3 \
					-std=c++23

RELEASE_CXXFLAGS =  -fdiagnostics-color=always \
					-fdiagnostics-show-template-tree \
					-fdiagnostics-path-format=inline-events \
					-fdiagnostics-show-caret \
					-ggdb3 \
					-O2 \
					-Wall \
					-Wextra \
					-Wshadow \
					-Wconversion \
					-Wmisleading-indentation \
					-Werror \
					-pedantic \
					-mavx \
					-fbounds-check \
					-fconcepts-diagnostics-depth=3 \
					-std=c++23
					
FULL_RELEASE_CXXFLAGS =  -fdiagnostics-color=always \
					-fdiagnostics-show-template-tree \
					-fdiagnostics-path-format=inline-events \
					-fdiagnostics-show-caret \
					-O3 \
					-Wall \
					-Wextra \
					-Wshadow \
					-Wconversion \
					-Wmisleading-indentation \
					-Werror \
					-pedantic \
					-mavx \
					-fconcepts-diagnostics-depth=3 \
					-std=c++23 \
					-fno-exceptions
					
RELEASE ?= 0
ifeq (${RELEASE}, 1)
    CXXFLAGS = ${RELEASE_CXXFLAGS}
	OUT_DIR = bin/release
else ifeq (${RELEASE}, 2)
    CXXFLAGS = ${FULL_RELEASE_CXXFLAGS}
	OUT_DIR = bin/full_release
else
    CXXFLAGS = ${DEBUG_CXXFLAGS}
	OUT_DIR = bin/debug
endif

all: mus_simulator

mus_simulator:
	@echo Building $@..."\n"
	@mkdir -p ${OUT_DIR}
	$(CXX) $(CXXFLAGS) $@.cpp -o ${OUT_DIR}/$@
	@echo Built $@ successfully."\n"
	
done:
	@echo Built $@ successfully."\n"
