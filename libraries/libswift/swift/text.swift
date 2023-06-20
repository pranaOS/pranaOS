import Loggable

class TextFile: File, Loggable {
    let name: String = "Text File"
    var text: String {
        get {
            return String(utf8Characters: file.Data) ?? ""
        }
        set {
            var buf = ContiguousArray<UInt8>(newValue.utf8)
            file.Info.Size = newValue.utf8.count
            file.Data.deallocate()
            file.Data = MutableData(start: &buf[0], count: buf.count)
            writeToDisk()
        }
    }

    required init?(partition: Partition, path: String) {
        super.init(partition: partition, path: path)
        guard file.Info.Extension == "TXT" || file.Info.Extension == "PLI" || file.Info.Extension == "XML"  else {
            Log("File didn't pass extension checks for plain text", level: .Error)
            file.Data.deallocate()
            return nil
        }
    }
}