CPPFLAGS=-g -std=gnu++17 -Ivendor/jansson/include -Wall -Wextra
LDFLAGS=-g
LDLIBS=-Lvendor/jansson/lib -ljansson

SRCS=$(wildcard *.cpp cards/*.cpp)
OBJS=$(subst .cpp,.o,$(SRCS))

# ugly hack
ROOT_DIR=$(dir $(realpath $(firstword $(MAKEFILE_LIST))))

all: server

vendor:
	mkdir vendor

vendor/jansson-src: vendor
	curl https://digip.org/jansson/releases/jansson-2.13.1.tar.bz2 | tar xj -C vendor
	mv vendor/jansson-2.13.1 vendor/jansson-src

vendor/jansson: vendor
	mkdir vendor/jansson

vendor/jansson/include: vendor/jansson-src vendor/jansson
	cd vendor/jansson-src; env CFLAGS=-ggdb3 ./configure --prefix=$(ROOT_DIR)/vendor/jansson
	make -C vendor/jansson-src CFLAGS=-ggdb3
	make -C vendor/jansson-src install

Connection.cpp: vendor/jansson/include

server: $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	rm -f *.o cards/*.o server

distclean: clean
	rm -rf vendor

.PHONY: clean all depend

include .depend
