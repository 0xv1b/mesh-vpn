#include "tap.h"
#include <stdbool.h>
#include <assert.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <pthread.h>


struct vport_t 
{
    int tapfd;
    int vport_sockfd;
    struct sockaddr_in vswitch_addr;
};


void vport_init(struct vport_t *vport, const char *server_ip_str, int server_port);
void *forward_ether_data_to_vswitch(void *raw_vport);
void *forward_ether_data_to_tap(void *raw_vport);

int main(int argc, char const *argv[])
{
    if(argc != 3)
    {
        ERROR_PRINT_THEN_EXIT("Usage: vport {server_ip} {server_port}\n");
    }

    const char *server_ip_str = argv[1];
    int server_port = atoi(argv[2]);

    // Initialise the port
    struct vport_t vport;
    vport_init(&vport, server_ip_str, server_port);

    // Create upforwarding thread that runs forward_ether_data_to_vswitch
    pthread_t up_forwarder;
    if(pthread_create(&up_forwarder, NULL, forward_ether_data_to_vswitch, &vport) != 0)
    {
        ERROR_PRINT_THEN_EXIT("fail to pthread_create: %s\n", strerror(errno));
    }

    // Create downforwarding thread that runs forward_ether_data_to_vtap
    pthread_t down_forwarder;
    if(pthread_create(&down_forwarder, NULL, forward_ether_data_to_tap, &vport) != 0)
    {
        ERROR_PRINT_THEN_EXIT("fail to pthread_create: %s\n", strerror(errno));
    }

    // Wait for threads to finish
    if(pthread_join(up_forwarder, NULL) != 0 || pthread_join(down_forwarder, NULL) != 0)
    {
        ERROR_PRINT_THEN_EXIT("fail to pthread_join: %s\n", strerror(errno));
    }

    return 0;

}


void vport_init(struct vport_t *vport, const char *server_ip_str, int server_port)
{
    // TAP device
    char ifname[IFNAMSIZ] = "tapuyan";
    int tapfd = tap_alloc(ifname);
    if (tapfd < 0)
    {
        ERROR_PRINT_THEN_EXIT("fail to tap_alloc: %s\n", strerror(errno));
    }

    // Socket
    int vport_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (vport_sockfd < 0)
    {
        ERROR_PRINT_THEN_EXIT("fail to socket: %s\n", strerror(errno));
    }

    // Vswitch info
    struct sockaddr_in vswitch_addr;
    memset(&vswitch_addr, 0, sizeof(vswitch_addr))
    vswitch_addr.sin_family = AF_INET;
    vswitch_addr.sin_port = htons(server_port);


    if (inet_pton(AF_INET, server_ip_str, &vswitch_addr.sin_addr) != 1)
    {
        ERROR_PRINT_THEN_EXIT("fail to inet_pton: %s\n", strerror(errno));
    }

    vport->tapfd = tapfd;
    vport->vport_sockfd = vport_sockfd;
    vport->vswitch_addr = vswitch_addr;

    printf("[VPort] TAP device name: %s, VSwitch: %s:%d\n", ifname, server_ip_str, server_port);

}

void *forward_ether_data_to_tap(void *raw_vport) 
{

}

void *forward_ether_data_to_vswitch(void *raw_vport) 
{
    
}





