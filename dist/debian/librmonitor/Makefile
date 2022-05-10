prefix ?= /usr/local

VERSION = 1.0.0
VERSION_MAJOR = 1

CC = g++
FLAGS = -std=c++14 -O2 -Wno-unused-result -Wno-unused-function -Wno-unused-label \
	-Wno-unused-value -Wno-unused-variable

MACROS = \
	-DNDEBUG \
	-D_GNU_SOURCE \
	-D_POSIX_C_SOURCE=200112L \
	-D_XOPEN_SOURCE=600

INCLUDES = \
	`wx-config --cflags`

LIBS = \
	`wx-config --libs`

CFLAGS = $(FLAGS) $(MACROS) $(INCLUDES)
LDFLAGS = $(LIBS)


# 
# Source files
# 
RM_SRCS = \
	src/attribute.cpp \
	src/call.cpp \
	src/client.cpp \
	src/client_com.cpp \
	src/echo.cpp \
	src/encryption.cpp \
	src/request.cpp \
	src/serial.cpp \
	src/serial_list.cpp \
	src/sync.cpp \
	src/timerbase.cpp \
	src/widget.cpp \
	src/serial/serial.cpp \
	src/serial/impl/unix.cpp \
	src/serial/impl/list_ports/list_ports_linux.cpp

RM_WX_SRCS = \
	src/button.cpp \
	src/checkbox.cpp \
	src/echobox.cpp \
	src/gauge.cpp \
	src/icon.cpp \
	src/radiobox.cpp \
	src/serial_wx.cpp \
	src/slider.cpp \
	src/stattext.cpp \
	src/textctrl.cpp \
	src/timer.cpp

RM_OBJS = $(patsubst src/%.cpp, build/%.o, $(RM_SRCS))

RM_WX_OBJS = $(patsubst src/%.cpp, build/%.o, $(RM_WX_SRCS))




all: rmonitor rmonitor-wx
	@ :

mkdir:
	@ mkdir -p build
	@ mkdir -p build/serial
	@ mkdir -p build/serial/impl
	@ mkdir -p build/serial/impl/list_ports


# 
# RM library
# 
rmonitor: mkdir $(RM_OBJS)
	$(CC) -shared -Wl,-soname,librmonitor.so.$(VERSION_MAJOR) \
		-o librmonitor.so.$(VERSION) $(RM_OBJS) $(LDFLAGS) -shared
	@ ln -sf librmonitor.so.$(VERSION) librmonitor.so.$(VERSION_MAJOR)
	@ ln -sf librmonitor.so.$(VERSION) librmonitor.so
	@ mv librmonitor.so build
	@ mv librmonitor.so.$(VERSION) build
	@ mv librmonitor.so.$(VERSION_MAJOR) build

$(RM_OBJS): build/%.o: src/%.cpp
	@ $(CC) -fPIC -c $(CFLAGS) -o $@ $<


# 
# RM wxWidgets
# 
rmonitor-wx: rmonitor $(RM_WX_OBJS)
	$(CC) -shared -Wl,-soname,librmonitor-wx.so.$(VERSION_MAJOR) \
		-o librmonitor-wx.so.$(VERSION) $(RM_WX_OBJS) \
		$(LDFLAGS) -L./build -lrmonitor -shared
	@ ln -sf librmonitor-wx.so.$(VERSION) librmonitor-wx.so.$(VERSION_MAJOR)
	@ ln -sf librmonitor-wx.so.$(VERSION) librmonitor-wx.so
	@ mv librmonitor-wx.so build
	@ mv librmonitor-wx.so.$(VERSION) build
	@ mv librmonitor-wx.so.$(VERSION_MAJOR) build

$(RM_WX_OBJS): build/%.o: src/%.cpp
	@ $(CC) -fPIC -c $(CFLAGS) -o $@ $<




install:
	@ :
	install -Dm 755 build/librmonitor.so.$(VERSION) \
		$(DESTDIR)$(prefix)/lib/librmonitor.so.$(VERSION)
	cp -a build/librmonitor.so \
		$(DESTDIR)$(prefix)/lib/librmonitor.so
	cp -a build/librmonitor.so.$(VERSION_MAJOR) \
		$(DESTDIR)$(prefix)/lib/librmonitor.so.$(VERSION_MAJOR)
	@ :
	install -Dm 755 build/librmonitor-wx.so.$(VERSION) \
		$(DESTDIR)$(prefix)/lib/librmonitor-wx.so.$(VERSION)
	cp -a build/librmonitor-wx.so \
		$(DESTDIR)$(prefix)/lib/librmonitor-wx.so
	cp -a build/librmonitor-wx.so.$(VERSION_MAJOR) \
		$(DESTDIR)$(prefix)/lib/librmonitor-wx.so.$(VERSION_MAJOR)
	@ :
	install -Dm 644 rmonitor.pc \
		$(DESTDIR)$(prefix)/lib/pkgconfig/rmonitor.pc
	install -Dm 644 src/robotmonitor.hpp \
		$(DESTDIR)$(prefix)/include/robotmonitor.hpp
	install -Dm 644 src/rm/attribute.hpp \
		$(DESTDIR)$(prefix)/include/rm/attribute.hpp
	install -Dm 644 src/rm/button.hpp \
		$(DESTDIR)$(prefix)/include/rm/button.hpp
	install -Dm 644 src/rm/call.hpp \
		$(DESTDIR)$(prefix)/include/rm/call.hpp
	install -Dm 644 src/rm/checkbox.hpp \
		$(DESTDIR)$(prefix)/include/rm/checkbox.hpp
	install -Dm 644 src/rm/client.hpp \
		$(DESTDIR)$(prefix)/include/rm/client.hpp
	install -Dm 644 src/rm/config.h \
		$(DESTDIR)$(prefix)/include/rm/config.h
	install -Dm 644 src/rm/echo.hpp \
		$(DESTDIR)$(prefix)/include/rm/echo.hpp
	install -Dm 644 src/rm/echobox.hpp \
		$(DESTDIR)$(prefix)/include/rm/echobox.hpp
	install -Dm 644 src/rm/encryption.hpp \
		$(DESTDIR)$(prefix)/include/rm/encryption.hpp
	install -Dm 644 src/rm/gauge.hpp \
		$(DESTDIR)$(prefix)/include/rm/gauge.hpp
	install -Dm 644 src/rm/icon.hpp \
		$(DESTDIR)$(prefix)/include/rm/icon.hpp
	install -Dm 644 src/rm/radiobox.hpp \
		$(DESTDIR)$(prefix)/include/rm/radiobox.hpp
	install -Dm 644 src/rm/request.hpp \
		$(DESTDIR)$(prefix)/include/rm/request.hpp
	install -Dm 644 src/rm/serial.hpp \
		$(DESTDIR)$(prefix)/include/rm/serial.hpp
	install -Dm 644 src/rm/slider.hpp \
		$(DESTDIR)$(prefix)/include/rm/slider.hpp
	install -Dm 644 src/rm/spinctrl.hpp \
		$(DESTDIR)$(prefix)/include/rm/spinctrl.hpp
	install -Dm 644 src/rm/stattext.hpp \
		$(DESTDIR)$(prefix)/include/rm/stattext.hpp
	install -Dm 644 src/rm/sync.hpp \
		$(DESTDIR)$(prefix)/include/rm/sync.hpp
	install -Dm 644 src/rm/textctrl.hpp \
		$(DESTDIR)$(prefix)/include/rm/textctrl.hpp
	install -Dm 644 src/rm/timer.hpp \
		$(DESTDIR)$(prefix)/include/rm/timer.hpp
	install -Dm 644 src/rm/timerbase.hpp \
		$(DESTDIR)$(prefix)/include/rm/timerbase.hpp
	install -Dm 644 src/rm/widget.hpp \
		$(DESTDIR)$(prefix)/include/rm/widget.hpp
	install -Dm 644 src/serial/serial.h \
		$(DESTDIR)$(prefix)/include/rm/serial/serial.h
	install -Dm 644 src/serial/v8stdint.h \
		$(DESTDIR)$(prefix)/include/rm/serial/v8stdint.h

clean:
	-rm -r build

distclean: clean

uninstall:
	-rm -f $(DESTDIR)$(prefix)/lib/librmonitor.so
	-rm -f $(DESTDIR)$(prefix)/lib/librmonitor.so.$(VERSION_MAJOR)
	-rm -f $(DESTDIR)$(prefix)/lib/librmonitor.so.$(VERSION)
	-rm -f $(DESTDIR)$(prefix)/lib/librmonitor-wx.so
	-rm -f $(DESTDIR)$(prefix)/lib/librmonitor-wx.so.$(VERSION_MAJOR)
	-rm -f $(DESTDIR)$(prefix)/lib/librmonitor-wx.so.$(VERSION)
	-rm -f -r $(DESTDIR)$(prefix)/include/rm

.PHONY: all remote install clean distclean uninstall
