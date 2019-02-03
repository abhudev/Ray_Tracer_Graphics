SRC = .
TGT = obj
$(shell mkdir -p $(TGT) >/dev/null)

EXEC = tracer
CXXFLAGS = -Wall -std=c++11 -O3 -fopenmp
SOURCES = $(wildcard $(SRC)/*.cpp)
HEADERS = $(wildcard $(SRC)/*.hpp)
# OBJS = $(SOURCES:.cpp=.o)
OBJS = $(addprefix $(TGT)/, $(notdir $(SOURCES:.cpp=.o)))

$(TGT)/%.o: $(SRC)/%.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

$(EXEC): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS)

.PHONY: clean
clean:
	rm -rf $(TGT); rm $(EXEC)