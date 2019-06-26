src = $(wildcard src/*.cpp)
obj = $(src:.c=.o)


# Others libs
LDLIBS  += -ldl -lpthread
# Code is in C++11. (not in 14 because we target an Ubuntu 16.04 that uses a gcc 5).
CXXFLAGS=-g -std=c++11 -Wall -pedantic -I src

.PHONY: blackjack clean distclean
all: blackjack

clean distclean:
	rm -fr *.o blackjack

blackjack: $(obj)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(CFLAGS) $(LDFLAGS) $(LDLIBS)
