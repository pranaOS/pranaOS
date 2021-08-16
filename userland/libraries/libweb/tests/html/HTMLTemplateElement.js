describe("HTMLTemplateElement", () => {
    loadLocalPage("Template.html");

    afterInitialPageLoad(page => {
        test("Basic functionality", () => {
            const template = page.document.getElementById("template");
            expect(template).not.toBeNull();

            expect(template.firstChild).toBeNull();

            expect(template.content.nodeName).toBe("#document-fragment");

            const templateDiv = template.content.getElementById("templatediv");
            expect(templateDiv.nodeName).toBe("DIV");
            expect(templateDiv.textContent).toBe("Hello template!");
        });

        test("Templates are inert (e.g. scripts won't run)", () => {

            expect(page.window.templateScriptRan).toBeUndefined();
        });
    });
    waitForPageToLoad();
});
