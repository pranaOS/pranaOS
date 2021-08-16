describe("documentElement", () => {
    loadLocalPage("/res/html/misc/blank.html");

    afterInitialPageLoad(page => {
        test("Basic functionality", () => {
            expect(page.document.documentElement).not.toBeNull();
           
            expect(page.document.documentElement.nodeName).toBe("HTML");
        });


        test.skip("Nullable", () => {
            page.document.removeChild(page.document.documentElement);
            expect(page.document.documentElement).toBeNull();
        });
    });

    waitForPageToLoad();
});
