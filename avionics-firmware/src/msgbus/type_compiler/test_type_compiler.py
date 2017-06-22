import unittest
from unittest.mock import Mock
from type_compiler import *


class TestParser(unittest.TestCase):

    def test_line_split(self):
        line = 'hello world # comment    \n'
        expr, comment = split_line_in_expression_and_comment(line)
        self.assertEqual(['hello', 'world'], expr)
        self.assertEqual(' comment', comment)

    def test_line_split_empty(self):
        line = '\n'
        expr, comment = split_line_in_expression_and_comment(line)
        self.assertEqual([], expr)
        self.assertEqual('', comment)

    def test_parse_minimal_type_definition(self):
        lines = [
            'simple:',
            '    int32 x'
        ]
        t = parse(iter(enumerate(lines)))
        expect = [
            TypeDefinition(
                typename='simple',
                docstring=[],
                entries=[
                    TypeDefinition.Entry(
                        type='int32',
                        name='x',
                        docstring='')
                ]
            )
        ]
        self.assertEqual(list(t), expect)

    def test_parse_type_definition(self):
        lines = [
            '# multiline',
            '# comment',
            '',
            '# example docstring',
            'example:',
            '    int32 x # x docstring',
            '    float arr[10]',
            '    string(10) str'
        ]
        t = parse(iter(enumerate(lines)))
        expect = [
            CommentBlock([' multiline', ' comment']),
            WhitespaceBlock(1),
            TypeDefinition(
                typename='example',
                docstring=[' example docstring'],
                entries=[
                    TypeDefinition.Entry(type='int32', name='x', docstring=' x docstring'),
                    TypeDefinition.Entry(type='float', name='arr', array_sz=10),
                    TypeDefinition.Entry(type='string', name='str', str_len=10),
                ]
            )
        ]
        self.assertEqual(list(t), expect)


class TestTypeHashGeneration(unittest.TestCase):

    def setUp(self):
        custom_subtype = TypeDefinition('custom', [TypeDefinition.Entry(type='int8', name='x')])
        custom_subtype = TypeDefinition('custom', [TypeDefinition.Entry(type='int8', name='x')])
        self.entries = [TypeDefinition.Entry(type='int8', name='foo'),
                        TypeDefinition.Entry(type='custom', name='bar')]
        self.typedef = TypeDefinition('test', self.entries)
        self.typedict = {'custom': custom_subtype, 'test': self.typedef}

    def test_type_name_changes_hash(self):
        a = TypeDefinition('foo', self.entries)
        b = TypeDefinition('bar', self.entries)
        self.assertFalse(a.get_hash({}) == b.get_hash({}))

    def test_entry_name_changes_hash(self):
        a_entries = [TypeDefinition.Entry(type='int8', name='foo')]
        b_entries = [TypeDefinition.Entry(type='int8', name='bar')]
        a = TypeDefinition('test', a_entries)
        b = TypeDefinition('test', b_entries)
        self.assertFalse(a.get_hash({}) == b.get_hash({}))

    def test_entry_type_changes_hash(self):
        a_entries = [TypeDefinition.Entry(type='int8', name='foo')]
        b_entries = [TypeDefinition.Entry(type='int16', name='foo')]
        a = TypeDefinition('test', a_entries)
        b = TypeDefinition('test', b_entries)
        self.assertFalse(a.get_hash({}) == b.get_hash({}))

    def test_custom_entry_type_changes_hash(self):
        a = Mock()
        a.get_hash = Mock(return_value=b'aaaa')
        b = Mock()
        b.get_hash = Mock(return_value=b'bbbb')
        typedict = {'a': a, 'b': b}
        a_entries = [TypeDefinition.Entry(type='a', name='foo')]
        b_entries = [TypeDefinition.Entry(type='b', name='foo')]
        a = TypeDefinition('test', a_entries)
        b = TypeDefinition('test', b_entries)
        self.assertFalse(a.get_hash(typedict) == b.get_hash(typedict))

    def test_array_size_changes_hash(self):
        a_entries = [TypeDefinition.Entry(type='int8', name='foo', array_sz=2)]
        b_entries = [TypeDefinition.Entry(type='int8', name='foo', array_sz=3)]
        a = TypeDefinition('test', a_entries)
        b = TypeDefinition('test', b_entries)
        self.assertFalse(a.get_hash({}) == b.get_hash({}))

    def test_dynamic_array_changes_hash(self):
        a_entries = [TypeDefinition.Entry(type='int8', name='foo', array_sz=2)]
        b_entries = [TypeDefinition.Entry(type='int8', name='foo', array_sz=2, dynamic_array=True)]
        a = TypeDefinition('test', a_entries)
        b = TypeDefinition('test', b_entries)
        self.assertFalse(a.get_hash({}) == b.get_hash({}))

    def test_string_size_changes_hash(self):
        a_entries = [TypeDefinition.Entry(type='string', name='foo', str_len=2)]
        b_entries = [TypeDefinition.Entry(type='string', name='foo', str_len=3)]
        a = TypeDefinition('test', a_entries)
        b = TypeDefinition('test', b_entries)
        self.assertFalse(a.get_hash({}) == b.get_hash({}))

    def test_docstring_doesnt_change_hash(self):
        a_entries = [TypeDefinition.Entry(type='int8', name='foo', docstring='bla')]
        b_entries = [TypeDefinition.Entry(type='int8', name='foo')]
        a = TypeDefinition('test', a_entries, docstring='bla')
        b = TypeDefinition('test', b_entries)
        self.assertEqual(a.get_hash({}), b.get_hash({}))


class TestCCodeGeneration(unittest.TestCase):

    def test_C_struct_entry(self):
        entry = TypeDefinition.Entry(type='int32', name='foo')
        self.assertEqual(['    int32_t foo;'], C_struct_entry(entry))

    def test_C_struct_entry_string(self):
        entry = TypeDefinition.Entry(type='string', str_len=10, name='foo')
        self.assertEqual(['    char foo[11];'], C_struct_entry(entry))

    def test_C_struct_entry_array(self):
        entry = TypeDefinition.Entry(type='int32', name='foo', array_sz=10)
        self.assertEqual(['    int32_t foo[10];'], C_struct_entry(entry))

    def test_C_struct_entry_dynamic_array(self):
        entry = TypeDefinition.Entry(type='int32', name='foo', array_sz=10, dynamic_array=True)
        self.assertEqual(['    int32_t foo[10];',
                          '    uint16_t foo_len;'], C_struct_entry(entry))

    def test_C_struct_entry_dynamic_array_of_strings(self):
        entry = TypeDefinition.Entry(type='string', str_len=10, name='args', array_sz=3, dynamic_array=True)
        self.assertEqual(['    char args[3][11];',
                          '    uint16_t args_len;'], C_struct_entry(entry))

    def test_C_type_definition_custom_type_entry(self):
        e = TypeDefinition.Entry(type='custom', name='x')
        type_defintion = generate_C_type_definition_entry('test', e)
        expect = [
            '    {',
            '        .name = "x",',
            '        .is_base_type = 0,',
            '        .is_array = 0,',
            '        .is_dynamic_array = 0,',
            '        .array_len = 0,',
            '        .dynamic_array_len_struct_offset = 0,',
            '        .struct_offset = offsetof(test_t, x),',
            '        .type = &custom_type,',
            '        .size = sizeof(custom_t),',
            '    },'
        ]
        self.assertEqual(expect, type_defintion)

    def test_C_type_definition_string_type_entry(self):
        e = TypeDefinition.Entry(type='string', str_len=10, name='str')
        type_defintion = generate_C_type_definition_entry('test', e)
        expect = [
            '    {',
            '        .name = "str",',
            '        .is_base_type = 1,',
            '        .is_array = 0,',
            '        .is_dynamic_array = 0,',
            '        .array_len = 0,',
            '        .dynamic_array_len_struct_offset = 0,',
            '        .struct_offset = offsetof(test_t, str),',
            '        .base_type = MSGBUS_TYPE_STRING,',
            '        .size = sizeof(char[11]),',
            '    },'
        ]
        self.assertEqual(expect, type_defintion)

    def test_C_type_definition_array_entry(self):
        e = TypeDefinition.Entry(type='int32', array_sz=100, name='x')
        type_defintion = generate_C_type_definition_entry('test', e)
        expect = [
            '    {',
            '        .name = "x",',
            '        .is_base_type = 1,',
            '        .is_array = 1,',
            '        .is_dynamic_array = 0,',
            '        .array_len = 100,',
            '        .dynamic_array_len_struct_offset = 0,',
            '        .struct_offset = offsetof(test_t, x),',
            '        .base_type = MSGBUS_TYPE_INT32,',
            '        .size = sizeof(int32_t),',
            '    },'
        ]
        self.assertEqual(expect, type_defintion)

    def test_C_type_definition_dyn_array_entry(self):
        e = TypeDefinition.Entry(type='int32', array_sz=100, dynamic_array=True, name='x')
        type_defintion = generate_C_type_definition_entry('test', e)
        expect = [
            '    {',
            '        .name = "x",',
            '        .is_base_type = 1,',
            '        .is_array = 0,',
            '        .is_dynamic_array = 1,',
            '        .array_len = 100,',
            '        .dynamic_array_len_struct_offset = offsetof(test_t, x_len),',
            '        .struct_offset = offsetof(test_t, x),',
            '        .base_type = MSGBUS_TYPE_INT32,',
            '        .size = sizeof(int32_t),',
            '    },'
        ]
        self.assertEqual(expect, type_defintion)

    def test_C_type_definition_object(self):
        t = TypeDefinition('test', [None]*3)
        t.get_hash = Mock(return_value=b'\xaa\xbb\xcc\xdd')
        type_defintion = generate_C_type_definition_object(t, {})
        expect = [
            'const msgbus_type_definition_t test_type = {',
            '    .hash = {0xaa, 0xbb, 0xcc, 0xdd},',
            '    .nb_elements = 3,',
            '    .elements = test_entries,',
            '    .struct_size = sizeof(test_t),',
            '};'
        ]
        self.assertEqual(expect, type_defintion)
        t.get_hash.assert_called_once_with({})
