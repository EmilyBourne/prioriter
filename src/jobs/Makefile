SRCS=$(wildcard *.cpp)
OBJS=$(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))
INCLUDE=$(addprefix $(subst src,include,$(CURDIR))/, $(subst cpp,hpp,$(SRCS)))


all: $(OBJS)

$(OBJ_DIR)/%.o: %.cpp $(INCLUDE)
	@echo "Making     "$< 
ifeq ($(VERBOSE), 1)
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@
else
	@$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@
endif
