#!/usr/bin/env python3

# imports
import struct
import re
import math
import json
from sys import argv, stderr
from os import path
from string import whitespace
from tempfile import NamedTemporaryFile
from subprocess import call

atom_end = set('()"' + whitespace)


def parse(sexp):
    sexp = re.sub(r'(?m)\(;.*;\)', '', re.sub(r'(;;.*)', '', sexp))
    stack, i, length = [[]], 0, len(sexp)
    while i < length:
        c = sexp[i]
        kind = type(stack[-1])
        if kind == list:
            if c == '(':
                stack.append([])
            elif c == ')':
                stack[-2].append(stack.pop())
            elif c == '"':
                stack.append('')
            elif c in whitespace:
                pass
            else:
                stack.append((c,))
        elif kind == str:
            if c == '"':
                stack[-2].append(stack.pop())
            elif c == '\\':
                i += 1
                if sexp[i] != '"':
                    stack[-1] += '\\'
                stack[-1] += sexp[i]
            else:
                stack[-1] += c
        elif kind == tuple:
            if c in atom_end:
                atom = stack.pop()
                stack[-1].append(atom)
                continue
            else:
                stack[-1] = ((stack[-1][0] + c),)
        i += 1
    return stack.pop()


def parse_typed_value(ast):
    types = {
        'i32.const': 'i32',
        'i64.const': 'i64',
        'f32.const': 'float',
        'f64.const': 'double',
    }
    if len(ast) == 2 and ast[0][0] in types:
        return {"type": types[ast[0][0]], "value": ast[1][0]}

    return {"type": "error"}


def generate_module_source_for_compilation(entries):
    s = '('
    for entry in entries:
        if type(entry) == tuple and len(entry) == 1 and type(entry[0]) == str:
            s += entry[0] + ' '
        elif type(entry) == str:
            s += json.dumps(entry) + ' '
        elif type(entry) == list:
            s += generate_module_source_for_compilation(entry)
        else:
            raise Exception("wat? I dunno how to pretty print " + str(type(entry)))
    while s.endswith(' '):
        s = s[:len(s) - 1]
    return s + ')'


def generate_binary_source(chunks):
    res = b''
    for chunk in chunks:
        i = 0
        while i < len(chunk):
            c = chunk[i]
            if c == '\\':
                res += bytes.fromhex(chunk[i + 1: i + 3])
                i += 3
                continue
            res += c.encode('utf-8')
            i += 1
    return res


named_modules = {}
named_modules_inverse = {}
registered_modules = {}


def generate(ast):
    global named_modules, named_modules_inverse, registered_modules

    if type(ast) != list:
        return []
    tests = []
    for entry in ast:
        if len(entry) > 0 and entry[0] == ('module',):
            name = None
            mode = 'ast'  
            start_index = 1
            if len(entry) > 1:
                if isinstance(entry[1], tuple) and isinstance(entry[1][0], str) and entry[1][0].startswith('$'):
                    name = entry[1][0]
                    if len(entry) > 2:
                        if isinstance(entry[2], tuple) and entry[2][0] in ('binary', 'quote'):
                            mode = entry[2][0]
                            start_index = 3
                        else:
                            start_index = 2
                elif isinstance(entry[1][0], str):
                    mode = entry[1][0]
                    start_index = 2

            tests.append({
                "module": {
                    'ast': lambda: ('parse', generate_module_source_for_compilation(entry)),
                    'binary': lambda: ('literal', generate_binary_source(entry[start_index:])),
                    'quote': lambda: ('literal', entry[start_index]),
                }[mode](),
                "tests": []
            })

            if name is not None:
                named_modules[name] = len(tests) - 1
                named_modules_inverse[len(tests) - 1] = (name, None)
        elif len(entry) in [2, 3] and entry[0][0].startswith('assert_'):
            if entry[1][0] == ('invoke',):
                arg, name, module = 0, None, None
                if isinstance(entry[1][1], str):
                    name = entry[1][1]
                else:
                    name = entry[1][2]
                    module = named_modules[entry[1][1][0]]
                    arg = 1
                tests[-1]["tests"].append({
                    "kind": entry[0][0][len('assert_'):],
                    "function": {
                        "module": module,
                        "name": name,
                        "args": list(parse_typed_value(x) for x in entry[1][arg + 2:])
                    },
                    "result": parse_typed_value(entry[2]) if len(entry) == 3 + arg else None
                })
            elif entry[1][0] == ('get',):
                arg, name, module = 0, None, None
                if isinstance(entry[1][1], str):
                    name = entry[1][1]
                else:
                    name = entry[1][2]
                    module = named_modules[entry[1][1][0]]
                    arg = 1
                tests[-1]["tests"].append({
                    "kind": entry[0][0][len('assert_'):],
                    "get": {
                        "name": name,
                        "module": module,
                    },
                    "result": parse_typed_value(entry[2]) if len(entry) == 3 + arg else None
                })
            else:
                if not len(tests):
                    tests.append({
                        "module": ('literal', b""),
                        "tests": []
                    })
                tests[-1]["tests"].append({
                    "kind": "testgen_fail",
                    "function": {
                        "module": None,
                        "name": "<unknown>",
                        "args": []
                    },
                    "reason": f"Unknown assertion {entry[0][0][len('assert_'):]}"
                })
        elif len(entry) >= 2 and entry[0][0] == 'invoke':
            arg, name, module = 0, None, None
            if not isinstance(entry[1], str) and isinstance(entry[1][1], str):
                name = entry[1][1]
            elif isinstance(entry[1], str):
                name = entry[1]
            else:
                name = entry[1][2]
                module = named_modules[entry[1][1][0]]
                arg = 1
            tests[-1]["tests"].append({
                "kind": "ignore",
                "function": {
                    "module": module,
                    "name": name,
                    "args": list(parse_typed_value(x) for x in entry[1][arg + 2:])
                },
                "result": parse_typed_value(entry[2]) if len(entry) == 3 + arg else None
            })
        elif len(entry) > 1 and entry[0][0] == 'register':
            if len(entry) == 3:
                registered_modules[entry[1]] = named_modules[entry[2][0]]
                x = named_modules_inverse[named_modules[entry[2][0]]]
                named_modules_inverse[named_modules[entry[2][0]]] = (x[0], entry[1])
            else:
                index = len(tests) - 1
                registered_modules[entry[1]] = index
                named_modules_inverse[index] = (":" + entry[1], entry[1])
        else:
            if not len(tests):
                tests.append({
                    "module": ('literal', b""),
                    "tests": []
                })
            tests[-1]["tests"].append({
                "kind": "testgen_fail",
                "function": {
                    "module": None,
                    "name": "<unknown>",
                    "args": []
                },
                "reason": f"Unknown command {entry[0][0]}"
            })
    return tests
