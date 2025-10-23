#include "application.h"
#include "main.h"
#include "lwip.h"
#include "gpio.h"
#include "Comm/UdpServer.hpp"
#include "lwip/pbuf.h"
#include "lwip/etharp.h"
#include "lwip/netif.h"
#include "lwip/ethip6.h"
#include "ethernetif.h"
#include <string.h>

/* Broadcast MAC address */
static const uint8_t broadcast_mac[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

/* Example Ethernet type */
#define ETH_TYPE_CUSTOM 0x88B5  // arbitrary custom type

void send_raw_ethernet_broadcast(struct netif *netif, const uint8_t *payload, uint16_t len)
{
    struct pbuf *p;
    struct eth_hdr *ethhdr;

    /* Allocate pbuf with room for Ethernet header + payload */
    p = pbuf_alloc(PBUF_RAW_TX, sizeof(struct eth_hdr) + len, PBUF_RAM);
    if (!p) return;

    /* Fill Ethernet header */
    ethhdr = (struct eth_hdr *)p->payload;
    SMEMCPY(&ethhdr->dest.addr, broadcast_mac, 6);
    SMEMCPY(&ethhdr->src.addr, netif->hwaddr, 6);
    ethhdr->type = lwip_htons(ETH_TYPE_CUSTOM);

    /* Copy payload after Ethernet header */
    memcpy((uint8_t*)ethhdr + sizeof(struct eth_hdr), payload, len);

    /* Send packet via the netif driver */
    netif->linkoutput(netif, p);

    pbuf_free(p);
}



extern "C" struct netif gnetif;  // defined by CubeMX-generated code

extern "C" int application(void){
    Comm::UdpServer udpserver;
    udpserver.udp_server_init();

    while(1) {
        MX_LWIP_Process();
    }
    return 0;
}