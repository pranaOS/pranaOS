/**
 * @file urlparser.h
 * @author Krisna Pranav
 * @brief url_parser
 * @version 6.0
 * @date 2023-06-29
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/optional.h>
#include <mods/stringview.h>
#include <mods/url.h>

namespace Mods
{

#define ENUMERATE_STATES                 \
    STATE(SchemeStart)                   \
    STATE(Scheme)                        \
    STATE(NoScheme)                      \
    STATE(SpecialRelativeOrAuthority)    \
    STATE(PathOrAuthority)               \
    STATE(Relative)                      \
    STATE(RelativeSlash)                 \
    STATE(SpecialAuthoritySlashes)       \
    STATE(SpecialAuthorityIgnoreSlashes) \
    STATE(Authority)                     \
    STATE(Host)                          \
    STATE(Hostname)                      \
    STATE(Port)                          \
    STATE(File)                          \
    STATE(FileSlash)                     \
    STATE(FileHost)                      \
    STATE(PathStart)                     \
    STATE(Path)                          \
    STATE(CannotBeABaseUrlPath)          \
    STATE(Query)                         \
    STATE(Fragment)

class URLParser
{
  public:
    enum class State
    {
#define STATE(state) state,
        ENUMERATE_STATES
#undef STATE
    }; // enum class State

    /**
     * @param state 
     * @return char const* 
     */
    static char const* state_name(State const& state)
    {
        switch(state)
        {
#define STATE(state)   \
    case State::state: \
        return #state;
            ENUMERATE_STATES
#undef STATE
        }
        VERIFY_NOT_REACHED();
    }

    /**
     * @param input 
     * @param base_url 
     * @param url 
     * @param state_override 
     * @return URL 
     */
    static URL parse(StringView input, URL const* base_url = nullptr, Optional<URL> url = {}, Optional<State> state_override = {});

  private:
    /**
     * @param raw_input 
     * @return Optional<URL> 
     */
    static Optional<URL> parse_data_url(StringView raw_input);
}; // class URLParser

#undef ENUMERATE_STATES

} // namespace Mods

using Mods::URLParser;