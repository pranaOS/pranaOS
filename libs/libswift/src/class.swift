class OperatingSystem {
    let name: String

    init(name: String) {
        self.name = name
    }

    func os() {}
}

class pranaOS: os {
    override func os() {
        stars()
    }

    private func stars() {
        print("\(name) pranaOS has reached 58 stars!!")
    }
}