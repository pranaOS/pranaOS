import Logging

final class HDA: PCIModule {
    var Name: String = "HDA"
    var description: String {
        return "Intel high definition audio device drivers"
    }
    static var CompatibleDevices: [PCIDevice] = [(0x8086, 0x2668), (0x8086, 0x27D8), (0x1002, 0x4383), (0x15AD, 0x1977)]
    static var ComptabileClasses: [PCIClass]? = nil
    init?(pci: PCI) {
        Log("Found Intel HDA: ", level: .Verbose)
    }
}
