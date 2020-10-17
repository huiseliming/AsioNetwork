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

    std::ostream& operator<<(std::ostream& os,const Message<T>& msg)
    {
        os << static_cast<int>(msg.header.id) << msg.header.size;
        return os;
    }
    template<typename DataType>
    std::ostream& operator<<(Message<T>& msg,const DataType& msg)
    {
        static_assert(std::is_standard_layout<DataType>::value);
        return os;
    }
};
















