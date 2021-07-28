import gdb
import gdb.types
import re


def handler_class_for_type(type, re=re.compile('^([^<]+)(<.*>)?$')):
    typename = str(type.tag)

    match = re.match(typename)
    if not match:
        return None

    klass = match.group(1)

    if klass == 'Base::Atomic':
        return BaseAtomic
    elif klass == 'Base::DistinctNumeric':
        return BaseDistinctNumeric
    elif klass == 'Base::HashMap':
        return BaseHashMapPrettyPrinter
    elif klass == 'Base::RefCounted':
        return BaseRefCounted
    elif klass == 'Base::RefPtr':
        return BaseRefPtr
    elif klass == 'Base::OwnPtr':
        return BaseOwnPtr
    elif klass == 'Base::NonnullRefPtr':
        return BaseRefPtr
    elif klass == 'Base::SinglyLinkedList':
        return BaseSinglyLinkedList
    elif klass == 'Base::String':
        return BaseString
    elif klass == 'Base::StringView':
        return BaseStringView
    elif klass == 'Base::StringImpl':
        return BaseStringImpl
    elif klass == 'Base::Variant':
        return BaseVariant
    elif klass == 'Base::Vector':
        return BaseVector
    elif klass == 'VirtualAddress':
        return VirtualAddress
    else:
        return UnhandledType


class UnhandledType:
    @classmethod
    def prettyprint_type(cls, type):
        return type.name


class BaseAtomic:
    def __init__(self, val):
        self.val = val

    def to_string(self):
        return self.val["m_value"]

    @classmethod
    def prettyprint_type(cls, type):
        contained_type = type.template_argument(0)
        return f'Base::Atomic<{handler_class_for_type(contained_type).prettyprint_type(contained_type)}>'


class BaseDistinctNumeric:
    def __init__(self, val):
        self.val = val

    def to_string(self):
        return self.val["m_value"]

    @classmethod
    def prettyprint_type(cls, type):
        actual_name = type.template_argument(1)
        parts = actual_name.name.split("::")
        unqualified_name = re.sub(r'__(\w+)_tag', r'\1', actual_name.name)
        if unqualified_name != actual_name.name:
            qualified_name = '::'.join(parts[:-2] + [unqualified_name])
            return qualified_name
        contained_type = type.template_argument(0)
        return f'Base::DistinctNumeric<{handler_class_for_type(contained_type).prettyprint_type(contained_type)}>'


class BaseRefCounted:
    def __init__(self, val):
        self.val = val

    def to_string(self):
        return self.val["m_ref_count"]

    @classmethod
    def prettyprint_type(cls, type):
        contained_type = type.template_argument(0)
        return f'Base::RefCounted<{handler_class_for_type(contained_type).prettyprint_type(contained_type)}>'


class BaseString:
    def __init__(self, val):
        self.val = val

    def to_string(self):
        if int(self.val["m_impl"]["m_bits"]["m_value"]) == 0:
            return '""'
        else:
            impl = BaseRefPtr(self.val["m_impl"]).get_pointee().dereference()
            return BaseStringImpl(impl).to_string()

    @classmethod
    def prettyprint_type(cls, type):
        return 'Base::String'


class BaseStringView:
    def __init__(self, val):
        self.val = val

    def to_string(self):
        if int(self.val["m_length"]) == 0:
            return '""'
        else:
            characters = self.val["m_characters"]
            str_type = characters.type.target().array(self.val["m_length"]).pointer()
            return str(characters.cast(str_type).dereference())

    @classmethod
    def prettyprint_type(cls, type):
        return 'Base::StringView'


def get_field_unalloced(val, member, type):

    return gdb.parse_and_eval(f"*({type}*)(({val.type.name}*){int(val.address)})->{member}")
