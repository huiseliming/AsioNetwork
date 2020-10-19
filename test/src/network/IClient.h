#pragma once
#include <memory>
#include "network/Common.h"
#include "network/Connection.h"




template<typename MessageType>
class IClient
{
public:

	IClient()
	{

	}

	virtual ~IClient() 
	{
		Disconnect();
	}

	bool ConnectToServer(const std::string host, const uint16_t port) 
	{
		try
		{
			m_connection = std::make_unique<Connection<MessageType>>(Connection<MessageType>::Owner::kClient, m_ioContext, std::move(m_socket), m_messageIn);
			asio::ip::tcp::resolver resolver(m_ioContext);
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
			m_connection->DisConnect();
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


























