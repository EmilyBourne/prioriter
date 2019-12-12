SRCS=$(wildcard *.cpp)
OBJS=$(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

all: $(OBJS)

$(OBJ_DIR)/%.o: %.cpp $(subst src,include,$(CURDIR))/%.hpp
	@echo "Making     "$<
ifeq ($(VERBOSE), 1)
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@
else
	@$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@
endif
