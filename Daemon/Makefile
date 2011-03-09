boat_daemon: boat_daemon.c
	gcc -o boat_daemon -g -O0 boat_daemon.c -lusb-1.0 -I/usr/include/libusb-1.0


LIBS="/usr/lib/libusb-1.0.a /usr/lib/libpthread.a /usr/lib/libdl.a /usr/lib/libc.a /usr/lib/librt.a"

boat_daemon_static: boat_daemon.c
	gcc -o boat_daemon_static -g -O0 boat_daemon.c $(LIBS) -I/usr/include/libusb-1.0

clean:
	rm -f boat_daemon boat_daemon_static