/**
 * @file address_space.h
 * @author Krisna Pranav
 * @brief Address Space
 * @version 6.0
 * @date 2024-10-20
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/rbtree.h>
#include <mods/vector.h>
#include <mods/weakptr.h>
#include <kernel/memory/allocationstrategy.h>
#include <kernel/memory/pagedirectory.h>
#include <kernel/unixtypes.h>

namespace Kernel::Memory 
{
    class AddressSpace 
    {
    public:
        /**
         * @param parent 
         * @return ErrorOr<NonnullOwnPtr<AddressSpace>> 
         */
        static ErrorOr<NonnullOwnPtr<AddressSpace>> try_create(AddressSpace const* parent);

        /**
         * @brief Destroy the Address Space object
         * 
         */
        ~AddressSpace();

        /**
         * @return PageDirectory& 
         */
        PageDirectory& page_directory() 
        { 
            return *m_page_directory; 
        }

        /**
         * @return const PageDirectory& 
         */
        const PageDirectory& page_directory() const 
        { 
            return *m_page_directory; 
        }

        /**
         * @return ErrorOr<Region*> 
         */
        ErrorOr<Region*> add_region(NonnullOwnPtr<Region>);

        /**
         * @return size_t 
         */
        size_t region_count() const 
        { 
            return m_regions.size(); 
        }

        /**
         * @return RedBlackTree<FlatPtr, NonnullOwnPtr<Region>>& 
         */
        RedBlackTree<FlatPtr, NonnullOwnPtr<Region>>& regions() 
        { 
            return m_regions; 
        }

        /**
         * @return const RedBlackTree<FlatPtr, NonnullOwnPtr<Region>>& 
         */
        const RedBlackTree<FlatPtr, NonnullOwnPtr<Region>>& regions() const 
        { 
            return m_regions; 
        }

        void dump_regions();

        /**
         * @return ErrorOr<void> 
         */
        ErrorOr<void> unmap_mmap_range(VirtualAddress, size_t);

        /**
         * @param alignment 
         * @return ErrorOr<VirtualRange> 
         */
        ErrorOr<VirtualRange> try_allocate_range(VirtualAddress, size_t, size_t alignment = PAGE_SIZE);

        /**
         * @param offset_in_vmobject 
         * @param name 
         * @param prot 
         * @param shared 
         * @return ErrorOr<Region*> 
         */
        ErrorOr<Region*> allocate_region_with_vmobject(VirtualRange const&, NonnullRefPtr<VMObject>, size_t offset_in_vmobject, StringView name, int prot, bool shared);

        /**
         * @param name 
         * @param prot 
         * @param strategy 
         * @return ErrorOr<Region*> 
         */
        ErrorOr<Region*> allocate_region(VirtualRange const&, StringView name, int prot = PROT_READ | PROT_WRITE, AllocationStrategy strategy = AllocationStrategy::Reserve);

        /**
         * @param region 
         */
        void deallocate_region(Region& region);

        /**
         * @param region 
         * @return NonnullOwnPtr<Region> 
         */
        NonnullOwnPtr<Region> take_region(Region& region);

        /**
         * @param source_region 
         * @param offset_in_vmobject 
         * @return ErrorOr<Region*> 
         */
        ErrorOr<Region*> try_allocate_split_region(Region const& source_region, VirtualRange const&, size_t offset_in_vmobject);

        /**
         * @param source_region 
         * @return ErrorOr<Vector<Region*, 2>> 
         */
        ErrorOr<Vector<Region*, 2>> try_split_region_around_range(Region const& source_region, VirtualRange const&);

        /**
         * @return Region* 
         */
        Region* find_region_from_range(VirtualRange const&);
        Region* find_region_containing(VirtualRange const&);

        /**
         * @return ErrorOr<Vector<Region*>> 
         */
        ErrorOr<Vector<Region*>> find_regions_intersecting(VirtualRange const&);

        bool enforces_syscall_regions() const 
        { 
            return m_enforces_syscall_regions; 
        }

        /**
         * @brief Set the enforces syscall regions object
         * 
         * @param b 
         */
        void set_enforces_syscall_regions(bool b) 
        { 
            m_enforces_syscall_regions = b; 
        }

        void remove_all_regions(Badge<Process>);

        /**
         * @brief Get the lock object
         * 
         * @return RecursiveSpinlock& 
         */
        RecursiveSpinlock& get_lock() const 
        { 
            return m_lock; 
        }

        /**
         * @return ErrorOr<size_t> 
         */
        ErrorOr<size_t> amount_clean_inode() const;

        size_t amount_dirty_private() const;
        size_t amount_virtual() const;
        size_t amount_resident() const;
        size_t amount_shared() const;
        size_t amount_purgeable_volatile() const;
        size_t amount_purgeable_nonvolatile() const;

    private:
        /**
         * @brief Construct a new Address Space object
         * 
         */
        explicit AddressSpace(NonnullRefPtr<PageDirectory>);

        mutable RecursiveSpinlock m_lock;

        RefPtr<PageDirectory> m_page_directory;

        RedBlackTree<FlatPtr, NonnullOwnPtr<Region>> m_regions;

        bool m_enforces_syscall_regions { false };
    }; // class AddressSpace
} // namespace Kernel::Memory