
#pragma once

// includes
#include <AK/NonnullOwnPtrVector.h>
#include <AK/String.h>

class ManualNode {
public:
    virtual ~ManualNode() { }

    virtual NonnullOwnPtrVector<ManualNode>& children() const = 0;
    virtual const ManualNode* parent() const = 0;
    virtual String name() const = 0;
    virtual bool is_page() const { return false; }
    virtual bool is_open() const { return false; }
};