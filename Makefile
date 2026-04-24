CXX      = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET   = digitalized
SRCS     = main.cpp game.cpp robot.cpp module.cpp station.cpp utils.cpp
OBJS     = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
