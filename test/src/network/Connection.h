#pragma once
#include <deque>
#include "Common.h"
#include "Message.h"
#include "ThreadSafeDeque.h"

template<typename MessageType>
class Connection :public std::enable_shared_from_this<Connection<MessageType>>
{
public:
    enum class Owner {
        kServer,
        kClient
    }m_owner;
public:
    Connection(Owner owner, asio::io_context& asioContext, asio::ip::tcp::socket socket, ThreadSafeDeque<Message<MessageType>>& messageIn)
        : m_owner(owner)
        , m_asioContext(asioContext)
        , m_writeStand(asioContext)
        , m_socket(std::move(socket))
        , m_messageIn(messageIn);
    {}
    virtual ~Connection()
    {}

    bool ConnectToServer();
    bool ConnectToClient();
    bool DisConnect();
    bool IsConnected() const ;

    bool Send(Message<MessageType>& msg) 
    {
    }

protected:
    asio::io_context& m_asioContext;
    asio::io_context::strand m_writeStand;
    asio::ip::tcp::socket m_socket;
    
private:
    std::deque<Message<MessageType>> m_messageOut;
    ThreadSafeDeque<Message<MessageType>>& m_messageIn;

};










