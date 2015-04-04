#pragma once

namespace base {

/**
 * @class shared_ptr
 * @brief Simple implementation of shared pointer idiom.
 */
template <typename T>
class shared_ptr
{
public:
    shared_ptr()
        : m_ptr(0)
        , m_ref_count(0)
    {
    }

    shared_ptr(T* t)
        : m_ptr(t)
        , m_ref_count(new int(1))
    {
    }

    shared_ptr(const T& t)
        : m_ptr(new T(t))
        , m_ref_count(new int(1))
    {
    }

    shared_ptr(const shared_ptr& p)
        : m_ptr(p.m_ptr)
        , m_ref_count(p.m_ref_count)
    {
        ++ (*m_ref_count);
    }

    shared_ptr& operator=(const shared_ptr& p)
    {
        clear_ptr();
        m_ptr = p.m_ptr;
        m_ref_count = p.m_ref_count;
        ++ (*m_ref_count);
        return *this;
    }

    ~shared_ptr()
    {
        clear_ptr();
    }

public:
    T& operator*()
    {
        return *m_ptr;
    }

    T* operator->()
    {
        return m_ptr;
    }

    const T& operator*() const
    {
        return *m_ptr;
    }

    const T* operator->() const
    {
        return m_ptr;
    }

private:
    void clear_ptr()
    {
        if (m_ptr) {
            -- (*m_ref_count);
            if ((*m_ref_count) == 0) {
                delete m_ptr;
                delete m_ref_count;
                m_ptr = 0;
                m_ref_count = 0;
            }
        }
    }

private:
    T* m_ptr;
    int* m_ref_count;
};

}
