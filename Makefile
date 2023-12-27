CXX = 				g++-12

DEBUG_CXXFLAGS =  	-fdiagnostics-color=always \
					-fdiagnostics-show-template-tree \
					-fdiagnostics-path-format=inline-events \
					-fdiagnostics-show-caret \
					-ggdb3 \
					-O1 \
					-Wall \
					-Wextra \
					-Wshadow \
					-Wconversion \
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
					-O3 \
					-Wall \
					-Wextra \
					-Wshadow \
					-Wconversion \
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

MUS_SIMULATOR_LIB = 	-ltbb -lpthread
MUS_SIMULATOR_INCL = 	-I/home/miguelveganzones/Libraries/parallel-hashmap-master/parallel_hashmap
MUS_SIMULATOR_BUILD_DIR = 	.

all: mus_simulator

mus_simulator:
	@echo Building $@..."\n"
	@mkdir -p $(MUS_SIMULATOR_BUILD_DIR)/${OUT_DIR}
	$(CXX) $(CXXFLAGS) $(MUS_SIMULATOR_INCL) $(MUS_SIMULATOR_BUILD_DIR)/$@.cpp ${MUS_SIMULATOR_LIB} -o $(MUS_SIMULATOR_BUILD_DIR)/${OUT_DIR}/$@
	@echo Built $@ successfully."\n"
	
done:
	@echo Built $@ successfully."\n"
