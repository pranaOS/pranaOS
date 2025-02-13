/**
 * @file rbtree.h
 * @author Krisna Pranav
 * @brief RB Tree
 * @version 6.0
 * @date 2024-10-20
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/concept.h>
#include <mods/error.h>
#include <mods/noncopyable.h>
#include <mods/kmalloc.h>

namespace Mods
{
    /**
     * @tparam K 
     */
    template <Integral K>
    class BaseRedBlackTree
    {
        MOD_MAKE_NONCOPYABLE(BaseRedBlackTree);
        MOD_MAKE_NONMOVABLE(BaseRedBlackTree);

    public:
        /**
         * @return size_t 
         */
        [[nodiscard]] size_t size() const
        {
            return m_size;
        }

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool is_empty() const
        {
            return m_size == 0;
        }

        enum class Color : bool
        {
            Red,
            Black
        }; // enum class Color : bool

        struct Node
        {
            Node* left_child{nullptr};
            Node* right_child{nullptr};
            Node* parent{nullptr};

            Color color{Color::Red};

            K key;

            /**
             * @brief Construct a new Node object
             * 
             * @param key 
             */
            Node(K key)
                : key(key)
            {
            }

            /**
             * @brief Construct a new Node object
             * 
             */
            Node()
            {
            }

            virtual ~Node() {};
        }; // struct Node

    protected:
        /**
         * @brief Construct a new Base Red Black Tree object
         * 
         */
        BaseRedBlackTree() = default; 

        /**
         * @brief Destroy the Base Red Black Tree object
         * 
         */
        virtual ~BaseRedBlackTree() = default;

        /**
         * @param subtree_root 
         */
        void rotate_left(Node* subtree_root)
        {
            VERIFY(subtree_root);
            auto* pivot = subtree_root->right_child;
            VERIFY(pivot);
            auto* parent = subtree_root->parent;

            subtree_root->right_child = pivot->left_child;

            if(subtree_root->right_child)
                subtree_root->right_child->parent = subtree_root;

            pivot->left_child = subtree_root;
            subtree_root->parent = pivot;

            pivot->parent = parent;
            if(!parent)
            {
                m_root = pivot;
            }
            else if(parent->left_child == subtree_root)
            { 
                parent->left_child = pivot;
            }
            else
            { 
                parent->right_child = pivot;
            }
        }

        /**
         * @param subtree_root 
         */
        void rotate_right(Node* subtree_root)
        {
            VERIFY(subtree_root);
            auto* pivot = subtree_root->left_child;
            VERIFY(pivot);
            auto* parent = subtree_root->parent;

            subtree_root->left_child = pivot->right_child;

            if(subtree_root->left_child)
                subtree_root->left_child->parent = subtree_root;

            pivot->right_child = subtree_root;
            subtree_root->parent = pivot;

            pivot->parent = parent;
            if(!parent)
            { 
                m_root = pivot;
            }
            else if(parent->left_child == subtree_root)
            { 
                parent->left_child = pivot;
            }
            else
            { 
                parent->right_child = pivot;
            }
        }

        /**
         * @param node 
         * @param key 
         * @return Node* 
         */
        static Node* find(Node* node, K key)
        {
            while(node && node->key != key)
            {
                if(key < node->key)
                {
                    node = node->left_child;
                }
                else
                {
                    node = node->right_child;
                }
            }
            return node;
        }

        /**
         * @param node 
         * @param key 
         * @return Node* 
         */
        static Node* find_largest_not_above(Node* node, K key)
        {
            Node* candidate = nullptr;
            while(node)
            {
                if(key == node->key)
                    return node;
                if(key < node->key)
                {
                    node = node->left_child;
                }
                else
                {
                    candidate = node;
                    node = node->right_child;
                }
            }
            return candidate;
        }

        /**
         * @param node 
         * @param key 
         * @return Node* 
         */
        static Node* find_smallest_not_below(Node* node, K key)
        {
            Node* candidate = nullptr;
            while(node)
            {
                if(node->key == key)
                    return node;

                if(node->key <= key)
                {
                    node = node->right_child;
                }
                else
                {
                    candidate = node;
                    node = node->left_child;
                }
            }
            return candidate;
        }

        /**
         * @param node 
         */
        void insert(Node* node)
        {
            VERIFY(node);
            Node* parent = nullptr;
            Node* temp = m_root;
            while(temp)
            {
                parent = temp;
                if(node->key < temp->key)
                    temp = temp->left_child;
                else
                    temp = temp->right_child;
            }
            if(!parent)
            {
                node->color = Color::Black;
                m_root = node;
                m_size = 1;
                m_minimum = node;
                return;
            }

            if(node->key < parent->key) 
                parent->left_child = node;
            else 
                parent->right_child = node;
            node->parent = parent;

            if(node->parent->parent) 
                insert_fixups(node);

            m_size++;
            if(m_minimum->left_child == node)
                m_minimum = node;
        }

        /**
         * @param node 
         */
        void insert_fixups(Node* node)
        {
            VERIFY(node && node->color == Color::Red);
            while(node->parent && node->parent->color == Color::Red)
            {
                auto* grand_parent = node->parent->parent;
                if(grand_parent->right_child == node->parent)
                {
                    auto* uncle = grand_parent->left_child;
                    if(uncle && uncle->color == Color::Red)
                    {
                        node->parent->color = Color::Black;
                        uncle->color = Color::Black;
                        grand_parent->color = Color::Red;
                        node = grand_parent;
                    }
                    else
                    {
                        if(node->parent->left_child == node)
                        {
                            node = node->parent;
                            rotate_right(node);
                        }
                        node->parent->color = Color::Black;
                        grand_parent->color = Color::Red;
                        rotate_left(grand_parent);
                    }
                }
                else
                {
                    auto* uncle = grand_parent->right_child;
                    if(uncle && uncle->color == Color::Red)
                    {
                        node->parent->color = Color::Black;
                        uncle->color = Color::Black;
                        grand_parent->color = Color::Red;
                        node = grand_parent;
                    }
                    else
                    {
                        if(node->parent->right_child == node)
                        {
                            node = node->parent;
                            rotate_left(node);
                        }
                        node->parent->color = Color::Black;
                        grand_parent->color = Color::Red;
                        rotate_right(grand_parent);
                    }
                }
            }
            m_root->color = Color::Black;
        }

        /**
         * @param node 
         */
        void remove(Node* node)
        {
            VERIFY(node);

            if(m_size == 1)
            {
                m_root = nullptr;
                m_minimum = nullptr;
                m_size = 0;
                return;
            }

            if(m_minimum == node)
                m_minimum = successor(node);

            if(node->left_child && node->right_child)
            {
                auto* successor_node = successor(node); 
                auto neighbour_swap = successor_node->parent == node;
                node->left_child->parent = successor_node;
                if(!neighbour_swap)
                    node->right_child->parent = successor_node;
                if(node->parent)
                {
                    if(node->parent->left_child == node)
                    {
                        node->parent->left_child = successor_node;
                    }
                    else
                    {
                        node->parent->right_child = successor_node;
                    }
                }
                else
                {
                    m_root = successor_node;
                }
                if(successor_node->right_child)
                    successor_node->right_child->parent = node;
                if(neighbour_swap)
                {
                    successor_node->parent = node->parent;
                    node->parent = successor_node;
                }
                else
                {
                    if(successor_node->parent)
                    {
                        if(successor_node->parent->left_child == successor_node)
                        {
                            successor_node->parent->left_child = node;
                        }
                        else
                        {
                            successor_node->parent->right_child = node;
                        }
                    }
                    else
                    {
                        m_root = node;
                    }
                    swap(node->parent, successor_node->parent);
                }
                swap(node->left_child, successor_node->left_child);
                if(neighbour_swap)
                {
                    node->right_child = successor_node->right_child;
                    successor_node->right_child = node;
                }
                else
                {
                    swap(node->right_child, successor_node->right_child);
                }
                swap(node->color, successor_node->color);
            }

            auto* child = node->left_child ?: node->right_child;

            if(child)
                child->parent = node->parent;
            if(node->parent)
            {
                if(node->parent->left_child == node)
                    node->parent->left_child = child;
                else
                    node->parent->right_child = child;
            }
            else
            {
                m_root = child;
            }

            if(node->color != Color::Red)
                remove_fixups(child, node->parent);

            m_size--;
        }

        /**
         * @param node 
         * @param parent 
         */
        void remove_fixups(Node* node, Node* parent)
        {
            while(node != m_root && (!node || node->color == Color::Black))
            {
                if(parent->left_child == node)
                {
                    auto* sibling = parent->right_child;
                    if(sibling->color == Color::Red)
                    {
                        sibling->color = Color::Black;
                        parent->color = Color::Red;
                        rotate_left(parent);
                        sibling = parent->right_child;
                    }
                    if((!sibling->left_child || sibling->left_child->color == Color::Black) && (!sibling->right_child || sibling->right_child->color == Color::Black))
                    {
                        sibling->color = Color::Red;
                        node = parent;
                    }
                    else
                    {
                        if(!sibling->right_child || sibling->right_child->color == Color::Black)
                        {
                            sibling->left_child->color = Color::Black; 
                            sibling->color = Color::Red;
                            rotate_right(sibling);
                            sibling = parent->right_child;
                        }
                        sibling->color = parent->color;
                        parent->color = Color::Black;
                        sibling->right_child->color = Color::Black; 
                        rotate_left(parent);
                        node = m_root; 
                    }
                }
                else
                {
                    auto* sibling = parent->left_child;
                    if(sibling->color == Color::Red)
                    {
                        sibling->color = Color::Black;
                        parent->color = Color::Red;
                        rotate_right(parent);
                        sibling = parent->left_child;
                    }
                    if((!sibling->left_child || sibling->left_child->color == Color::Black) && (!sibling->right_child || sibling->right_child->color == Color::Black))
                    {
                        sibling->color = Color::Red;
                        node = parent;
                    }
                    else
                    {
                        if(!sibling->left_child || sibling->left_child->color == Color::Black)
                        {
                            sibling->right_child->color = Color::Black; 
                            sibling->color = Color::Red;
                            rotate_left(sibling);
                            sibling = parent->left_child;
                        }
                        sibling->color = parent->color;
                        parent->color = Color::Black;
                        sibling->left_child->color = Color::Black; 
                        rotate_right(parent);
                        node = m_root; 
                    }
                }
                parent = node->parent;
            }
            node->color = Color::Black; 
        }

        /**
         * @param node 
         * @return Node* 
         */
        static Node* successor(Node* node)
        {
            VERIFY(node);
            
            if(node->right_child)
            {
                node = node->right_child;
                while(node->left_child)
                    node = node->left_child;
                return node;
            }
            auto temp = node->parent;
            while(temp && node == temp->right_child)
            {
                node = temp;
                temp = temp->parent;
            }
            return temp;
        }

        /**
         * @param node 
         * @return Node* 
         */
        static Node* predecessor(Node* node)
        {
            VERIFY(node);

            if(node->left_child)
            {
                node = node->left_child;
                while(node->right_child)
                    node = node->right_child;
                return node;
            }

            auto temp = node->parent;

            while(temp && node == temp->left_child)
            {
                node = temp;
                temp = temp->parent;
            }
            return temp;
        }

        Node* m_root{nullptr};
        size_t m_size{0};
        Node* m_minimum{nullptr}; 
    }; // class BaseRedBlackTree

    /**
     * @tparam TreeType 
     * @tparam ElementType 
     */
    template <typename TreeType, typename ElementType>
    class RedBlackTreeIterator
    {
    public:
        /**
         * @brief Construct a new Red Black Tree Iterator object
         * 
         */
        RedBlackTreeIterator() = default;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(RedBlackTreeIterator const& other) const
        {
            return m_node != other.m_node;
        }

        /**
         * @return RedBlackTreeIterator& 
         */
        RedBlackTreeIterator& operator++()
        {
            if(!m_node)
                return *this;
            m_prev = m_node;
            
            m_node = static_cast<typename TreeType::Node*>(TreeType::successor(m_node));
            return *this;
        }

        /**
         * @return RedBlackTreeIterator& 
         */
        RedBlackTreeIterator& operator--()
        {
            if(!m_prev)
                return *this;

            m_node = m_prev;
            m_prev = static_cast<typename TreeType::Node*>(TreeType::predecessor(m_prev));
            return *this;
        }

        /**
         * @return ElementType& 
         */
        ElementType& operator*()
        {
            return m_node->value;
        }

        /**
         * @return ElementType* 
         */
        ElementType* operator->()
        {
            return &m_node->value;
        }

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool is_end() const
        {
            return !m_node;
        }

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool is_begin() const
        {
            return !m_prev;
        }

        /**
         * @return auto 
         */
        [[nodiscard]] auto key() const
        {
            return m_node->key;
        }

    private:
        friend TreeType;

        /**
         * @brief Construct a new Red Black Tree Iterator object
         * 
         * @param node 
         * @param prev 
         */
        explicit RedBlackTreeIterator(typename TreeType::Node* node, typename TreeType::Node* prev = nullptr)
            : m_node(node), m_prev(prev)
        {
        }
        typename TreeType::Node* m_node{nullptr};
        typename TreeType::Node* m_prev{nullptr};
    }; // class RedBlackTreeIterator

    /**
     * @tparam K 
     * @tparam V 
     */
    template <Integral K, typename V>
    class RedBlackTree final : public BaseRedBlackTree<K>
    {
    public:
        /**
         * @brief Construct a new Red Black Tree object
         * 
         */
        RedBlackTree() = default;

        /**
         * @brief Destroy the Red Black Tree object
         * 
         */
        virtual ~RedBlackTree() override
        {
            clear();
        }

        using BaseTree = BaseRedBlackTree<K>;

        /**
         * @param key 
         * @return V* 
         */
        [[nodiscard]] V* find(K key)
        {
            auto* node = static_cast<Node*>(BaseTree::find(this->m_root, key));
            if(!node)
                return nullptr;
            return &node->value;
        }       

        /**
         * @param key 
         * @return V* 
         */
        [[nodiscard]] V* find_largest_not_above(K key)
        {
            auto* node = static_cast<Node*>(BaseTree::find_largest_not_above(this->m_root, key));
            if(!node)
                return nullptr;
            return &node->value;
        }

        /**
         * @param key 
         * @return V* 
         */
        [[nodiscard]] V* find_smallest_not_below(K key)
        {
            auto* node = static_cast<Node*>(BaseTree::find_smallest_not_below(this->m_root, key));
            if(!node)
                return nullptr;
            return &node->value;
        }

        /**
         * @param key 
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> try_insert(K key, V const& value)
        {
            return try_insert(key, V(value));
        }

        /**
         * @param key 
         * @param value 
         */
        void insert(K key, V const& value)
        {
            MUST(try_insert(key, value));
        }

        /**
         * @param key 
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> try_insert(K key, V&& value)
        {
            auto* node = new(nothrow) Node(key, move(value));
            if(!node)
                return Error::from_errno(ENOMEM);
            BaseTree::insert(node);
            return {};
        }

        /**
         * @param key 
         * @param value 
         */
        void insert(K key, V&& value)
        {
            MUST(try_insert(key, move(value)));
        }

        using Iterator = RedBlackTreeIterator<RedBlackTree, V>;
        friend Iterator;

        /**
         * @return Iterator 
         */
        Iterator begin()
        {
            return Iterator(static_cast<Node*>(this->m_minimum));
        }

        /**
         * @return Iterator 
         */
        Iterator end()
        {
            return {};
        }

        /**
         * @param key 
         * @return Iterator 
         */
        Iterator begin_from(K key)
        {
            return Iterator(static_cast<Node*>(BaseTree::find(this->m_root, key)));
        }

        using ConstIterator = RedBlackTreeIterator<const RedBlackTree, const V>;
        friend ConstIterator;

        /**
         * @return ConstIterator 
         */
        ConstIterator begin() const
        {
            return ConstIterator(static_cast<Node*>(this->m_minimum));
        }

        /**
         * @return ConstIterator 
         */
        ConstIterator end() const
        {
            return {};
        }

        /**
         * @param key 
         * @return ConstIterator 
         */
        ConstIterator begin_from(K key) const
        {
            return ConstIterator(static_cast<Node*>(BaseTree::find(this->m_root, key)));
        }

        /**
         * @param key 
         * @return ConstIterator 
         */
        ConstIterator find_largest_not_above_iterator(K key) const
        {
            auto node = static_cast<Node*>(BaseTree::find_largest_not_above(this->m_root, key));
            if(!node)
                return end();
            return ConstIterator(node, static_cast<Node*>(BaseTree::predecessor(node)));
        }

        /**
         * @param key 
         * @return ConstIterator 
         */
        ConstIterator find_smallest_not_below_iterator(K key) const
        {
            auto node = static_cast<Node*>(BaseTree::find_smallest_not_below(this->m_root, key));
            if(!node)
                return end();
            return ConstIterator(node, static_cast<Node*>(BaseTree::predecessor(node)));
        }

        /**
         * @param key 
         * @return V 
         */
        V unsafe_remove(K key)
        {
            auto* node = BaseTree::find(this->m_root, key);
            VERIFY(node);

            BaseTree::remove(node);

            V temp = move(static_cast<Node*>(node)->value);

            node->right_child = nullptr;
            node->left_child = nullptr;
            delete node;

            return temp;
        }

        /**
         * @param key 
         * @return true 
         * @return false 
         */
        bool remove(K key)
        {
            auto* node = BaseTree::find(this->m_root, key);

            if(!node)
                return false;

            BaseTree::remove(node);

            node->right_child = nullptr;
            node->left_child = nullptr;
            delete node;

            return true;
        }

        void clear()
        {
            delete this->m_root;
            this->m_root = nullptr;
            this->m_minimum = nullptr;
            this->m_size = 0;
        }

    private:
        struct Node : BaseRedBlackTree<K>::Node
        {

            V value;

            Node(K key, V value)
                : BaseRedBlackTree<K>::Node(key), value(move(value))
            {
            }

            ~Node()
            {
                delete this->left_child;
                delete this->right_child;
            }
        }; // struct Node : BaseRedBlackTree<K>::Node
    }; // class RedBlackTree final : public BaseRedBlackTree<K>
} // namespace Mods

using Mods::RedBlackTree;