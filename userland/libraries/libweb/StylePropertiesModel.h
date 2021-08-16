/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/NonnullRefPtrVector.h>
#include <libgui/Model.h>
#include <libweb/css/StyleProperties.h>

namespace Web {

class StyleProperties;

class StylePropertiesModel final : public GUI::Model {
public:
    enum Column {
        PropertyName,
        PropertyValue,
        __Count
    };

    static NonnullRefPtr<StylePropertiesModel> create(const CSS::StyleProperties& properties) { return adopt_ref(*new StylePropertiesModel(properties)); }

    virtual int row_count(const GUI::ModelIndex& = GUI::ModelIndex()) const override;
    virtual int column_count(const GUI::ModelIndex& = GUI::ModelIndex()) const override { return Column::__Count; }
    virtual String column_name(int) const override;
    virtual GUI::Variant data(const GUI::ModelIndex&, GUI::ModelRole) const override;

private:
    explicit StylePropertiesModel(const CSS::StyleProperties& properties);
    const CSS::StyleProperties& properties() const { return *m_properties; }

    NonnullRefPtr<CSS::StyleProperties> m_properties;

    struct Value {
        String name;
        String value;
    };
    Vector<Value> m_values;
};

}
