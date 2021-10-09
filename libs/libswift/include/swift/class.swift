class OperatingSystem {
    let name: String

    init(name: String) {
        self.name = name
    }

    func os() {}
}

class pranaOS: OperatingSystem {
    override func os() {
        stars()
    }

    private func stars() {
        print("\(name) has reached 58 stars!!")
    }
}

class pranaOSfork: OperatingSystem {
    override func os() {
        forks()
    }

    private func forks() {
        print("\(name) has reached 25 forks")
    }
}

class pranaOS {
    let name: String

    init(name: String) {
        self.name = name
    }

    func os() {
        
    }
}