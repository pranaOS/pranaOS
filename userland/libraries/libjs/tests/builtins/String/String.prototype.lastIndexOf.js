test("basic functionality", () => {
    expect(String.prototype.lastIndexOf).toHaveLength(1);

    expect("hello friends".lastIndexOf()).toBe(-1);
    expect("hello friends".lastIndexOf("e")).toBe(9);
    expect("hello friends".lastIndexOf("e", -7)).toBe(-1);
    expect("hello friends".lastIndexOf("e", 100)).toBe(9);
    expect("hello friends".lastIndexOf("")).toBe(13);
    expect("hello friends".lastIndexOf("Z")).toBe(-1);
    expect("hello friends".lastIndexOf("pranaOS")).toBe(-1);
    expect("hello friends".lastIndexOf("", 4)).toBe(4);
    expect("hello pranaOS friends".lastIndexOf("pranaOS")).toBe(6);
    expect("hello pranaOS friends pranaOS".lastIndexOf("pranaOS")).toBe(23);
    expect("hello pranaOS friends pranaOS".lastIndexOf("pranaOS", 14)).toBe(6);
    expect("".lastIndexOf("")).toBe(0);
    expect("".lastIndexOf("", 1)).toBe(0);
    expect("".lastIndexOf("", -1)).toBe(0);
    expect("hello friends pranaOS".lastIndexOf("h", 10)).toBe(0);
    expect("hello friends pranaOS".lastIndexOf("l", 4)).toBe(3);
    expect("hello friends pranaOS".lastIndexOf("s", 13)).toBe(12);
    expect("hello".lastIndexOf("pranaOS")).toBe(-1);
});

test("UTF-16", () => {
    var s = "😀";
    expect(s.lastIndexOf("😀")).toBe(0);
    expect(s.lastIndexOf("\ud83d")).toBe(0);
    expect(s.lastIndexOf("\ude00")).toBe(1);
    expect(s.lastIndexOf("a")).toBe(-1);
});
