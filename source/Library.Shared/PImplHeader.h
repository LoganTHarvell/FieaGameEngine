#pragma once

#include <memory>

namespace Library
{
    template<typename T>
    class PImpl
    {
    public:
        PImpl();
        ~PImpl();
        PImpl(const PImpl& rhs);
        PImpl& operator=(const PImpl& rhs);
        PImpl(PImpl&& rhs);
        PImpl& operator=(PImpl&& rhs);

        template<typename ...Args> PImpl(Args&& ...);

        T* operator->();
        const T* operator->() const;
        
        T& operator*();
        const T& operator*() const;

    private:
        std::unique_ptr<T> mImplementation;
    };
}