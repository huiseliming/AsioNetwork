#pragma once
#include <memory>
#include <mutex>

template<typename T>
class Singleton
{
private:
    Singleton<T>(const Singleton<T>&) = delete;
    Singleton<T>& operator = (const Singleton<T>&) = delete;
    Singleton<T>(Singleton<T>&&) = delete;
    Singleton<T>& operator = (Singleton<T>&&) = delete;

    Singleton<T>() = default;
    static std::unique_ptr<T> pInstance;
    static std::once_flag onceFlag;

public:
    virtual ~Singleton<T>() = default;

    static T& GetInstance()
    {
        std::call_once(onceFlag, []() {
            pInstance.reset(new T);
            });
        return *pInstance.get();
    }

    template<typename... Args>
    static T& GetInstance(Args&& ...args)
    {
        std::call_once(onceFlag, [&]() {
            pInstance.reset(new T(std::forward<Args>(args)...));
            });
        return *pInstance.get();
    }
};

template<typename T>
std::unique_ptr<T> Singleton<T>::pInstance;
template<typename T>
std::once_flag Singleton<T>::onceFlag;
