#include "Comm/UdpServer.hpp"
#include <cstring>
#include <vector>

#define UDP_LOCAL_PORT  30000

namespace Comm{

void UdpServer::udp_receive_callback(void *arg, struct udp_pcb *pcb,
                                 struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    if (p != NULL) {
        // Print or handle received data
        printf("Received %d bytes from %s:%d\n", p->len, ipaddr_ntoa(addr), port);

        // Echo it back to sender
        udp_sendto(pcb, p, addr, port);
        HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);

        // Free the received pbuf
        pbuf_free(p);
    }
}

void UdpServer::udp_server_init(void)
{
    struct udp_pcb *pcb;
    err_t err;

    // Create new UDP control block
    pcb = udp_new();
    if (!pcb) {
        printf("Failed to create UDP PCB\n");
        return;
    }

    // Bind to any IP address and UDP_LOCAL_PORT
    err = udp_bind(pcb, IP_ADDR_ANY, UDP_LOCAL_PORT);
    if (err != ERR_OK) {
        printf("UDP bind failed: %d\n", err);
        udp_remove(pcb);
        return;
    }

    // Set receive callback
    udp_recv(pcb, udp_receive_callback, this);

    printf("UDP echo server started on port %d\n", UDP_LOCAL_PORT);
}

}