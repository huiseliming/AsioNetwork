#pragma once
#include <vector>
#include <iostream>

using MessageBody = std::vector<uint8_t>;

template<typename T>
struct MessageHeader
{
public:
    T id{};
    uint32_t size = 0;
};

template<typename T>
struct Message
{
public:
    MessageHeader<T> header;
    MessageBody body;
    size_t size() const 
    {
        return sizeof(MessageHeader<T>) + body.size(); 
    }

    friend std::ostream& operator<<(std::ostream& os, const Message<T>& msg)
    {
        os << static_cast<int>(msg.header.id) << msg.header.size;
        return os;
    }

    template<typename DataType>
    friend Message<T>& operator<<(Message<T>& msg,const DataType& data)
    {
        static_assert(std::is_standard_layout<DataType>::value, "DataType不能有指针或者引用");
        size_t i = msg.body.size();
        msg.body.resize(msg.body.size() + sizeof(DataType));
        std::memcpy(msg.body.data() + i, &data, sizeof(DataType));
        msg.header.size = msg.size();
        return msg;
    }

    template<typename DataType>
    friend Message<T>& operator>>(Message<T>& msg, DataType& data)
    {
        static_assert(std::is_standard_layout<DataType>::value, "DataType不能有指针或者引用");
        size_t size = msg.body.size();
        size_t dataOffset = size - sizeof(DataType);
        std::memcpy(&data, msg.body.data() + dataOffset, sizeof(DataType));
        msg.body.resize(msg.body.size() - sizeof(DataType));
        msg.header.size = msg.size();
        return msg;
    }

};












