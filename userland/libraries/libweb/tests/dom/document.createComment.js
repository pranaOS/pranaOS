describe("createComment", () => {
    loadLocalPage("/res/html/misc/blank.html");

    afterInitialPageLoad(page => {
        test("Basic functionality", () => {
            const comment = page.document.createComment("Create Comment Test");

            expect(comment.nodeName).toBe("#comment");
            expect(comment.data).toBe("Create Comment Test");
            expect(comment.length).toBe(19);
        });
    });
    waitForPageToLoad();
});
