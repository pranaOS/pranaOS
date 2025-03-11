class File {
    internal var file: (Info: DirectoryEntry, Data: MutableData)
    fileprivate let partition: Partition
    fileprivate let path: String

    required init?(partition: Partition, path: String) {
        self.partition = partition
        self.path = path
        guard let f = partition.ReadFile(fromPath: path) else {
            return nil
        }

        file = f 
    }

    func writeToDisk() {
        partition.WriteFile(fromPath: path, file: file)
    }

    deinit {
        file.Data.deallocate()
    }
}