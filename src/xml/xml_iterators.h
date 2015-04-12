#pragma once

#include "xml.h"

namespace xml
{

//! Iterator of child nodes of node
template<class Ch = char>
class node_iterator
{

public:

    typedef node<Ch> value_type;
    typedef node<Ch> &reference;
    typedef node<Ch> *pointer;
    typedef std::ptrdiff_t difference_type;
    typedef std::bidirectional_iterator_tag iterator_category;

    node_iterator()
        : m_node(0)
    {
    }

    node_iterator(node<Ch> *node)
        : m_node(node->first_node())
    {
    }

    reference operator *() const
    {
        assert(m_node);
        return *m_node;
    }

    pointer operator->() const
    {
        assert(m_node);
        return m_node;
    }

    node_iterator& operator++()
    {
        assert(m_node);
        m_node = m_node->next_sibling();
        return *this;
    }

    node_iterator operator++(int)
    {
        node_iterator tmp = *this;
        ++this;
        return tmp;
    }

    node_iterator& operator--()
    {
        assert(m_node && m_node->previous_sibling());
        m_node = m_node->previous_sibling();
        return *this;
    }

    node_iterator operator--(int)
    {
        node_iterator tmp = *this;
        ++this;
        return tmp;
    }

    bool operator ==(const node_iterator<Ch> &rhs) const
    {
        return m_node == rhs.m_node;
    }

    bool operator !=(const node_iterator<Ch> &rhs) const
    {
        return m_node != rhs.m_node;
    }

private:

    node<Ch> *m_node;

};

//! Iterator of child attributes of node
template<class Ch = char>
class attribute_iterator
{

public:

    typedef attribute<Ch> value_type;
    typedef attribute<Ch> &reference;
    typedef attribute<Ch> *pointer;
    typedef std::ptrdiff_t difference_type;
    typedef std::bidirectional_iterator_tag iterator_category;

    attribute_iterator()
        : m_attribute(0)
    {
    }

    attribute_iterator(node<Ch> *node)
        : m_attribute(node->first_attribute())
    {
    }

    reference operator *() const
    {
        assert(m_attribute);
        return *m_attribute;
    }

    pointer operator->() const
    {
        assert(m_attribute);
        return m_attribute;
    }

    attribute_iterator& operator++()
    {
        assert(m_attribute);
        m_attribute = m_attribute->next_attribute();
        return *this;
    }

    attribute_iterator operator++(int)
    {
        attribute_iterator tmp = *this;
        ++this;
        return tmp;
    }

    attribute_iterator& operator--()
    {
        assert(m_attribute && m_attribute->previous_attribute());
        m_attribute = m_attribute->previous_attribute();
        return *this;
    }

    attribute_iterator operator--(int)
    {
        attribute_iterator tmp = *this;
        ++this;
        return tmp;
    }

    bool operator ==(const attribute_iterator<Ch> &rhs)
    {
        return m_attribute == rhs.m_attribute;
    }

    bool operator !=(const attribute_iterator<Ch> &rhs)
    {
        return m_attribute != rhs.m_attribute;
    }

private:

    attribute<Ch> *m_attribute;

};

}
