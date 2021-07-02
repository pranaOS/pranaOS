#pragma once

// includes
#include "Type.h"

namespace Spreadsheet {

class NumericCell : public CellType {

public:
    NumericCell();
    virtual ~NumericCell() override;
    virtual String display(Cell&, const CellTypeMetadata&) const override;
    virtual JS::Value js_value(Cell&, const CellTypeMetadata&) const override;
};

}
