#pragma once
#include "network/Common.h"



template<typename MessageType>
class IServer
{
public:
	IServer()
		: m_acceptor(m_ioContext)
	{
	
	}

	virtual ~IServer()
	{
		Stop();
	}

	bool Start() 
	{
		try
		{
			WaitForClientConnection();
		}
		catch (const std::exception& e)
		{
			std::cout << "[Server] Exception: " << e.what() << std::endl;
			return false;
		}
		return true;
	}

	void Stop() 
	{
		m_ioContext.stop();

		if (m_thread.joinable())
			m_thread.join();
		std::cout << "[Server] Stopped!" << std::endl;
	}

	void WaitForClientConnection() 
	{
		m_acceptor.async_accept(
			[this] (std::error_code ec, asio::ip::tcp::socket socket)
			{
				if (!ec)
				{
					auto remote_endpoint = socket.remote_endpoint();
					std::cout << "[Server] New Connection: " << remote_endpoint << std::endl;
					std::shared_ptr<Connection<MessageType>> newConnection =
						std::make_shared<Connection<MessageType>>(Connection<MessageType>::Owner::kServer, m_ioContext,std::move(socket), m_messageIn);
					if (OnClientConnect(newConnection))
					{
						m_connections.push_back(std::move(newConnection));
						m_connections.back()->ConnectToClient(m_idCounter++);
						std::cout << "[Server] " << remote_endpoint <<" [ID:"<<m_connections.back()->GetID() << "] Connection Approved" << std::endl;
					}
					else
					{
						std::cout << "[Server] " << remote_endpoint << " Connection Denied" << std::endl;
					}
				}
				else
				{
					std::cout << "[Server] New Connectsion Error: " << ec.message() << std::endl;
				}
				WaitForClientConnection();
			});
	}

	void MessageClient(std::shared_ptr<Connection<MessageType>> client ,Message<MessageType> msg)
	{
		if (client && client->IsConnected())
		{
			Client->Send(msg);
		}
		else
		{
			OnClientDisconnect(client);
			client.reset();
			m_connections.erase(std::remove(std::begin(m_connections),std::end(m_connections),client),std::end(m_connections));
		}
	}

	void MessageAllClients(Message<MessageType> msg, std::shared_ptr<Connection<MessageType>> pIgnoreClient = nullptr)
	{
		bool bInvalidClientExists;
		for (auto& client, m_connections) 
		{
			if (client && client->IsConnected())
			{
				if (client != pIgnoreClient);
					client->Send(msg);
			}
			else
			{
				OnClientDisconnect();
				client.reset();
				bInvalidClientExists = true;
			}
		}
		if (bInvalidClientExists)
		{
			m_connections.erase(std::remove(std::begin(m_connections), std::end(m_connections), nullptr), std::end(m_connections));
		}
	}

	void Update(size_t maxMessages = -1)
	{
		size_t messageCount = 0;
		while (messageCount < maxMessages && !m_messageIn.Empty())
		{
			auto msg = m_messageIn.PopFront();
			OnMessage(msg.remote, msg.msg);
			messageCount++;
		}
	}

protected:
	virtual bool OnClientConnect(std::shared_ptr<Connection<MessageType>> client) 
	{
		return false;
	}

	virtual void OnClientDisconnect(std::shared_ptr<Connection<MessageType>> client)
	{
	}

	virtual void OnMessage(std::shared_ptr<Connection<MessageType>> client, Message<MessageType> msg)
	{
	}

	TSQueue<OwnerMessage<MessageType>> m_messageIn;
	std::deque<std::shared_ptr<Connection<MessageType>>> m_connections;
	asio::io_context m_ioContext;
	std::thread m_thread;

	asio::ip::tcp::acceptor m_acceptor;
	uint32_t m_idCounter = 0;
};
































