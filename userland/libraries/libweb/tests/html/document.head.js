describe("head", () => {
    loadLocalPage("/res/html/misc/blank.html");

    afterInitialPageLoad(page => {
        test("Basic functionality", () => {
            expect(page.document.head).not.toBeNull();

            expect(page.document.head.nodeName).toBe("HEAD");
        });

        test.skip("Nullable", () => {
            page.document.documentElement.removeChild(page.document.head);
            expect(page.document.head).toBeNull();
        });
    });
    waitForPageToLoad();
});
