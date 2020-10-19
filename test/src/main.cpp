#include "AsioNetwork.h"
#include <iostream>
#include "network/Message.h"
#include "gtest/gtest.h"


enum class MessageType: uint32_t{
	kMessageTypeNoData,
	kMessageTypeHeartbeat,
	kMessageTypeMaxCount
};

TEST(AsioNetworkTest,MessageIOStreamTest)
{
	Message<MessageType> message;

	int a = 1;
	bool b = true;
	float c = 3.1414926f;

	struct {
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
