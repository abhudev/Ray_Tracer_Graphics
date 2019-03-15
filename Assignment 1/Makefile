SRC = .
TGT = obj
$(shell mkdir -p $(TGT) >/dev/null)
DEPDIR = .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

EXEC = tracer
INCLUDES = -Iinclude
CXXFLAGS = -Wall -std=c++11 -O3 -fopenmp -g $(INCLUDES)
SOURCES = $(wildcard $(SRC)/*.cpp)
HEADERS = $(wildcard $(SRC)/*.hpp)
DEPFILES = $(wildcard $(DEPDIR)/*.d)
OBJS = $(addprefix $(TGT)/, $(notdir $(SOURCES:.cpp=.o)))
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

all: $(EXEC) $(TGT)/.compiler_flags

.PHONY: force
$(TGT)/.compiler_flags: force
	echo '$(DEPFLAGS) $(CXXFLAGS)' | cmp -s - $@ || echo '$(CXXFLAGS)' > $@

$(TGT)/%.o: $(SRC)/%.cpp $(DEPDIR)/%.d $(TGT)/.compiler_flags
	$(CXX) $(DEPFLAGS) $(CXXFLAGS) -c $< -o $@
	$(POSTCOMPILE)

$(EXEC): $(OBJS) $(TGT)/.compiler_flags
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

clean:
	rm -rf $(TGT); rm -rf .d; rm $(EXEC)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

-include $(DEPFILES)