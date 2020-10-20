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
    }m_owner = kServer;
public:
    Connection(Owner owner, asio::io_context& asioContext, asio::ip::tcp::socket socket, ThreadSafeDeque<OwnerMessage<MessageType>>& messageIn)
        : m_owner(owner)
        , m_ioContext(asioContext)
        , m_writeStand(asioContext)
        , m_socket(std::move(socket))
        , m_messageIn(messageIn)
    {}

    virtual ~Connection()
    {

    }

    bool ConnectToServer(const asio::ip::tcp::resolver::results_type& endpoints)
    {
        assert(m_owner == Owner::kClient);

        asio::async_connect(m_socket, endpoints, 
            [this](std::error_code ec, asio::ip::tcp::endpoint endpoint)
            {
                if (!ec) 
                {
                    ReadHeader();
                }
            });
        return true;
    }

    bool ConnectToClient(uint32_t id)
    { 
        assert(m_owner == Owner::kServer);
        if (m_socket.is_open()) 
        {
            m_id = id;
            ReadHeader();
            return true;
        }
        return false;
    }

    void DisConnect() 
    {
        if (IsConnected())
        {
            asio::post(m_ioContext, [this]() { m_socket.close(); });
        }
    }

    bool IsConnected() const { return m_socket.is_open(); }
    
    bool GetID() { return m_id; }

    bool Send(Message<MessageType>&& msg) 
    {
        m_writeStand.post(
            [this, msg = std::forward<Message<MessageType>>(msg)] {
                m_messageOut.push_back(msg);
                if (m_messageOut.size() <= 1)
                {
                    WriteHeader();
                }
            });
    }

private:
    void ReadHeader() 
    {
        asio::async_read(m_socket, asio::buffer(&m_messageTemporaryIn.header,sizeof(MessageHeader<MessageType>)),
            [this](std::error_code ec, std::size_t length)
            {
                if (!ec)
                {
                    if (m_messageTemporaryIn.header.size > 0)
                    {
                        m_messageTemporaryIn.body.resize(m_messageTemporaryIn.header.size);
                        ReadBody();
                    }
                    else 
                    {
                        AddToIncomingMessageQueue();
                        ReadHeader();
                    }
                }
                else
                {
                    std::cout << "[Server] ReaderHeader Failed!" << std::endl;
                    m_socket.close();
                }
            });
    }

    void ReadBody() 
    {
        asio::async_read(m_socket, asio::buffer(m_messageTemporaryIn.body.data(), m_messageTemporaryIn.header.size),
            [this](std::error_code ec, std::size_t length)
            {
                if (!ec)
                {
                        AddToIncomingMessageQueue();
                        ReadHeader();
                }
                else
                {
                    std::cout << "[Server] ReadBody Failed!" << std::endl;
                    m_socket.close();
                }
            });
    }

    void WriteHeader()
    {
        asio::async_write(m_socket, asio::buffer(&m_messageOut.front().header, sizeof(MessageHeader<MessageType>)),
            m_writeStand.wrap([this](std::error_code ec, std::size_t length)
            {
                if (!ec)
                {
                    if (m_messageOut.front().body.size() > 0)
                    {
                        WriteBody();
                    }
                    else
                    {
                        m_messageOut.pop_front();
                        if (!m_messageOut.empty())
                        {
                            WriteHeader();
                        }
                    }
                }
                else
                {
                    std::cout << "[Server] WriteHeader Failed!" << std::endl;
                    m_socket.close();
                }
            }));
    }

    void WriteBody()
    {
        asio::async_write(m_socket, asio::buffer(m_messageOut.front().body.data(), m_messageOut.front().body.size()),
            m_writeStand.wrap([this](std::error_code ec, std::size_t length)
            {
                if (!ec)
                {
                    m_messageOut.pop_front();
                    if (!m_messageOut.empty())
                    {
                        WriteHeader();
                    }
                }
                else
                {
                    std::cout << "[Server] WriteBody Failed!" << std::endl;
                    m_socket.close();
                }
            }));
    }

    void AddToIncomingMessageQueue()
    {
        if(m_owner == Owner::kServer)
            m_messageIn.push_back({ this->shared_from_this(), m_messageTemporaryIn });
        else
            m_messageIn.push_back({ nullptr, m_messageTemporaryIn });
    }

protected:
    asio::io_context& m_ioContext;
    asio::io_context::strand m_writeStand;
    asio::ip::tcp::socket m_socket;
    
private:
    std::deque<Message<MessageType>> m_messageOut;
    ThreadSafeDeque<OwnerMessage<MessageType>>& m_messageIn;
    uint32_t m_id = 0;

    
    Message<MessageType> m_messageTemporaryIn;
};










