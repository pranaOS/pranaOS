/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/FlyString.h>
#include <libweb/dom/CharacterData.h>

namespace Web::DOM {

class Comment final : public CharacterData {
public:
    using WrapperType = Bindings::CommentWrapper;

    explicit Comment(Document&, const String&);
    virtual ~Comment() override;

    virtual FlyString node_name() const override { return "#comment"; }
};

}
