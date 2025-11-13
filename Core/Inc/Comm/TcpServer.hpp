/**
 * @file TcpServer.hpp
 * @author Josef Aschwanden (josef.aschwanden@stud.hslu.ch)
 * @brief TCP Server example program
 * @version 0.1
 * @date 2025-10-23
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "lwip/tcp.h"


namespace Comm{


class TcpServer {
public:
    enum class State{CLOSED, LISTEN, ACCEPTED};

    TcpServer(uint16_t port);

    err_t listen(uint16_t port);

    State getState() const;

    err_t write(const void* data, uint16_t len);


private:
    static err_t accept(void *arg, struct tcp_pcb *newpcb, err_t err);

    static err_t recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);

    static TcpServer* thisFromArg(void *arg);

    //void cleanup(struct tcp_pcb *pcb);

    err_t m_err;
    State m_state = State::CLOSED;

    tcp_pcb *m_pcb;
    tcp_pcb *m_connectionPcb;


};


}