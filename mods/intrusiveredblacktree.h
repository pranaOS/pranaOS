/**
 * @file intrusiverbtree.h
 * @author Krisna Pranav
 * @brief Intrusive RB Tree
 * @version 6.0
 * @date 2024-10-20
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/intrusivedetails.h>
#include <mods/redblacktree.h>

namespace Mods::Detail
{
    /**
     * @tparam K 
     * @tparam V 
     * @tparam Container 
     */
    template <Integral K, typename V, typename Container = RawPtr<V>>
    class IntrusiveRedBlackTreeNode;

    struct ExtractIntrusiveRedBlackTreeTypes
    {
        /**
         * @tparam K 
         * @tparam V 
         * @tparam Container 
         * @tparam T 
         * @param x 
         * @return K 
         */
        template <typename K, typename V, typename Container, typename T>
        static K key(IntrusiveRedBlackTreeNode<K, V, Container> T::* x);

        /**
         * @tparam K 
         * @tparam V 
         * @tparam Container 
         * @tparam T 
         * @param x 
         * @return V 
         */
        template <typename K, typename V, typename Container, typename T>
        static V value(IntrusiveRedBlackTreeNode<K, V, Container> T::* x);

        /**
         * @tparam K 
         * @tparam V 
         * @tparam Container 
         * @tparam T 
         * @param x 
         * @return Container 
         */
        template <typename K, typename V, typename Container, typename T>
        static Container container(IntrusiveRedBlackTreeNode<K, V, Container> T::* x);
    }; // struct ExtractIntrusiveRedBlackTreeTypes

    /**
     * @tparam K 
     * @tparam V 
     * @tparam Container 
     */
    template <Integral K, typename V, typename Container = RawPtr<V>>
    using SubstitutedIntrusiveRedBlackTreeNode = IntrusiveRedBlackTreeNode<K, V, typename Detail::SubstituteIntrusiveContainerType<V, Container>::Type>;

    /**
     * @tparam K 
     * @tparam V 
     * @tparam Container 
     * @tparam member 
     */
    template <Integral K, typename V, typename Container, SubstitutedIntrusiveRedBlackTreeNode<K, V, Container> V::* member>
    class IntrusiveRedBlackTree : public BaseRedBlackTree<K>
    {
    public:
        /**
         * @brief Construct a new Intrusive Red Black Tree object
         * 
         */
        IntrusiveRedBlackTree() = default;

        virtual ~IntrusiveRedBlackTree() override
        {
            clear();
        }

        using BaseTree = BaseRedBlackTree<K>;
        using TreeNode = SubstitutedIntrusiveRedBlackTreeNode<K, V, Container>;

        /**
         * @param key 
         * @return Container 
         */
        Container find(K key)
        {
            auto* node = static_cast<TreeNode*>(BaseTree::find(this->m_root, key));
            if(!node)
                return nullptr;
            return node_to_value(*node);
        }

        /**
         * @param key 
         * @return Container 
         */
        Container find_largest_not_above(K key)
        {
            auto* node = static_cast<TreeNode*>(BaseTree::find_largest_not_above(this->m_root, key));
            if(!node)
                return nullptr;
            return node_to_value(*node);
        }

        /**
         * @param key 
         * @return Container 
         */
        Container find_smallest_not_below(K key)
        {
            auto* node = static_cast<TreeNode*>(BaseTree::find_smallest_not_below(this->m_root, key));
            if(!node)
                return nullptr;
            return node_to_value(*node);
        }

        /**
         * @param key 
         * @param value 
         */
        void insert(K key, V& value)
        {
            auto& node = value.*member;
            VERIFY(!node.m_in_tree);
            static_cast<typename BaseTree::Node&>(node).key = key;
            BaseTree::insert(&node);

            if constexpr(!TreeNode::IsRaw)
                node.m_self.reference = &value; 

            node.m_in_tree = true;
        }

        /**
         * @tparam ElementType 
         */
        template <typename ElementType>
        class BaseIterator
        {
        public:
            /**
             * @brief Construct a new Base Iterator object
             * 
             */
            BaseIterator() = default;

            /**
             * @param other 
             * @return true 
             * @return false 
             */
            bool operator!=(BaseIterator const& other) const
            {
                return m_node != other.m_node;
            }

            /**
             * @return BaseIterator& 
             */
            BaseIterator& operator++()
            {
                if(!m_node)
                    return *this;
                m_prev = m_node;

                m_node = static_cast<TreeNode*>(BaseTree::successor(m_node));
                return *this;
            }

            /**
             * @return BaseIterator& 
             */
            BaseIterator& operator--()
            {
                if(!m_prev)
                    return *this;
                m_node = m_prev;
                m_prev = static_cast<TreeNode*>(BaseTree::predecessor(m_prev));
                return *this;
            }

            /**
             * @return ElementType& 
             */
            ElementType& operator*()
            {
                VERIFY(m_node);
                return *node_to_value(*m_node);
            }

            /**
             * @return auto 
             */
            auto operator->()
            {
                VERIFY(m_node);
                return node_to_value(*m_node);
            }

            /**
             * @return true 
             * @return false 
             */
            [[nodiscard]] bool is_end() const
            {
                return !m_node;
            }

            [[nodiscard]] bool is_begin() const
            {
                return !m_prev;
            }

            [[nodiscard]] auto key() const
            {
                return m_node->key;
            }

        private:
            friend class IntrusiveRedBlackTree;

            /**
             * @brief Construct a new Base Iterator object
             * 
             * @param node 
             * @param prev 
             */
            explicit BaseIterator(TreeNode* node, TreeNode* prev = nullptr)
                : m_node(node), m_prev(prev)
            {
            }

            TreeNode* m_node{nullptr};
            TreeNode* m_prev{nullptr};
        }; // class BaseIterator

        using Iterator = BaseIterator<V>;

        Iterator begin()
        {
            return Iterator(static_cast<TreeNode*>(this->m_minimum));
        }

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
            return Iterator(static_cast<TreeNode*>(BaseTree::find(this->m_root, key)));
        }

        /**
         * @param value 
         * @return Iterator 
         */
        Iterator begin_from(V& value)
        {
            return Iterator(&(value.*member));
        }

        using ConstIterator = BaseIterator<const V>;

        ConstIterator begin() const
        {
            return ConstIterator(static_cast<TreeNode*>(this->m_minimum));
        }

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
            return ConstIterator(static_cast<TreeNode*>(BaseTree::find(this->m_rootF, key)));
        }

        /**
         * @param value 
         * @return ConstIterator 
         */
        ConstIterator begin_from(V const& value) const
        {
            return Iterator(&(value.*member));
        }

        /**
         * @param key 
         * @return true 
         * @return false 
         */
        bool remove(K key)
        {
            auto* node = static_cast<TreeNode*>(BaseTree::find(this->m_root, key));

            if(!node)
                return false;

            BaseTree::remove(node);

            node->right_child = nullptr;
            node->left_child = nullptr;
            node->m_in_tree = false;

            if constexpr(!TreeNode::IsRaw)
                node->m_self.reference = nullptr;

            return true;
        }

        void clear()
        {
            clear_nodes(static_cast<TreeNode*>(this->m_root));
            this->m_root = nullptr;
            this->m_minimum = nullptr;
            this->m_size = 0;
        }

    private:
        /**
         * @param node 
         */
        static void clear_nodes(TreeNode* node)
        {
            if(!node)
                return;
            clear_nodes(static_cast<TreeNode*>(node->right_child));
            node->right_child = nullptr;
            clear_nodes(static_cast<TreeNode*>(node->left_child));
            node->left_child = nullptr;
            node->m_in_tree = false;
            if constexpr(!TreeNode::IsRaw)
                node->m_self.reference = nullptr;
        }

        /**
         * @param node 
         * @return V* 
         */
        static V* node_to_value(TreeNode& node)
        {
            return bit_cast<V*>(bit_cast<u8*>(&node) - bit_cast<u8*>(member));
        }
    };

    /**
     * @tparam K 
     * @tparam V 
     * @tparam Container 
     */
    template <Integral K, typename V, typename Container>
    class IntrusiveRedBlackTreeNode : public BaseRedBlackTree<K>::Node
    {
    public:
        /**
         * @brief Destroy the Intrusive Red Black Tree Node object
         * 
         */
        ~IntrusiveRedBlackTreeNode()
        {
            VERIFY(!is_in_tree());
        }

        [[nodiscard]] bool is_in_tree() const
        {
            return m_in_tree;
        }

        [[nodiscard]] K key() const
        {
            return BaseRedBlackTree<K>::Node::key;
        }

        static constexpr bool IsRaw = IsPointer<Container>;

    #ifndef __clang__
    private:
        template <Integral TK, typename TV, typename TContainer, SubstitutedIntrusiveRedBlackTreeNode<TK, TV, TContainer> TV::* member>
        friend class ::Mods::Detail::IntrusiveRedBlackTree;
    #endif

        bool m_in_tree{false};
        [[no_unique_address]] SelfReferenceIfNeeded<Container, IsRaw> m_self;
    }; // class IntrusiveRedBlackTreeNode : public BaseRedBlackTree<K>::Node

    /**
     * @tparam K 
     * @tparam V 
     * @tparam member 
     */
    template <Integral K, typename V, SubstitutedIntrusiveRedBlackTreeNode<K, V, NonnullRefPtr<V>> V::* member>
    class IntrusiveRedBlackTree<K, V, NonnullRefPtr<V>, member> : public IntrusiveRedBlackTree<K, V, RefPtr<V>, member>
    {
    public:
        /**
         * @param key 
         * @return NonnullRefPtr<V> 
         */
        [[nodiscard]] NonnullRefPtr<V> find(K key) const
        {
            return IntrusiveRedBlackTree<K, V, RefPtr<V>, member>::find(key).release_nonnull();
        }

        /**
         * @param key 
         * @return NonnullRefPtr<V> 
         */
        [[nodiscard]] NonnullRefPtr<V> find_largest_not_above(K key) const
        {
            return IntrusiveRedBlackTree<K, V, RefPtr<V>, member>::find_largest_not_above(key).release_nonnull();
        }

        /**
         * @param key 
         * @return NonnullRefPtr<V> 
         */
        [[nodiscard]] NonnullRefPtr<V> find_smallest_not_below(K key) const
        {
            return IntrusiveRedBlackTree<K, V, RefPtr<V>, member>::find_smallest_not_below(key).release_nonnull();
        }
    }; // class IntrusiveRedBlackTree<K, V, NonnullRefPtr<V>, member> : public IntrusiveRedBlackTree<K, V, RefPtr<V>, member>
} // namespace Mods::Detail

namespace Mods
{
/**
 * @tparam K 
 * @tparam V 
 * @tparam Container 
 */
template <Integral K, typename V, typename Container = RawPtr<K>>
using IntrusiveRedBlackTreeNode = Detail::SubstitutedIntrusiveRedBlackTreeNode<K, V, Container>;

/**
 * @tparam member 
 */
template <auto member>
using IntrusiveRedBlackTree = Detail::IntrusiveRedBlackTree<
    decltype(Detail::ExtractIntrusiveRedBlackTreeTypes::key(member)),
    decltype(Detail::ExtractIntrusiveRedBlackTreeTypes::value(member)),
    decltype(Detail::ExtractIntrusiveRedBlackTreeTypes::container(member)),
    member>;

} // namespace Mods

using Mods::IntrusiveRedBlackTree;
using Mods::IntrusiveRedBlackTreeNode;
