#include "tap.h"

// From tuntap.txt on kernel.org

// TUN and TAP are kernel virtual network devices
// TAP simulates a link layer device (layer 2) and carries Ethernet frames, and can be used to create a user space network bridge.
// A user space program may also pass packets into a TUN/TAP device. In this case the TUN/TAP device delivers (or "injects") 
//  these packets to the operating-system network stack thus emulating their reception from an external source.

int tap_alloc(char *dev)
{
    struct ifreq ifr;
    int fd, err;

    if ((fd = open("/dev/net/tun", O_RDWR)) < 0){ return fd; }

    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;

    if(*dev)
    {
        // set the interface name if dev is not null
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    }

    // Finally we have an ioctl system call operation
    if ((err = ioctl(fd, TUNSETIFF, (void *)&ifr)) < 0)
    {
        // If error occurs
        close(fd);
        return err;
    }

    strcpy(dev, ifr.ifr_name);
    return fd;
}