CC ?= gcc
EDJE_CC = /usr/bin/edje_cc


PKGS = elementary atk eail
LDFLAGS = `pkg-config --libs $(PKGS)`
LDFLAGS += -L. -l utils
LDFLAGS += -l request
LDFLAGS += -Wl,-rpath,`pwd`

CFLAGS = -I. `pkg-config --cflags $(PKGS)`
CFLAGS += -Wall -fPIC -g


REQUEST_LIB_SOURCE = request.c
REQUEST_LIB_TARGET = librequest.so
REQUEST_LIB_OBJECT = $(REQUEST_LIB_SOURCE:.c=.o)

UTILS_LIB_SOURCE = utils.c
UTILS_LIB_TARGET = libutils.so
UTILS_LIB_OBJECT = $(UTILS_LIB_SOURCE:.c=.o)

EDJE_SOURCES = layout_example.edc
EDJE_OBJECTS = $(EDJE_SOURCES:.edc=.edj)

TCS := $(shell ls -1 *.c |grep -v $(REQUEST_LIB_SOURCE) | grep -v $(UTILS_LIB_SOURCE) | cut -d. -f1)

all: requestlib utilslib samples

requestlib: $(REQUEST_LIB_TARGET)
$(REQUEST_LIB_TARGET): $(REQUEST_LIB_OBJECT)
	$(CC) -shared -o $(REQUEST_LIB_TARGET) $(REQUEST_LIB_OBJECT)

utilslib: $(UTILS_LIB_TARGET)
$(UTILS_LIB_TARGET): $(UTILS_LIB_OBJECT)
	$(CC) -shared -o $(UTILS_LIB_TARGET) $(UTILS_LIB_OBJECT)


$(EDJE_OBJECTS): $(EDJE_SOURCES)
	$(EDJE_CC) $<

samples: $(EDJE_OBJECTS) $(TCS)
%: %.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(TCS)
	rm -f $(REQUEST_LIB_OBJECT)
	rm -f $(UTILS_LIB_OBJECT)
	rm -f $(EDJE_OBJECTS)
	rm -f $(REQUEST_LIB_TARGET)
	rm -f $(UTILS_LIB_TARGET)
