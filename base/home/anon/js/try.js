try {
    console.log("you see me??");
    foo();
    console.log("not me");
} catch (e) {
    console.log("catch");
    console.log(e.name);
} finally {
    console.log("finaly");
}