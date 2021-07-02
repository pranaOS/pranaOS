#pragma once

// include
#include "Type.h"

namespace Spreadsheet {

class DateCell : public CellType {

public:
    DateCell();
    virtual ~DateCell() override;
    virtual String display(Cell&, const CellTypeMetadata&) const override;
    virtual JS::Value js_value(Cell&, const CellTypeMetadata&) const override;
};

}
