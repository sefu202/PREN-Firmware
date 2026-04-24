/**
 * @file TcpServer.cpp
 * @author Josef Aschwanden (josef.aschwanden@stud.hslu.ch)
 * @brief TCP Server
 * @version 0.1
 * @date 2025-10-23
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Comm/TcpServer.hpp"
#include <cassert>
#include <cstring> // std::memcpy


namespace Comm{

TcpServer::TcpServer(uint16_t port){
    listen(port);
}

err_t TcpServer::listen(uint16_t port)
{
    if (m_state == State::LISTEN) {
        return ERR_ALREADY;
    }
    if (m_state == State::ACCEPTED) {
        return ERR_ISCONN;
    }
    if (m_state != State::CLOSED) { // Fallback if any other states ever get added
        return ERR_USE;
    }

    m_pcb = tcp_new();
    if (!m_pcb) {
        m_err = ERR_MEM;
        return m_err;
    }

    tcp_arg(m_pcb, this);   // this pointer gets passed to LWIP callback functions as arg

    m_err = tcp_bind(m_pcb, IP_ADDR_ANY, port);
    if (m_err != ERR_OK) {
        tcp_abort(m_pcb);
        m_pcb = nullptr;

        return m_err;
    }

    tcp_pcb *newpcb = tcp_listen(m_pcb);

    if (newpcb) {
        m_pcb = newpcb;
    }
    else{
        tcp_abort(m_pcb);
        m_pcb = nullptr;
        return ERR_MEM;
    }

    m_state = State::LISTEN;

    tcp_accept(m_pcb, accept);

    return ERR_OK;
}

TcpServer::State TcpServer::getState() const {
    return m_state;
}

void TcpServer::receive(void *buffer, uint16_t sizeBuffer){
    assert(buffer != nullptr);

    if (buffer != nullptr && sizeBuffer != 0) {
        m_receiveBuffer = buffer;
        m_receivedBytes = 0;
        m_sizeReceiveBuffer = sizeBuffer;
    }
}

uint16_t TcpServer::getReceivedBytes() const {
    return m_receivedBytes;
}


err_t TcpServer::write(const void *data, uint16_t len)
{

    if (m_state == State::LISTEN){
        return ERR_CONN;
    }
    else if (m_state == State::CLOSED){
        return ERR_CLSD;
    }

    assert(m_state == State::ACCEPTED);

    tcp_write(m_connectionPcb, data, len, TCP_WRITE_FLAG_COPY);
    tcp_output(m_connectionPcb);

    return ERR_OK;
}

err_t TcpServer::accept(void *arg, struct tcp_pcb *newpcb, err_t err){
    if (!newpcb || err != ERR_OK) {
        return err;
    }

    assert(arg != nullptr);
    if (!arg){
        tcp_abort(newpcb);
        return ERR_ABRT;
    }

    // only one simultanous connection
    if (thisFromArg(arg)->m_state == State::ACCEPTED) {
        tcp_abort(newpcb);
        return ERR_ABRT;
    }

    tcp_arg(newpcb, arg);
    tcp_recv(newpcb, recv);

    thisFromArg(arg)->m_state = State::ACCEPTED;
    thisFromArg(arg)->m_connectionPcb = newpcb;

    return ERR_OK;
}

err_t TcpServer::recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err){

    assert(arg != nullptr);
    if (!arg){
        tcp_abort(tpcb);

        if (p){
            pbuf_free(p);
        }

        return ERR_ABRT;
    }

    // Connection closed, indicated by p == nullptr
    if (!p) {
        tcp_close(tpcb);
        thisFromArg(arg)->m_state = State::LISTEN;
        thisFromArg(arg)->m_connectionPcb = nullptr;
        return ERR_OK;
    }

    if (err != ERR_OK){
        if (p){
            pbuf_free(p);
        }

        tcp_abort(tpcb);
        thisFromArg(arg)->m_state = State::LISTEN;
        return ERR_ABRT;
    }

    // Receive buffer not yet ready => refuse data
    if (thisFromArg(arg)->m_receiveBuffer == nullptr || thisFromArg(arg)->m_receivedBytes != 0) {
        return ERR_INPROGRESS;
    }

    assert(p->len < thisFromArg(arg)->m_sizeReceiveBuffer);
    if (p->len < thisFromArg(arg)->m_sizeReceiveBuffer) {
        std::memcpy(thisFromArg(arg)->m_receiveBuffer, p->payload, p->len);
        thisFromArg(arg)->m_receivedBytes = p->len;
    }
    else {
        assert(0);
        // todo this would lead to data loss
    }

    tcp_recved(tpcb, p->len);

    pbuf_free(p);

    return ERR_OK;
}


TcpServer* TcpServer::thisFromArg(void *arg) {
    return static_cast<TcpServer*>(arg);
}



}