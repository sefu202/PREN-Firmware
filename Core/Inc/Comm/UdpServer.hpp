#include "lwip/udp.h"
#include "lwip/pbuf.h"
#include "lwip/ip.h"
#include "lwip/ip_addr.h"
#include "lwip/mem.h"
#include "lwip/err.h"

namespace Comm{


class UdpServer{
private:
    static void udp_receive_callback(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);

public:
    void udp_server_init(void);

};

    
}
