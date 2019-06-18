src = test.cpp
obj = $(src:.cpp=.o)

CFLAGS = -Wall -std=c++1y -O0
LDFLAGS = -lm 
CC=g++

test: $(obj)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	rm -f $(obj) test

