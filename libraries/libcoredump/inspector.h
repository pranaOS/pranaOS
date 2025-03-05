/**
 * @file inspector.h
 * @author Krisna Pranav
 * @brief inspector
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "reader.h"
#include <mods/noncopyable.h>
#include <libdebug/processinspector.h>

namespace Coredump 
{

    class Inspector : public Debug::ProcessInspector 
    {
        MOD_MAKE_NONCOPYABLE(Inspector);
        MOD_MAKE_NONMOVABLE(Inspector);

    public:
        /**
         * @param coredump_path 
         * @param on_progress 
         * @return OwnPtr<Inspector> 
         */
        static OwnPtr<Inspector> create(String const& coredump_path, Function<void(float)> on_progress = {});

        /**
         * @brief Destroy the Inspector object
         * 
         */
        virtual ~Inspector() override = default;

        /**
         * @param address 
         * @param data 
         * @return true 
         * @return false 
         */
        virtual bool poke(FlatPtr address, FlatPtr data) override;

        /**
         * @param address 
         * @return Optional<FlatPtr> 
         */
        virtual Optional<FlatPtr> peek(FlatPtr address) const override;

        /**
         * @brief Get the registers object
         * 
         * @return PtraceRegisters 
         */
        virtual PtraceRegisters get_registers() const override;

        /**
         * @brief Set the registers object
         * 
         */
        virtual void set_registers(PtraceRegisters const&) override;

        virtual void for_each_loaded_library(Function<IterationDecision(Debug::LoadedLibrary const&)>) const override;

    private:
        /**
         * @brief Construct a new Inspector object
         * 
         * @param on_progress 
         */
        Inspector(NonnullOwnPtr<Reader>&&, Function<void(float)> on_progress);

        /**
         * @param on_progress 
         */
        void parse_loaded_libraries(Function<void(float)> on_progress);

        /**
         * @return size_t 
         */
        size_t number_of_libraries_in_coredump() const;

        NonnullOwnPtr<Reader> m_reader;

        NonnullOwnPtrVector<Debug::LoadedLibrary> m_loaded_libraries;
    }; // class Inspector : public Debug::ProcessInspector

} // namespace Coredump
