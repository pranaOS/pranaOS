class Position {
    constructor(column, row, sheet) {
        this.column = column;
        this.row = row;
        this.sheet = sheet ?? thisSheet;
        this.name = `${column}${row}`;
    }

    get contents() {
        return this.sheet.get_real_cell_contents(this.name);
    }

    set contents(value) {
        value = `${value}`;
        this.sheet.set_real_cell_contents(this.name, value);
        return value;
    }

    static from_name(name) {
        let sheet = thisSheet;
        let obj = sheet.parse_cell_name(name);
        return new Position(obj.column, obj.row, sheet);
    }

    up(how_many) {
        how_many = how_many ?? 1;
        const row = Math.max(0, this.row - how_many);
        return new Position(this.column, row, this.sheet);
    }

    down(how_many) {
        how_many = how_many ?? 1;
        const row = Math.max(0, this.row + how_many);
        return new Position(this.column, row, this.sheet);
    }

    left(how_many) {
        how_many = how_many ?? 1;
        return new Position(
            this.sheet.column_arithmetic(this.column, -how_many),
            this.row,
            this.sheet
        );
    }

    right(how_many) {
        how_many = how_many ?? 1;
        return new Position(
            this.sheet.column_arithmetic(this.column, how_many),
            this.row,
            this.sheet
        );
    }

    with_column(value) {
        return new Position(value, this.row, this.sheet);
    }

    with_row(value) {
        return new Position(this.column, value, this.sheet);
    }

    in_sheet(the_sheet) {
        return new Position(this.column, this.row, sheet(the_sheet));
    }

    value() {
        return this.sheet[this.name];
    }

    valueOf() {
        return value();
    }

    toString() {
        return `<Cell at ${this.name}>`;
    }
}

function range(start, end, columnStep, rowStep) {
    columnStep = integer(columnStep ?? 1);
    rowStep = integer(rowStep ?? 1);
    if (!(start instanceof Position)) {
        start = parse_cell_name(start) ?? { column: "A", row: 0 };
    }
    if (!(end instanceof Position)) {
        end = parse_cell_name(end) ?? start;
    }

    const cells = [];

    const start_column_index = column_index(start.column);
    const end_column_index = column_index(end.column);
    const start_column = start_column_index > end_column_index ? end.column : start.column;
    const distance = Math.abs(start_column_index - end_column_index);

    for (let col = 0; col <= distance; col += columnStep) {
        const column = column_arithmetic(start_column, col);
        for (
            let row = Math.min(start.row, end.row);
            row <= Math.max(start.row, end.row);
            row += rowStep
        ) {
            cells.push(column + row);
        }
    }
    return cells;

}

function split(str, sep) {
    const parts = [];
    let splitIndex = -1;
    for (;;) {
        splitIndex = str.indexOf(sep);
        if (splitIndex == -1) {
            if (str.length) parts.push(str);
            break;
        }

        parts.push(str.substring(0, splitIndex));
        str = str.slice(splitIndex + sep.length);
    }
    return parts;
}

function R(fmt, ...args) {
    if (args.length !== 0) throw new TypeError("R`` format must literal");

    fmt = fmt[0];
    return range(...split(fmt, ":"));
}

function randRange(min, max) {
    return Math.random() * (max - min) + min;
}