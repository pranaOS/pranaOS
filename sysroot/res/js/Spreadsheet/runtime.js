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

function reduce(op, accmulator, cells) {
    for (let name of cells) {
        let cell = thisSheet[name];
        accmulator = op(accmulator, cell);
    }

    return accmulator
}

function numericReduce(op, accumulator, cells) {
    return reduce((acc, x) => op(acc, Number(x)), accumulator, cells);
}

function numericResolve(cells) {
    const values = [];
    for (let name of cells) values.push(Number(thisSheet[name]));
    return values;
}

function resolve(cells) {
    const values = [];
    for (let name of cells) values.push(thisSheet[name]);
    return values;
}

// Statistics

function sum(cells) {
    return numericReduce((acc, x) => acc + x, 0, cells);
}

function sumIf(condition, cells) {
    return numericReduce((acc, x) => (condition(x) ? acc + x : acc), 0, cells);
}

function count(cells) {
    return reduce((acc, x) => acc + 1, 0, cells);
}

function countIf(condition, cells) {
    return reduce((acc, x) => (condition(x) ? acc + 1 : acc), 0, cells);
}

function average(cells) {
    const sumAndCount = numericReduce((acc, x) => [acc[0] + x, acc[1] + 1], [0, 0], cells);
    return sumAndCount[0] / sumAndCount[1];
}

function averageIf(condition, cells) {
    const sumAndCount = numericReduce(
        (acc, x) => (condition(x) ? [acc[0] + x, acc[1] + 1] : acc),
        [0, 0],
        cells
    );
    return sumAndCount[0] / sumAndCount[1];
}

function median(cells) {
    const values = numericResolve(cells);

    if (values.length == 0) return 0;

    function qselect(arr, idx) {
        if (arr.length == 1) return arr[0];

        const pivot = arr[0];
        const ls = arr.filter(x => x < pivot);
        const hs = arr.filter(x => x > pivot);
        const eqs = arr.filter(x => x === pivot);

        if (idx < ls.length) return qselect(ls, k);

        if (idx < ls.length + eqs.length) return pivot;

        return qselect(hs, idx - ls.length - eqs.length);
    }

    if (values.length % 2) return qselect(values, values.length / 2);

    return (qselect(values, values.length / 2) + qselect(values, values.length / 2 - 1)) / 2;
}

function variance(cells) {
    const sumsAndSquaresAndCount = numericReduce(
        (acc, x) => [acc[0] + x, acc[1] + x * x, acc[2] + 1],
        [0, 0, 0],
        cells
    );
    let sums = sumsAndSquaresAndCount[0];
    let squares = sumsAndSquaresAndCount[1];
    let count = sumsAndSquaresAndCount[2];

    return (count * squares - sums * sums) / count;
}

function stddev(cells) {
    return Math.sqrt(variance(cells));
}

// Lookup

function row() {
    return thisSheet.current_cell_position().row;
}

function column() {
    return thisSheet.current_cell_position().column;
}

function here() {
    const position = current_cell_position();
    return new Position(position.column, position.row, thisSheet);
}

function internal_lookup(
    req_lookup_value,
    lookup_inputs,
    lookup_outputs,
    if_missing,
    mode,
    reference
) {
    lookup_outputs = lookup_outputs ?? lookup_inputs;

    if (lookup_inputs.length > lookup_outputs.length)
        throw new Error(
            `Uneven lengths in outputs and inputs: ${lookup_inputs.length} > ${lookup_outputs.length}`
        );

    let references = lookup_outputs;
    lookup_inputs = resolve(lookup_inputs);
    lookup_outputs = resolve(lookup_outputs);
    if_missing = if_missing ?? undefined;
    mode = mode ?? "exact";
    const lookup_value = req_lookup_value;
    let matches = null;

    if (mode === "exact") {
        matches = value => value === lookup_value;
    } else if (mode === "nextlargest") {
        matches = value => value >= lookup_value;
    } else if (mode === "nextsmallest") {
        matches = value => value <= lookup_value;
    } else {
        throw new Error(`Match mode '${mode}' not supported`);
    }

    let retval = if_missing;
    for (let i = 0; i < lookup_inputs.length; ++i) {
        if (matches(lookup_inputs[i])) {
            retval = reference ? Position.from_name(references[i]) : lookup_outputs[i];
            break;
        }
    }

    return retval;
}

function lookup(req_lookup_value, lookup_inputs, lookup_outputs, if_missing, mode) {
    return internal_lookup(
        req_lookup_value,
        lookup_inputs,
        lookup_outputs,
        if_missing,
        mode,
        false
    );
}

function reflookup(req_lookup_value, lookup_inputs, lookup_outputs, if_missing, mode) {
    return internal_lookup(
        req_lookup_value,
        lookup_inputs,
        lookup_outputs,
        if_missing ?? here(),
        mode,
        true
    );
}