#pragma once
#include <memory>
#include "network/Common.h"
#include "network/Connection.h"




template<typename MessageType>
class IClient
{
public:

	bool ConnectToServer(const std::string host, const uint16_t port) 
	{
		try
		{
			m_connection = std::make_unique<MessageType>();

			asio::ip::tcp::resolver resolver(m_context);
			auto endpoints = resolver.resolve(host,std::to_string(port));
			m_connection->ConnectToServer(endpoints);
			m_thread = std::move(std::thread([this] { m_ioContext.run(); }));
		}
		catch (const std::exception& e)
		{
			std::cout << "Client Exception: " << e.what() << std::endl;
			return false;
		}
		return true;
	}

	void Disconnect() 
	{
		if (IsConnected())
			return m_connection->IsConnected();
		else
			return false;
	}

	void IsConnected() 
	{
		if (m_connection)
			return m_connection->IsConnected();
		else
			return false;
	}

protected:
	asio::io_context m_ioContext;
	std::thread m_thread;

	asio::ip::tcp::socket m_socket;
	std::unique_ptr<Connection<MessageType>> m_connection;

private:
	TSQueue<OwnerMessage<MessageType>> m_messageIn;
};


























