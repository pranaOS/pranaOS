/**
 * @file dwarftypes.h
 * @author Krisna Pranav
 * @brief dwarf types
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/stream.h>
#include <mods/types.h>

namespace Debug::Dwarf 
{

    enum class CompilationUnitType 
    {
        Full = 1,
        Partial = 3
    }; // enum class CompilationUnitType

    struct [[gnu::packed]] CompilationUnitHeaderCommon {
        u32 length;
        u16 version;
    }; // struct [[gnu::packed]] CompilationUnitHeaderCommon

    struct [[gnu::packed]] CompilationUnitHeaderV4Ext {
        u32 abbrev_offset;
        u8 address_size;
    }; // struct [[gnu::packed]] CompilationUnitHeaderV4Ext

    struct [[gnu::packed]] CompilationUnitHeaderV5Ext {
        u8 unit_type;
        u8 address_size;
        u32 abbrev_offset;
    }; // struct [[gnu::packed]] CompilationUnitHeaderV5Ext

    struct [[gnu::packed]] CompilationUnitHeader {
        CompilationUnitHeaderCommon common;

        union {
            CompilationUnitHeaderV4Ext v4;
            CompilationUnitHeaderV5Ext v5;
        };

        size_t header_size() const
        {
            return sizeof(common) + ((common.version <= 4) ? sizeof(v4) : sizeof(v5));
        }

        /**
         * @return u32 
         */
        u32 length() const 
        { 
            return common.length; 
        }

        /**
         * @return u16 
         */
        u16 version() const 
        { 
            return common.version; 
        }

        /**
         * @return CompilationUnitType 
         */
        CompilationUnitType unit_type() const
        {
            return (common.version <= 4) ? CompilationUnitType::Full : static_cast<CompilationUnitType>(v5.unit_type);
        }

        /**
         * @return u32 
         */
        u32 abbrev_offset() const 
        { 
            return (common.version <= 4) ? v4.abbrev_offset : v5.abbrev_offset; 
        }

        /**
         * @return u8 
         */
        u8 address_size() const 
        { 
            return (common.version <= 4) ? v4.address_size : v5.address_size; 
        }
    }; // struct [[gnu::packed]] CompilationUnitHeader

    /**
     * @param stream 
     * @param header 
     * @return InputStream& 
     */
    inline InputStream& operator>>(InputStream& stream, CompilationUnitHeader& header)
    {
        stream.read_or_error(Bytes { &header.common, sizeof(header.common) });
        if (header.common.version <= 4)
            stream.read_or_error(Bytes { &header.v4, sizeof(header.v4) });
        else
            stream.read_or_error(Bytes { &header.v5, sizeof(header.v5) });
        return stream;
    }

    enum class EntryTag : u16 {
        None = 0,
        ArrayType = 0x1,
        ClassType = 0x2,
        EntryPoint = 0x3,
        EnumerationType = 0x4,
        FormalParameter = 0x5,
        ImportedDeclaration = 0x8,
        Label = 0xa,
        LexicalBlock = 0xb,
        Member = 0xd,
        PointerType = 0xf,
        ReferenceType = 0x10,
        CompileUnit = 0x11,
        StringType = 0x12,
        StructureType = 0x13,
        SubroutineType = 0x15,
        TypeDef = 0x16,
        UnionType = 0x17,
        UnspecifiedParameters = 0x18,
        Variant = 0x19,
        CommonBlock = 0x1a,
        CommonInclusion = 0x1b,
        Inheritance = 0x1c,
        InlinedSubroutine = 0x1d,
        Module = 0x1e,
        PtrToMemberType = 0x1f,
        SetType = 0x20,
        SubRangeType = 0x21,
        WithStmt = 0x22,
        AccessDeclaration = 0x23,
        BaseType = 0x24,
        CatchBlock = 0x25,
        ConstType = 0x26,
        Constant = 0x27,
        Enumerator = 0x28,
        FileType = 0x29,
        Friend = 0x2a,
        NameList = 0x2b,
        NameListItem = 0x2c,
        PackedType = 0x2d,
        SubProgram = 0x2e,
        TemplateTypeParam = 0x2f,
        TemplateValueParam = 0x30,
        ThrownType = 0x31,
        TryBlock = 0x32,
        VariantPart = 0x33,
        Variable = 0x34,
        VolatileType = 0x35,
        DwarfProcedure = 0x36,
        RestrictType = 0x37,
        InterfaceType = 0x38,
        Namespace = 0x39,
        ImportedModule = 0x3a,
        UnspecifiedType = 0x3b,
        PartialUnit = 0x3c,
        ImportedUnit = 0x3d,
        MutableType = 0x3e,
        Condition = 0x3f,
        SharedTyped = 0x40,
        TypeUnit = 0x41,
        RValueReferenceType = 0x42,
        TemplateAlias = 0x43,
        CoArrayType = 0x44,
        GenericSubRange = 0x45,
        DynamicType = 0x46,
        AtomicType = 0x47,
        CallSite = 0x48,
        CallSiteParameter = 0x49,
        SkeletonUnit = 0x4a,
        ImmutableType = 0x4b,
        LoUser = 0x4080,
        HiUser = 0xffff,
    }; // enum class EntryTag : u16

    enum class Attribute : u16 {
        None = 0,
        Sibling = 0x1,
        Location = 0x2,
        Name = 0x3,
        Ordering = 0x9,
        ByteSize = 0xb,
        BitOffset = 0xc,
        BitSize = 0xd,
        StmtList = 0x10,
        LowPc = 0x11,
        HighPc = 0x12,
        Language = 0x13,
        Discr = 0x15,
        DiscrValue = 0x16,
        Visibility = 0x17,
        Import = 0x18,
        StringLength = 0x19,
        CommonReference = 0x1a,
        CompDir = 0x1b,
        ConstValue = 0x1c,
        ContainingType = 0x1d,
        DefaultValue = 0x1e,
        Inline = 0x20,
        IsOptional = 0x21,
        LowerBound = 0x22,
        Producer = 0x25,
        Prototyped = 0x27,
        ReturnAddr = 0x2a,
        StartScope = 0x2c,
        BitStride = 0x2e,
        UpperBound = 0x2f,
        AbstractOrigin = 0x31,
        Accessibility = 0x32,
        AddressClass = 0x33,
        Artificial = 0x34,
        BaseTypes = 0x35,
        CallingConvention = 0x36,
        Count = 0x37,
        MemberLocation = 0x38,
        DeclColumn = 0x39,
        DeclFile = 0x3a,
        DeclLine = 0x3b,
        Declaration = 0x3c,
        DiscrList = 0x3d,
        Encoding = 0x3e,
        External = 0x3f,
        FrameBase = 0x40,
        Friend = 0x41,
        IdentifierCase = 0x43,
        MacroInfo = 0x43,
        NameListItem = 0x44,
        Priority = 0x45,
        Segment = 0x46,
        Specification = 0x47,
        StaticLink = 0x48,
        Type = 0x49,
        UseLocation = 0x4a,
        VariableParameter = 0x4b,
        Virtuality = 0x4c,
        VtableElemLocation = 0x4d,
        Allocated = 0x4e,
        Associated = 0x4f,
        DataLocation = 0x50,
        ByteStride = 0x51,
        EntryPC = 0x52,
        UseUTF8 = 0x53,
        Extension = 0x54,
        Ranges = 0x55,
        Trampoline = 0x56,
        CallColumn = 0x57,
        CallFile = 0x58,
        CallLine = 0x59,
        Description = 0x5a,
        BinaryScale = 0x5b,
        DecimalScale = 0x5c,
        Small = 0x5d,
        DecimalSign = 0x5e,
        DigitCount = 0x5f,
        PictureString = 0x60,
        Mutable = 0x61,
        ThreadsScaled = 0x62,
        Explicit = 0x63,
        ObjectPointer = 0x64,
        Endianity = 0x65,
        Elemental = 0x66,
        Pure = 0x67,
        Recursive = 0x68,
        Signature = 0x69,
        MainSubprogram = 0x6a,
        DataBitOffset = 0x6b,
        ConstExpr = 0x6c,
        EnumClass = 0x6d,
        LinkageName = 0x6e,
        StringLengthBitSize = 0x6f,
        StringLengthByteSize = 0x70,
        Rank = 0x71,
        StrOffsetsBase = 0x72,
        AddrBase = 0x73,
        RngListsBase = 0x74,
        DWOName = 0x76,
        Reference = 0x77,
        RValueReference = 0x78,
        Macros = 0x79,
        CallAllCalls = 0x7a,
        CallAllSourceCalls = 0x7b,
        CallAllTailCalls = 0x7c,
        CallReturnPC = 0x7d,
        CallValue = 0x7e,
        CallOrigin = 0x7f,
        CallParameter = 0x80,
        CallPC = 0x81,
        CallTailCall = 0x82,
        CallTarget = 0x83,
        CallTargetClobbered = 0x84,
        CallDataLocation = 0x85,
        CallDataValue = 0x86,
        NoReturn = 0x87,
        Alignment = 0x88,
        ExportSymbols = 0x89,
        Deleted = 0x8a,
        Defaulted = 0x8b,
        LocListsBase = 0x8c,
        LoUser = 0x2000,
        HiUser = 0x3fff,
    }; // enum class Attribute : u16

    enum class AttributeDataForm : u8 {
        None = 0,
        Addr = 0x1,
        Block2 = 0x3,
        Block4 = 0x4,
        Data2 = 0x5,
        Data4 = 0x6,
        Data8 = 0x7,
        String = 0x8,
        Block = 0x9,
        Block1 = 0xa,
        Data1 = 0xb,
        Flag = 0xc,
        SData = 0xd,
        StringPointer = 0xe,
        UData = 0xf,
        RefAddr = 0x10,
        Ref1 = 0x11,
        Ref2 = 0x12,
        Ref4 = 0x13,
        Ref8 = 0x14,
        RefUData = 0x15,
        Indirect = 0x16,
        SecOffset = 0x17,
        ExprLoc = 0x18,
        FlagPresent = 0x19,
        StrX = 0x1a,
        AddrX = 0x1b,
        RefSup4 = 0x1c,
        StrPSup = 0x1d,
        Data16 = 0x1e,
        LineStrP = 0x1f,
        RefSig8 = 0x20,
        ImplicitConst = 0x21,
        LocListX = 0x22,
        RngListX = 0x23,
        RefSup8 = 0x24,
        StrX1 = 0x25,
        StrX2 = 0x26,
        StrX3 = 0x27,
        StrX4 = 0x28,
        AddrX1 = 0x29,
        AddrX2 = 0x2a,
        AddrX3 = 0x2b,
        AddrX4 = 0x2c
    }; // enum class AttributeDataForm : u8

    struct [[gnu::packed]] AttributeSpecification {
        Attribute attribute;
        AttributeDataForm form;
        ssize_t value;
    }; // struct [[gnu::packed]] AttributeSpecification

    enum class RangeListEntryType : u8 {
        EndOfList = 0,
        BaseAddressX = 0x1,
        StartXEndX = 0x2,
        StartXLength = 0x3,
        OffsetPair = 0x4,
        BaseAddress = 0x5,
        StartEnd = 0x6,
        StartLength = 0x7
    }; // enum class RangeListEntryType : u8

} // namespace Debug::Dwarf
