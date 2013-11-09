NODE_NATIVE_PATH = node.native
HTTP_PARSER_PATH = ${NODE_NATIVE_PATH}/http-parser
LIBUV_PATH = ${NODE_NATIVE_PATH}/libuv
LIBQR_PATH = qrcode/libqr

LIBUV_NAME = libuv.a
LIBQR_NAME = libqr.a

OS_NAME=$(shell uname -s)
RTLIB=-lrt

INCLUDES = -I$(LIBUV_PATH)/include -I$(HTTP_PARSER_PATH) -I$(NODE_NATIVE_PATH) -I$(LIBQR_PATH) -I.
DEFINES = -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64

CXXFLAGS = -std=gnu++0x -g -O0 $(INCLUDES) $(DEFINES)

CP = cp -f

all: main

main: main.cpp $(LIBUV_PATH)/$(LIBUV_NAME) $(LIBQR_PATH)/$(LIBQR_NAME) $(HTTP_PARSER_PATH)/http_parser.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(RTLIB) -lz -lm -lpthread

$(LIBUV_PATH)/$(LIBUV_NAME):
	$(MAKE) -C $(LIBUV_PATH)

$(LIBQR_PATH)/$(LIBQR_NAME): $(LIBQR_PATH)/Makefile
	$(MAKE) -C $(LIBQR_PATH)

$(LIBQR_PATH)/Makefile:
	$(CP) Makefile.qrcode $(LIBQR_PATH)/Makefile

$(HTTP_PARSER_PATH)/http_parser.o:
	$(MAKE) -C $(HTTP_PARSER_PATH) http_parser.o

test: main
	./main

clean:
	$(MAKE) -C libuv clean
	$(MAKE) -C http-parser clean
	rm -f $(LIBUV_PATH)/$(LIBUV_NAME)
	rm -f $(HTTP_PARSER_PATH)/http_parser.o
	rm -f main
