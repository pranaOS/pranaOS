#pragma once

// includes
#include "Type.h"

namespace Spreadsheet {

class IdentityCell : public CellType {

public:
    IdentityCell();
    virtual ~IdentityCell() override;
    virtual String display(Cell&, const CellTypeMetadata&) const override;
    virtual JS::Value js_value(Cell&, const CellTypeMetadata&) const override;
};

}
