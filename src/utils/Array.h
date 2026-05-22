#pragma once

namespace bb
{
    template <typename T>
    class Array
    {
    private:
        T* m_data;
        size_t m_numActiveElements;
        size_t m_capacity;

        Array();
        ~Array();
    };

    template <typename T>
    Array<T>::Array()
    {
        // Allocate space on heap
    }

    template <typename T>
    Array<T>::~Array()
    {
        // De-allocate space on heap
    }
}