#pragma once 

namespace uqac
{
    template<typename T>
    class span
    {
    private:
        T* m_ptr;
        size_t m_size;
    public:
        span() : ptr(nullptr), m_size(0) {}
        span(T* ptr, size_t size) : m_ptr(ptr), m_size(size) {}
        span(T* ptr, T* end) : m_ptr(ptr), m_size(end - ptr) {}
        span(const span& other) : m_ptr(other.m_ptr), m_size(other.m_size) {}
        span(span&& other) : m_ptr(other.m_ptr), m_size(other.m_size) { other.m_ptr = nullptr; other.m_size = 0; }
        ~span() { }

        span& operator=(const span& other) { m_ptr = other.m_ptr; m_size = other.m_size; return *this; }
        span& operator=(span&& other) { m_ptr = other.m_ptr; m_size = other.m_size; other.m_ptr = nullptr; other.m_size = 0; return *this; }

        T& operator[](size_t index) { return m_ptr[index]; }
        const T& operator[](size_t index) const { return m_ptr[index]; }

        T* begin() { return m_ptr; }
        T* end() { return m_ptr + m_size; }
        const T* begin() const { return m_ptr; }
        const T* end() const { return m_ptr + m_size; }

        size_t size() const { return m_size; }
        bool empty() const { return m_size == 0; }
    };
}
