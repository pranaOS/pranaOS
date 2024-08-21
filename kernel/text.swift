/**
 * @file text.swift
 * @author Krisna Pranav
 * @brief Memory
 * @version 6.0
 * @date 2023-07-30
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

import Log

class Text: File, Loggable {
    let Name: String = "Text"
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
        guard file.info.Extension == "TXT" || file.Info.Extension == "PLI" || file.Info.Extension == "XML" else {
            Log("Not as expected file extension", level: .Error)
            file.Data.deallocate()
            return nil
        }        
    }
}