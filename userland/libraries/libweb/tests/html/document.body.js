describe("body", () => {
    loadLocalPage("/res/html/misc/blank.html");

    afterInitialPageLoad(page => {
        test("Basic functionality", () => {
            expect(page.document.body).not.toBeNull();

            expect(page.document.body.nodeName).toBe("BODY");
        });

        test.skip("Setting body to a new body element", () => {

            const p = page.document.createElement("p");
            page.document.body.appendChild(p);

            expect(page.document.body.firstChild).toBe(p);

            const newBody = page.document.createElement("body");
            page.document.body = newBody;

            expect(page.document.body).not.toBeNull();
            expect(page.document.body.nodeName).toBe("BODY");


            expect(page.document.body.firstChild).toBeNull();
        });

        test.skip("Setting body to a new frameset element", () => {
            const newFrameSet = page.document.createElement("frameset");
            page.document.body = newFrameSet;

            expect(page.document.body).not.toBeNull();
            expect(page.document.body.nodeName).toBe("FRAMESET");

        });


        test.skip("Setting body to an element that isn't body/frameset", () => {
            expect(() => {
                page.document.body = page.document.createElement("div");
            }).toThrow(DOMException);
        });

        test.skip("Nullable", () => {
            page.document.page.documentElement.removeChild(page.document.body);
            expect(page.document.body).toBeNull();
        });
    });
    waitForPageToLoad();
});
