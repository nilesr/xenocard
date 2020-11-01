CPPFLAGS=-g -std=gnu++17 -Ivendor/jansson/include -Wall -Wextra
LDFLAGS=-g
LDLIBS=-Lvendor/jansson/lib -ljansson

SRCS=$(wildcard *.cpp cards/*.cpp)
OBJS=$(subst .cpp,.o,$(SRCS))

# ugly hack
ROOT_DIR=$(dir $(realpath $(firstword $(MAKEFILE_LIST))))

JANSSON_VERSION=2.13.1
WEBSOCKETD_VERSION=0.3.0

all: server

vendor/jansson-src:
	mkdir -p vendor
	curl https://digip.org/jansson/releases/jansson-$(JANSSON_VERSION).tar.bz2 | tar xj -C vendor
	mv vendor/jansson-$(JANSSON_VERSION) vendor/jansson-src

vendor/jansson/include/jansson.h: vendor/jansson-src
	mkdir -p vendor/jansson
	cd vendor/jansson-src; env CFLAGS=-ggdb3 ./configure --prefix=$(ROOT_DIR)/vendor/jansson
	make -C vendor/jansson-src CFLAGS=-ggdb3
	make -C vendor/jansson-src install

vendor/websocketd/websocketd:
	mkdir -p vendor/websocketd
	curl -L -o vendor/websocketd-src.zip https://github.com/joewalnes/websocketd/releases/download/v$(WEBSOCKETD_VERSION)/websocketd-$(WEBSOCKETD_VERSION)-linux_amd64.zip
	unzip -d vendor/websocketd vendor/websocketd-src.zip

Connection.cpp: vendor/jansson/include/jansson.h

server: $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	rm -f .depend *.o cards/*.o server

distclean: clean
	rm -rf vendor

.PHONY: clean all depend

-include .depend
