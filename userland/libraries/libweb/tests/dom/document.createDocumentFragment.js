describe("createDocumentFragment", () => {
    loadLocalPage("/res/html/misc/blank.html");

    afterInitialPageLoad(page => {
        test("Basic functionality", () => {
            const fragment = page.document.createDocumentFragment();

            expect(fragment.nodeName).toBe("#document-fragment");
        });
    });

    waitForPageToLoad();
});
