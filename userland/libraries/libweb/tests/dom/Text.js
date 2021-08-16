describe("Text", () => {
    loadLocalPage("/res/html/misc/blank.html");

    afterInitialPageLoad(page => {
        test("Basic functionality", () => {
            const title = page.document.getElementsByTagName("title")[0];
            expect(title).toBeDefined();


            expect(title.firstChild.nodeName).toBe("#text");
            expect(title.firstChild.data).toBe("Blank");
            expect(title.firstChild.length).toBe(5);
        });
    });
    waitForPageToLoad();
});
