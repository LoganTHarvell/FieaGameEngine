#pragma once

#include "PImplHeader.h"

namespace Library
{
    template<typename T>
    PImpl<T>::PImpl() : mImplementation(std::make_unique<T>()) 
    { 
    }

    template<typename T>
    PImpl<T>::~PImpl() = default;

    template<typename T>
    PImpl<T>::PImpl(const PImpl& rhs) :
        mImplementation(std::make_unique(rhs.mImplementation ? *rhs : nullptr))
    {
    }

    template<typename T>
    PImpl<T>& PImpl<T>::operator=(const PImpl& rhs)
    {
        if (this == &rhs) return *this;

        mImplementation = std::make_unique<T>(rhs.mImplementation ? *rhs : nullptr);
        return *this;
    }

    template<typename T>
    PImpl<T>::PImpl(PImpl&& rhs) = default;

    template<typename T>
    PImpl<T>& PImpl<T>::operator=(PImpl&& rhs) = default;

    template<typename T>
    template<typename ...Args>
    PImpl<T>::PImpl(Args&& ...args) :
        mImplementation(std::make_unique<T>(std::forward<Args>(args)...)) 
    { 
    }

    template<typename T>
    T* PImpl<T>::operator->() 
    { 
        return mImplementation.get(); 
    }

    template<typename T>
    const T* PImpl<T>::operator->() const
    { 
        return mImplementation.get(); 
    }

    template<typename T>
    T& PImpl<T>::operator*() 
    {
        return *mImplementation.get(); 
    }

    template<typename T>
    const T& PImpl<T>::operator*() const
    {
        return *mImplementation.get(); 
    }
}