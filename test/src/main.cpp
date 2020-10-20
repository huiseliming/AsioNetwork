#include "AsioNetwork.h"
#include <iostream>
#include <gtest/gtest.h>
#include "network/Message.h"
#include "network/IClient.h"
#include "network/IServer.h"

TEST(AsioNetworkTest,MessageIOStreamTest)
{
	enum class CustomMessageType : uint32_t {
		kMessageTypeNoData,
		kMessageTypeHeartbeat,
		kMessageTypeMaxCount
	};

	Message<CustomMessageType> message;

	int a = 1;
	bool b = true;
	float c = 3.1414926f;

	struct XXX{
		float x;
		float y;
	} d[5];
	for (size_t i = 0; i < 5; i++)
	{
		d[i].x = 1;
		d[i].y = 1;
	}
	message << a << b << c << d;
	a = 0;
	b = false;
	c = 1.0;
	for (size_t i = 0; i < 5; i++)
	{
		d[i].x = 2;
		d[i].y = 2;
	}
	message >> d >> c >> b >> a;
	ASSERT_EQ(a, 1);
	ASSERT_EQ(b, true);
	ASSERT_EQ(c, 3.1414926f);
	for (size_t i = 0; i < 5; i++)
	{
		ASSERT_EQ(d[i].x, 1);
		ASSERT_EQ(d[i].y, 1);
	}

}


TEST(AsioNetworkTest, ConnectTest)
{
	enum class CustomMessageType : uint32_t {
		kUnknow,
		kServerAccept,
		kServerDeny,
		kServerPing,
		kMessageToServer,
		kMessageToClient,
		kMessageAll,
		kMaxCount
	};


	class CustomClient : public IClient<CustomMessageType>
	{
	public:
		using Message = Message<CustomMessageType>;
	protected:
		virtual void OnMessage(std::shared_ptr<Connection<CustomMessageType>> client, Message msg) override
		{
			switch (msg.header.id)
			{
			case CustomMessageType::kServerAccept:
				std::cout << "[Client] ServerAccept" << std::endl;
				PingServer();
				break;
			case CustomMessageType::kServerDeny:
				std::cout << "[Client] ServerDenyr" << std::endl;
				break;
			case CustomMessageType::kServerPing:
			{
				std::cout << "[Client] ServerPing" << std::endl;
				std::chrono::system_clock::time_point receiveTime = std::chrono::system_clock::now();
				std::chrono::system_clock::time_point sendTime;
				msg >> sendTime;
				std::cout << "Ping: " << std::chrono::duration<double>(receiveTime - sendTime).count() << std::endl;
			}
				break;
			default:
				break;
			}
		}

		void PingServer()
		{
			Message msg;
			msg.header.id = CustomMessageType::kServerPing;
			msg << std::chrono::system_clock::now();
			MessageServer(std::move(msg));
		}
	};

	class CustomServer : public IServer<CustomMessageType>
	{
	public:
		using Message = Message<CustomMessageType>;
		using IServer = IServer<CustomMessageType>;
	public:
		CustomServer(uint16_t port)
			:IServer(port)
		{
		}
	protected:

		virtual bool OnClientConnect(std::shared_ptr<Connection<CustomMessageType>> client) override
		{
			std::cout << "CustomServer ClientConnect" << std::endl;
			Message msg;
			msg.header.id = CustomMessageType::kServerAccept;
			MessageClient(client, std::move(msg));
			return true;
		}

		virtual void OnClientDisconnect(std::shared_ptr<Connection<CustomMessageType>> client) override
		{
			std::cout << "CustomServer ClientDisconnect" << std::endl;
		}

		virtual void OnMessage(std::shared_ptr<Connection<CustomMessageType>> client, Message msg) override
		{
			switch (msg.header.id)
			{
			case CustomMessageType::kServerPing:
				std::cout << "[Server] ServerPing" << std::endl;
				msg << std::chrono::system_clock::now();
				MessageClient(client,std::move(msg));
				break;
			default:
				break;
			}
		}
	};
	CustomServer server(53330);
	server.Start();
	std::thread t1([&] {
			while (true)
			{
				server.Update();
			}
		});
	CustomClient client;
	client.Connect("127.0.0.1", 53330);
	std::thread t2([&] {
			while (true)
			{
				client.Update();
			}
		});
	t1.join();
	t2.join();
}