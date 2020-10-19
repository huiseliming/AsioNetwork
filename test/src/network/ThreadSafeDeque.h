#pragma once

template<typename T>
class ThreadSafeDeque;

template<typename T>
using TSQueue = ThreadSafeDeque<T>;

template<typename T>
class ThreadSafeDeque
{
public:
    ThreadSafeDeque() = default;
    ThreadSafeDeque(const ThreadSafeDeque&) = delete;
    ThreadSafeDeque& operator=(const ThreadSafeDeque&) = delete;
    virtual ~ThreadSafeDeque() { m_deque.clear(); }

    const T& Front()
    {
        std::scoped_lock<std::mutex> lock(m_mutex);
        return m_deque.front();
    }
    const T& Back()
    {
        std::scoped_lock<std::mutex> lock(m_mutex);
        return m_deque.back();
    }

    void PushFront(const T& item)
    {
        std::scoped_lock<std::mutex> lock(m_mutex);
        return m_deque.push_front(item);
    }

    void PushBack(const T& item)
    {
        std::scoped_lock<std::mutex> lock(m_mutex);
        return m_deque.push_back(item);
    }

    void EmplaceFront(T&& item)
    {
        std::scoped_lock<std::mutex> lock(m_mutex);
        return m_deque.emplace_front(std::forward<T>(item));
    }

    void EmplaceBack(T&& item)
    {
        std::scoped_lock<std::mutex> lock(m_mutex);
        return m_deque.emplace_back(std::forward<T>(item));
    }

    void Empty()
    {
        std::scoped_lock<std::mutex> lock(m_mutex);
        return m_deque.empty();
    }

    void Size()
    {
        std::scoped_lock<std::mutex> lock(m_mutex);
        return m_deque.size();
    }

    void Clear()
    {
        std::scoped_lock<std::mutex> lock(m_mutex);
        return m_deque.clear();
    }

    T PopFront()
    {
        std::scoped_lock<std::mutex> lock(m_mutex);
        auto front = std::move(m_deque.front());
        m_deque.pop_front();
        return front;
    }

    T PopBack()
    {
        std::scoped_lock<std::mutex> lock(m_mutex);
        auto back = std::move(m_deque.back());
        m_deque.pop_back();
        return back;
    }

protected:
    std::deque<T> m_deque;
    std::mutex m_mutex;
};






