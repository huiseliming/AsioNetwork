#pragma once
#include <vector>
#include <iostream>

using MessageBody = std::vector<uint8_t>;

template<typename MessageType>
struct MessageHeader
{
public:
    MessageType id{};
    uint32_t size = 0;
};

template<typename MessageType>
struct Message
{
public:
    MessageHeader<MessageType> header;
    MessageBody body;
    size_t size() const 
    {
        return sizeof(MessageHeader<MessageType>) + body.size();
    }

    friend std::ostream& operator<<(std::ostream& os, const Message<MessageType>& msg)
    {
        os << "MessageId  : " << static_cast<int>(msg.header.id) <<std::endl 
           << "MessageSize: "<< msg.header.size << std::endl;
        return os;
    }

    template<typename DataType>
    friend Message<MessageType>& operator<<(Message<MessageType>& msg,const DataType& data)
    {
        static_assert(std::is_standard_layout<DataType>::value, "DataType must is standard layout");
        size_t i = msg.body.size();
        msg.body.resize(msg.body.size() + sizeof(DataType));
        std::memcpy(msg.body.data() + i, &data, sizeof(DataType));
        msg.header.size = static_cast<uint32_t>(msg.size());
        return msg;
    }

    template<typename DataType>
    friend Message<MessageType>& operator>>(Message<MessageType>& msg, DataType& data)
    {
        static_assert(std::is_standard_layout<DataType>::value, "DataType must is standard layout");
        size_t size = msg.body.size();
        size_t dataOffset = size - sizeof(DataType);
        std::memcpy(&data, msg.body.data() + dataOffset, sizeof(DataType));
        msg.body.resize(msg.body.size() - sizeof(DataType));
        msg.header.size = static_cast<uint32_t>(msg.size());
        return msg;
    }

};

template<typename MessageType>
class Connection;

template<typename MessageType>
struct OwnerMessage 
{
    std::shared_ptr<Connection<MessageType>> remote = nullptr;
    Message<MessageType> msg;
    friend std::ostream& operator<<(std::ostream& os, const OwnerMessage<MessageType>& msg)
    {
        os << msg.msg;
        return os; 
    }
};









