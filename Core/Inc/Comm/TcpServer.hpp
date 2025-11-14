/**
 * @file TcpServer.hpp
 * @author Josef Aschwanden (josef.aschwanden@stud.hslu.ch)
 * @brief TCP Server Class
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

    /**
     * \brief Constructor, automatically opens socket
     * @param port port to open
     */
    TcpServer(uint16_t port);

    /**
     * @brief Opens socket at port
     * @param port port to open
     */
    err_t listen(uint16_t port);

    /**
     * @brief Returns current state
     */
    State getState() const;

    /**
     * @brief Sets a receive buffer
     * @param buffer buffer for received data
     * @param sizeBuffer size of the buffer
     */
    void receive(void *buffer, uint16_t sizeBuffer);

    /**
     * @brief Returns how many bytes have been received, 
     * once there are bytes received the 
     * buffer should be emptied
     * 
     * @return uint16_t number of received bytes
     */
    uint16_t getReceivedBytes() const;

    /**
     * @brief Writes data
     * @param data Data buffer
     * @param len length of buffer
     * @return err_t ERR_CONN (Listening) or ERR_CLSD (Closed socket) or ERR_OK (no error)
     */
    err_t write(const void* data, uint16_t len);


private:

    /**
     * @brief LwIP accept callback
     */
    static err_t accept(void *arg, struct tcp_pcb *newpcb, err_t err);

    /**
     * @brief LwIP recv callback
     */
    static err_t recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);

    /**
     * @brief Converts arg from LwIP callbacks to a this object
     * @note arg gets set to this when registering the callbacks
     * @param arg arg from LwIP
     * @return TcpServer* this
     */
    static TcpServer* thisFromArg(void *arg);

    err_t m_err;
    State m_state = State::CLOSED;

    tcp_pcb *m_pcb;
    tcp_pcb *m_connectionPcb;   // PCB of active connection (only one at a time)

    uint16_t m_receivedBytes = 0;   // received bytes in received buffer
    void *m_receiveBuffer = nullptr;
    uint16_t m_sizeReceiveBuffer = 0;


};


}