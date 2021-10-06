import Swift

func myfunc() {
    print("hello world");
}

func letprint() {
    let os = "pranaOS"
    print("Hello From \(os)")
}

func main() {
    var number = 0

    if (number > 0) {
        myfunc()
    } else {
        letprint()
    }
}