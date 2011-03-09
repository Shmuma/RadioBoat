#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <libusb.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#define BOAT_VENDOR  0x04D8
#define BOAT_PRODUCT 0xD002

#define DAEMON_PORT 10201

void run_daemon (libusb_device_handle* handle);
void handle_client (int fd, libusb_device_handle* handle);

int main (int argc, char *argv[])
{
    struct libusb_context *ctx;
    struct libusb_device_handle *handle;
    int err;

    if ((err = libusb_init (&ctx)) != 0) {
        printf ("libusb_init failed with clode %d\n", err);
        return 1;
    }

//    libusb_set_debug (ctx, 3);

    while (1) {
        handle = libusb_open_device_with_vid_pid (ctx, BOAT_VENDOR, BOAT_PRODUCT);
    
        if (!handle && 0) {
            printf ("Boat USB device not found, sleep for 10 seconds and try again\n");
            sleep (10);
        }
        else {
            printf ("Boat device found, open master socket\n");
            //            libusb_claim_interface (handle, 0);
            run_daemon (handle);
            //            libusb_close (handle);
            break;
        }
    }

    libusb_exit (ctx);
    return 0;
}


void run_daemon (libusb_device_handle* handle)
{
    int sock = socket (PF_INET, SOCK_STREAM, 0);
    int client, yes = 1;
    struct sockaddr_in saddr, caddr;
    socklen_t len;

    if (sock == -1) {
        printf ("Socket creation failed: %d\n", errno);
        return;
    }

    setsockopt (sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    memset (&saddr, 0, sizeof (saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons (DAEMON_PORT);

    if (bind (sock, (struct sockaddr*)&saddr, sizeof (saddr)) == -1) {
        printf ("Bind error %d\n", errno);
        close (sock);
        return;
    }

    if (listen (sock, 10) == -1) {
        printf ("Listen error %d\n", errno);
        close (sock);
        return;
    }

    len = sizeof (saddr);
    while ((client = accept (sock, (struct sockaddr*)&caddr, &len)) >= 0) {
        printf ("New connection arrived, fd = %d\n", client);
        handle_client (client, handle);
        close (client);
        len = sizeof (saddr);
        printf ("Connection closed\n");
    }

    close (sock);
}


struct pwm_message
{
    unsigned char enable;
    unsigned char c1_p, c2_p;
};

void handle_client (int fd, libusb_device_handle* handle)
{
    struct pwm_message msg;
    int len, gone;

    while ((len = recv (fd, &msg, sizeof (msg), 0)) == sizeof (msg)) {
        if (msg.enable != 0 && msg.enable != 1) {
            printf ("Protocol violation\n");
            break;
        }
        printf ("%02x, %02x, %02x\n", msg.enable, msg.c1_p, msg.c2_p);
        //        libusb_bulk_transfer (handle, 1 | LIBUSB_ENDPOINT_OUT, (unsigned char*)&msg, sizeof (msg), &gone, 1000);
    }
}
