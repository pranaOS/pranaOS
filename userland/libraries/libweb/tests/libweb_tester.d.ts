interface LibwebTester {
    changePage(url: string): void;
}

interface Window {
    readonly libweb_tester: LibwebTester;
}
