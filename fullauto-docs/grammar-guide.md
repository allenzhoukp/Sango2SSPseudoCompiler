# Sango Smart Script Language Grammar Guide

Status: descriptive specification of the current v0.9.8 working tree.

This document describes the language accepted by the compiler in `cc/` as it
behaves today. It is not a description of C or C++. Sango Smart Script borrows
C syntax, but its preprocessing, precedence, declarations, scopes, and runtime
model differ in important ways.

The guide deliberately separates:

1. source-file preprocessing;
2. lexical and concrete grammar;
3. context-sensitive and type rules;
4. generated Sango VM assembly.

That separation is important for any future parser-generator grammar. Includes,
raw `#define` substitution, configured structure names, symbol resolution, and
`__asm` rewriting cannot be represented faithfully by context-free grammar
rules alone.

## 1. Sources of truth and compatibility policy

The current implementation is distributed across these compiler stages:

- `Lexer.cpp`: file loading, includes, defines, comments, literals, operators,
  and opaque `__asm` blocks;
- `BranchAnalysis.cpp`: top-level declarations, functions, statements, and
  control flow;
- `TreeConstruction.cpp`: expression parsing, precedence, lvalue checks, and
  most type inference;
- `TreeAnalysis.cpp`: expression-to-assembly lowering;
- `FixIdentifierLoader.cpp`: configured syscalls, INTV names, and structure
  types;
- `magic_inst.txt`: VM instruction and syscall reference.

When this guide disagrees with `Userguide_CN.txt`, the behavior of the current
compiler source is reported as the implemented behavior. Known disagreements
are collected in section 14 rather than silently resolved.

The compatibility baseline for future work should be:

- every source file that currently compiles must continue to compile;
- its generated assembly must remain unchanged except for explicitly
  nondeterministic text such as the compilation timestamp;
- invalid or previously crashing inputs may receive safer diagnostics;
- documented syntax that the current parser rejects should not be enabled as a
  side effect of a parser or performance refactor.

## 2. Grammar notation

The formal fragments below use this EBNF notation:

```text
rule          = required sequence ;
[ item ]      = optional item ;
{ item }      = zero or more repetitions ;
( a | b )     = choice ;
"text"        = a literal token ;
UPPER_CASE    = a lexical token described separately ;
```

Some rules are followed by semantic restrictions. Those restrictions are part
of the accepted language even when they cannot be expressed in plain EBNF.

## 3. Compilation and preprocessing model

### 3.1 Files and processing order

The compiler is invoked with one root source file. Configuration is loaded from
paths relative to the process working directory:

```text
config/CONFIG.txt
config/SYSCALL_TABLE.txt
config/INTV_TABLE.txt
config/STRUCT_TABLE.txt
```

If `use_exe_memory_io` is `0`, the file named by
`script_memory_io_file` is automatically loaded. With the default
configuration this is `sg2lang.h`. Because the lexer uses a file stack, the
automatically loaded file is tokenized before the root source file.

`#include` filenames are also opened relative to the process working directory,
not relative to the including file. A filename is included only once, using its
literal filename string as the identity. Repeated and circular includes are
therefore skipped when the spelling is identical.

### 3.2 Include directives

Supported forms are:

```text
#include "filename"
#include <filename>
```

Whitespace is allowed between `include` and the opening delimiter. The current
filename matcher rejects `<`, `>`, `|`, `:`, `"`, `*`, and `?` inside the
filename. Both `/` and `\` can occur lexically, although path behavior is
platform-dependent.

An include directive is consumed by the lexer and does not produce a parser
token.

### 3.3 Define directives

The only supported definition form is an object-like textual replacement:

```text
#define ORIGINAL REPLACEMENT_TEXT
```

Important behavior:

- `ORIGINAL` is the next non-whitespace sequence, not necessarily an
  identifier.
- `REPLACEMENT_TEXT` is required to be nonempty and extends to the end of the
  physical line, with trailing whitespace consumed by the directive matcher.
- there are no function-like macros, arguments, `#if`, `#ifdef`, token pasting,
  stringification, or recursive expansion rules;
- replacement is raw substring replacement, not token-aware replacement;
- it can therefore replace text inside longer identifiers, comments, string
  literals, and assembly blocks;
- a new definition is applied to the unread portion of every file currently on
  the include stack;
- existing definitions are applied to each newly loaded file;
- scanning resumes after the inserted replacement, which limits immediate
  self-replacement but does not provide C-preprocessor semantics.

Any other line beginning with `#` is skipped as a comment-like directive.

### 3.4 Comments

Outside `__asm`, the lexer recognizes:

```text
// line comment followed by a newline
/* block comment */
```

Comments are removed before parsing. A `//` comment at the physical end of a
file without a terminating newline is a known lexical edge case; see section
14.

### 3.5 Opaque assembly blocks

The lexer recognizes `__asm` followed by optional whitespace and `{` as one
opaque token:

```text
__asm {
    VM instructions
}
```

The closing brace is found by a custom scanner. Nested braces are not parsed.
Double-quoted strings and assembly comments receive limited special handling so
that many braces inside them do not terminate the block. `//` inside an
assembly block is converted to `;`, the assembler comment marker.

During statement lowering, the compiler performs additional textual rewriting
inside the assembly token, in this order:

1. local variable names become their numeric local slots;
2. parameter names become their negative parameter slots;
3. `PUSHSTR "text"` literals become string-table indices;
4. configured INTV names become numeric INTV indices.

These replacements use word-oriented regular expressions but are not a real
assembly parser. They may also affect matching text in assembly comments.
Global variable names are not rewritten inside `__asm`.

## 4. Lexical grammar

### 4.1 Whitespace

Spaces, tabs, LF, and CRLF separate tokens. Newline tokens are not delivered to
the parser. Source filename, line, and column are stored on each token for
diagnostics.

### 4.2 Identifiers and keywords

```text
IDENTIFIER = ("A".."Z" | "a".."z" | "_")
             { "A".."Z" | "a".."z" | "0".."9" | "_" } ;
```

All words initially lex as identifiers. Keywords such as `if`, `int`, and
`asynccall` are recognized by parser context. Identifiers are case-sensitive.

The regular expression implementation uses `\w` after the first character.
The intended and portable source alphabet is ASCII letters, digits, and
underscore.

Reserved-by-use words include:

```text
asynccall  break     byte       callsign  case      continue
default    do        else       extern    false     float
for        function  goto       if        int       naked
NULL       null      return     short     string    switch
true       unsigned  void       while
```

Intrinsic call names such as `GetGlobal`, `SetGlobal`, `GetIntv`, `SetIntv`,
`Delay`, `Wait`, and `IsRunning` are recognized in expression context. Names
loaded from the syscall and INTV tables are also effectively reserved when
visible.

### 4.3 Integer literals

Implemented token forms are:

```text
DECIMAL_OR_OCTAL = DIGIT { DIGIT } ;
HEX_PREFIX       = "0x" HEX_DIGIT { HEX_DIGIT } ;
HEX_SUFFIX       = DIGIT { HEX_DIGIT } "h" ;
```

Behavior:

- a digit sequence beginning with `0` is passed to `sscanf` as octal;
- other digit sequences are parsed as decimal;
- prefix hexadecimal requires lowercase `0x`;
- suffix hexadecimal must begin with a decimal digit because lexer dispatch
  tests identifiers before number patterns; later digits may use either case,
  and the suffix is lowercase `h`;
- a leading sign is not part of the literal and is parsed as unary `-`;
- integer storage is the host C++ `int` used by the compiler.

An octal-looking token is not validated after `sscanf`. For example, a leading
zero followed by `8` or `9` can be tokenized as one integer while only the
valid octal prefix contributes to its stored value.

### 4.4 Floating-point literals

The lexer accepts these broad forms:

```text
FLOAT =
      DIGIT* "." DIGIT+ [ EXPONENT ] [ "f" | "F" ]
    | DIGIT+ "."          [ EXPONENT ] [ "f" | "F" ]
    | DIGIT+ EXPONENT
    | DIGIT+ ( "f" | "F" ) ;

EXPONENT = ( "e" | "E" ) [ "+" | "-" ] DIGIT+ ;
```

Thus `1.0`, `.5`, `1.`, `3.78e+12`, and the currently used `1f` are accepted.
The sign before the significand remains a unary operator.

### 4.5 String and character-looking literals

Both quote styles produce the same string token:

```text
STRING = ( '"' | "'" ) STRING_CONTENT ( '"' | "'" ) ;
```

There is no distinct character type. `'a'` is a string value, not an integer
character constant. The current scanner does not remember which quote opened
the literal: either unescaped quote character closes it. A single quote inside
a double-quoted literal, or vice versa, must therefore be escaped to survive
lexing. The lexer preserves backslash escape spelling; a later unescape pass
handles `\\`, `\n`, `\t`, `\a`, `\b`, `\f`, `\r`, `\v`, escaped quotes,
`\?`, and two-digit `\xHH` escapes when emitting the string table. An unknown
escape is dropped rather than preserved. The scanner contains special handling
for legacy two-byte Big5 text.

### 4.6 Operators and punctuation

Recognized multi-character tokens are:

```text
<<= >>= -> ++ -- << >> >= <= == != && ||
+=  -=  *= /= %= ^= &= |=
```

Recognized single-character tokens are:

```text
- + * / % = ~ ! ^ & | , . ; : < > ( ) [ ] { }
```

Recognition does not imply parser support in every context. In particular,
`%=` is tokenized and has lowering code but is omitted from the current
expression-parser operator set.

## 5. Types

### 5.1 Implemented type syntax

The effective current type grammar is:

```text
type              = base-type { "*" } ;

base-type         = "void"
                  | "int"
                  | "float"
                  | "string"
                  | "short"
                  | "byte"
                  | "unsigned"
                  | CONFIGURED_STRUCT_NAME ;
```

`CONFIGURED_STRUCT_NAME` is not a fixed keyword set. It comes from
`config/STRUCT_TABLE.txt`. Names are compared after converting all-uppercase
underscore spelling to camel case, so both `SOLDIER_INFO` and `SoldierInfo`
resolve to the same table entry.

The source contains intended handling for `unsigned int`, `unsigned short`, and
`unsigned byte`, but the current conditional order makes those multiword forms
unreachable in ordinary source parsing. Only bare `unsigned` is reliably
recognized as the unsigned 32-bit integer type.

### 5.2 Internal type categories

The VM directly distinguishes three stack-value categories:

- integer-like values;
- floating-point values;
- strings.

The compiler additionally tracks:

- signed and unsigned byte/short/int source types;
- any number of pointer indirections, encoded in the high bits of a type code;
- configured structure values and structure pointers;
- executable function pointers.

All pointer values are treated as integer-compatible by general type
validation. They represent addresses in the game executable's memory. They do
not point to compiler locals or script globals, and there is no address-of
operator.

Structure values mainly exist as intermediate results while traversing embedded
structures and structure arrays. Normal script code should store structure
pointers rather than whole structures.

### 5.3 Casts

```text
cast-expression = "(" type ")" unary-expression ;
```

Implemented conversions include:

- integer-like to float and float to integer-like;
- integer-like or float to string;
- casts among integer-like and pointer types for type inference;
- identity casts.

String-to-number conversion is rejected. Direct float/string-to-pointer casts
and casts to non-pointer `void` are rejected by the current semantic checks.

## 6. Translation-unit grammar

The high-level grammar is:

```text
translation-unit       = { top-level-declaration } EOF ;

top-level-declaration  = extern-function-declaration
                       | typed-top-level-declaration ;

extern-function-declaration
                      = "extern" type [ "function" ] IDENTIFIER
                        "(" [ extern-parameter-list ] ")"
                        [ "callsign" INTEGER_LITERAL ] ";" ;

extern-parameter-list = extern-parameter { "," extern-parameter } ;
extern-parameter      = type [ IDENTIFIER ] ;

typed-top-level-declaration
                      = type [ "naked" ] [ "function" ] IDENTIFIER
                        ( function-rest | global-rest ) ;

function-rest         = "(" [ named-parameter-list ] ")"
                        [ "callsign" INTEGER_LITERAL ]
                        function-body ;

named-parameter-list = named-parameter { "," named-parameter } ;
named-parameter      = type IDENTIFIER ;

function-body        = statement ;

global-rest          = [ array-suffix ]
                        { "," variable-declarator }
                        ";" ;

variable-declarator  = { "*" } IDENTIFIER [ array-suffix ] ;
array-suffix         = "[" integer-constant-object "]" ;

integer-constant-object
                      = INTEGER_LITERAL
                      | "true"
                      | "false"
                      | "NULL"
                      | "null"
                      | "(" integer-constant-object ")" ;
```

### 6.1 Function definitions

Examples:

```text
void InitializeBattle(int attacker, int defender) {
    return;
}

int function GetValue() callsign 1001 {
    return 1;
}
```

`function` is optional compatibility syntax. `callsign` is optional and
defaults to zero.

A normal function body is parsed as one statement. Usually that statement is a
brace-delimited block, but the parser also accepts a single unbraced statement.

Function names must be unique across internal and extern declarations. The
function is entered into the symbol table immediately before its body is
parsed, so direct recursion is supported.

Calls to later internal functions are not supported: a normal call target must
already exist in the function map when its call expression is parsed. There is
no ordinary internal forward-declaration syntax.

### 6.2 Naked functions

```text
naked-function = type "naked" [ "function" ] IDENTIFIER
                 "(" [ named-parameter-list ] ")"
                 [ "callsign" INTEGER_LITERAL ] ASM_BLOCK ;
```

A naked function must be followed directly by exactly one lexer-level
`__asm { ... }` block. The compiler does not emit a stack prologue or automatic
return for it.

### 6.3 Extern functions

Extern declarations have no body. Parameter names are optional and, when
omitted, are synthesized internally.

```text
extern void InitializeBattle(int, int);
extern int function QueryValue(int objectId) callsign 0;
```

Extern functions cannot be called synchronously. They are valid only as the
target of `asynccall`.

### 6.4 Global variables

```text
int globalValue;
int values[10];
SangoObject* object, *objects[10];
```

Global initializers are rejected. Multiple declarators and one-dimensional
constant-length arrays are accepted. A global array is lowered to a base slot
plus hidden element slots.

Pointer stars on comma-separated globals have a non-C inheritance quirk. The
first declarator gets the stars consumed as part of `type`. A later declarator
without explicit stars retains the most recently used star count; explicit
stars reset it. Thus `int *a, b;` currently makes both variables pointers,
whereas the analogous local declaration makes only `a` a pointer. Mixing
pointer depths in one global declaration should be avoided.

The parser technically consumes `naked` before it knows whether a top-level
declaration is a function, so `naked` can be ignored on a global declaration.
This is an implementation quirk, not recommended source syntax. By contrast,
writing `function` and then failing to provide `(` is rejected.

For backward compatibility, the generated assembly declares at least 121
global slots even when fewer are defined by the source. The exact header value
is `max(globalCount + 1, 121)`, reserving the zero position used by the format.

## 7. Local declarations and function pointers

### 7.1 Local variables

```text
local-declaration     = base-type initialized-declarator
                        { "," initialized-declarator } ";" ;

initialized-declarator
                      = { "*" } IDENTIFIER
                        [ "=" expression | array-suffix ] ;
```

Examples:

```text
int x = 1, y, values[10];
SangoObject* object, *objects[10];
string message = "hello";
```

Local array initializers are not supported. Array lengths are parsed through
the primary-object parser and must produce an integer-constant node. Negative
lengths and general constant expressions are therefore not supported, while
the constant objects listed above may pass. Zero lengths are not explicitly
rejected. A non-pointer `void` local is rejected; `void*` is accepted because
the pointer star belongs to its declarator.

All locals have function scope, regardless of the block in which they appear.
Redeclaring a local or parameter name anywhere in that function is rejected.
A local shadows a global with the same name.

### 7.2 Executable function-pointer declarations

Function pointers use a dedicated statement form:

```text
function-pointer-declaration
    = type "(" "*" IDENTIFIER ")"
      "(" [ function-pointer-parameter-list ] ")"
      [ "=" expression ] ";" ;

function-pointer-parameter-list
    = named-parameter { "," named-parameter } ;
```

Example:

```text
SangoObject* (*GetObjectByHandle)(int handle) = 0x421A80;
SangoObject* object = GetObjectByHandle(intvAttackerMajor);
```

Function-pointer parameters require names. The pointer itself is a local
variable and may be assigned at runtime. Calling it dynamically installs its
address into the syscall slot configured by `funcptr_syscall`, then emits a
syscall through that slot.

## 8. Statement grammar

```text
statement             = asm-statement
                      | if-statement
                      | while-statement
                      | do-while-statement
                      | for-statement
                      | switch-statement
                      | jump-statement
                      | function-pointer-declaration
                      | local-declaration
                      | label-statement
                      | asynchronous-call-statement
                      | compound-statement
                      | expression-statement ;

compound-statement    = "{" { statement } "}" ;
expression-statement  = expression ";" ;
asm-statement         = ASM_BLOCK ;
```

There is no general empty statement: a bare `;` reaches the expression parser
and is rejected.

### 8.1 If

```text
if-statement = "if" "(" expression ")" statement
               [ "else" statement ] ;
```

`else if` works because the statement following `else` may itself be an
`if-statement`.

The condition must be non-string. Integer-like values are used directly and a
float condition is converted to integer before the branch instruction.

### 8.2 While and do-while

```text
while-statement    = "while" "(" expression ")" statement ;
do-while-statement = "do" statement "while"
                     "(" expression ")" ";" ;
```

`break` targets the loop end. `continue` targets the loop condition for
`while`/`do-while` and the increment stage for `for`.

### 8.3 For

The conventional supported subset is:

```text
for-statement = "for" "("
                ( local-declaration | expression-statement )
                expression ";"
                expression
                ")" statement ;
```

All three clauses are required; empty initializer, condition, or increment
clauses are not supported.

Internally, the initializer is parsed by the full statement parser rather than
a dedicated `for-init` rule. This can admit unusual constructs, but scripts
should restrict it to a local declaration or expression statement.

### 8.4 Switch

```text
switch-statement = "switch" "(" expression ")" "{"
                   { case-clause }
                   [ default-clause ]
                   "}" ;

case-clause      = "case" integer-constant-object ":"
                   { statement } ;

default-clause   = "default" ":" { statement } ;
```

The switch expression must be non-string; a float is converted to integer. A
case value is parsed as a single primary object, not a full constant expression,
and its resulting node must be an integer constant. A unary negative case such
as `case -1:` is not accepted by this path.

Cases fall through unless control leaves explicitly. `default`, when present,
must be the last clause because the current parser consumes the closing brace
immediately after it.

### 8.5 Labels and jumps

```text
label-statement = IDENTIFIER ":" ;

jump-statement  = "break" ";"
                | "continue" ";"
                | "goto" IDENTIFIER ";"
                | "return" [ expression ] ";" ;
```

Labels are stored in one translation-unit-wide set, not per-function scope.
Duplicate labels are rejected even when they occur in different functions.
Labels inside `__asm` are invisible to this set.

The implementation recognizes a label by checking whether the next token is
`:`, without verifying that the first token is an identifier. The EBNF shows
the safe and assembler-usable subset; other token kinds before `:` are parser
quirks rather than recommended labels.

Forward `goto` is emitted even though the target is not yet known. The current
Chinese localization suppresses the warning text for an unknown target.

`break` and `continue` are only safe in contexts where their corresponding
target stacks are nonempty. The current implementation does not diagnose all
out-of-context uses before accessing those stacks.

For a non-void function, `return` requires an expression. For a void function,
`return` takes no expression. The current end-of-function return check tracks
the most recently parsed statement rather than performing full control-flow
analysis.

### 8.6 Asynchronous calls

```text
asynchronous-call-statement
    = "asynccall" IDENTIFIER
      "(" [ argument-list ] ")" ";" ;

argument-list = expression { "," expression } ;
```

The target must already exist in the function map and may be internal or
extern. Argument count and types are checked against the declaration. The
return value, if any, is discarded.

Arguments are pushed after a string-table reference to the function name. Calls
up to `callbs_max_params` use `CALLBS`; larger calls use syscall numbers derived
from `asynccall_sys`.

Syscalls and intrinsic instructions cannot be asynchronous-call targets.

## 9. Expression grammar and precedence

The implemented precedence, from lowest to highest, is:

| Level | Operators | Associativity |
|---:|---|---|
| 0 | `= += -= *= /= &= |= ^= <<= >>=` | left |
| 1 | `&& ||` | left |
| 2 | `& | ^` | left |
| 3 | `== != < <= > >=` | left |
| 4 | `<< >>` | left |
| 5 | `+ -` | left |
| 6 | `* / %` | left |
| 7 | prefix unary and casts | prefix |
| 8 | `[] ->` and postfix `++ --` | postfix |

This is not C precedence:

- assignment operators are parsed left-associatively;
- `&&` and `||` share one level;
- `&`, `|`, and `^` share one level;
- equality and relational operators share one level;
- logical operators do not short-circuit.

The expression EBNF is:

```text
expression         = assignment-expression ;

assignment-expression
                   = logical-expression
                     { assignment-operator logical-expression } ;

assignment-operator
                   = "=" | "+=" | "-=" | "*=" | "/="
                   | "&=" | "|=" | "^=" | "<<=" | ">>=" ;

logical-expression = bitwise-expression
                     { ( "&&" | "||" ) bitwise-expression } ;

bitwise-expression = comparison-expression
                     { ( "&" | "|" | "^" ) comparison-expression } ;

comparison-expression
                   = shift-expression
                     { ( "==" | "!=" | "<" | "<=" | ">" | ">=" )
                       shift-expression } ;

shift-expression   = additive-expression
                     { ( "<<" | ">>" ) additive-expression } ;

additive-expression
                   = multiplicative-expression
                     { ( "+" | "-" ) multiplicative-expression } ;

multiplicative-expression
                   = unary-expression
                     { ( "*" | "/" | "%" ) unary-expression } ;

unary-expression   = ( "!" | "-" | "~" | "++" | "--" | "*" )
                     unary-expression
                   | cast-expression
                   | postfix-expression ;

cast-expression    = "(" type ")" unary-expression ;

postfix-expression = primary-expression { postfix-suffix } ;

postfix-suffix     = "[" expression "]"
                   | "->" IDENTIFIER embedded-member-traversal
                   | "++"
                   | "--" ;

embedded-member-traversal
                   = { "." IDENTIFIER | "[" expression "]" } ;

primary-expression = INTEGER_LITERAL
                   | FLOAT_LITERAL
                   | STRING_LITERAL
                   | "true"
                   | "false"
                   | "NULL"
                   | "null"
                   | local-reference
                   | global-reference
                   | intv-reference
                   | function-call
                   | function-pointer-call
                   | syscall-call
                   | intrinsic-call
                   | "(" expression ")" ;
```

`embedded-member-traversal` is context-sensitive. After `->`, or after indexing
a pointer to a configured structure, the compiler may consume configured
embedded members using `.` and structure-array indices. General C-style `.` on
an arbitrary expression is not implemented.

### 9.1 Primary resolution order

An identifier primary is resolved in this order:

1. `true`/`false` and null constants;
2. executable function-pointer call;
3. local or parameter;
4. global;
5. configured INTV;
6. configured syscall;
7. hard-coded intrinsic;
8. previously declared internal function;
9. unknown-identifier error.

This order explains local-over-global shadowing and why declarations affect
whether a word is parsed as a call or variable.

### 9.2 Calls

```text
function-call = IDENTIFIER "(" [ argument-list ] ")" ;
```

Calls are not a general postfix operation: the callee must be a resolvable
identifier. Calling the result of an arbitrary expression is unsupported.

Ordinary internal calls reject extern targets. Function-pointer calls are
recognized before ordinary local-variable references when the local has the
function-pointer type and is followed by `(`.

Configured syscalls supply their parameter count and return category from
`SYSCALL_TABLE.txt`. The parser checks syscall argument count but does not have
per-parameter types in that table.

### 9.3 Intrinsic calls

The following expression-level calls are hard-wired:

```text
GetGlobal(integer-index)             -> int
SetGlobal(integer-index, integer)    -> void
GetIntv(integer-index)               -> int
SetIntv(integer-index, integer)      -> void
Delay(integer)                       -> void
Wait(string)                         -> void
IsRunning(string)                    -> int
```

They lower to the corresponding instructions documented in `magic_inst.txt`.

### 9.4 Arrays and pointer indexing

`[]` has two distinct implementations:

- on a declared local/global array, the result is an indirect script-slot
  access;
- on a non-void pointer, the index is scaled by the pointed-to type's byte size
  and the result is a memory lvalue.

Array bounds are not checked. A declared array name cannot be used directly
without an index.

Pointer element sizes are:

- byte/unsigned byte: 1;
- short/unsigned short: 2;
- int/unsigned/float and pointer values: 4;
- configured structures: the size from `STRUCT_TABLE.txt`.

Although direct dereference of `string*` has dedicated string memory handling,
the current pointer-index scaling code has no valid `string` element-size
branch. `stringPointer[index]` should therefore be treated as unsupported.

### 9.5 Structure traversal

For `pointer->member`, the left operand must be a configured structure pointer.
The member name and offset come from `STRUCT_TABLE.txt`.

Embedded structures and structure arrays are traversed as address arithmetic.
For example:

```text
pBattleUserInfo->pSoldierInfo[index].pObject
```

is conceptually lowered to:

```text
*(pBattleUserInfo
  + offset(pSoldierInfo)
  + index * sizeof(SoldierInfo)
  + offset(pObject))
```

The final value can be an lvalue unless it is a whole structure, which the VM
cannot store directly on the script stack.

### 9.6 Unary and increment/decrement behavior

- `!` produces an integer truth value.
- unary `-` accepts integer-like or float operands, not strings.
- `~` requires an integer-like operand.
- unary `*` requires a non-void, non-structure pointer and produces a memory
  lvalue.
- prefix `++`/`--` updates the lvalue immediately and yields the new value.
- postfix `++`/`--` yields the old value; its update is emitted after the main
  expression has been lowered.

Valid increment/decrement lvalues include locals, globals, INTVs, indexed
script arrays, and memory lvalues.

### 9.7 Binary type behavior

- integer-like operands include all integer widths and all pointers;
- arithmetic involving a float produces float, except `%`, which rejects
  floats;
- `+` concatenates two strings;
- comparisons accept two strings, two numeric values, or mixed integer/float
  numeric values;
- string/non-string mixed operations are rejected;
- bitwise, shift, and logical operators reject floats and strings;
- `&&` and `||` evaluate both operands before emitting the VM logical
  instruction.

### 9.8 Assignments and lvalues

The left side of an assignment must be one of:

- a local or parameter;
- a global;
- a configured INTV;
- an indexed local/global array element;
- a dereferenced memory pointer;
- a configured structure member reached through `->`/embedded traversal.

Assignment expressions yield the assigned value. Compound assignments first
load the old value, calculate the result, store it, and optionally repush it.

The parser currently recognizes all compound assignments listed in the table
except `%=`. Lowering support for `%=` exists but is unreachable from the
operator loop.

## 10. Scope and name resolution

### 10.1 Translation-unit scope

These collections span the entire compilation:

- internal and extern function names;
- global variable names;
- user and generated labels;
- string-table entries;
- configured syscall, INTV, and structure names.

Functions and globals become visible only after their declarations have been
parsed. Internal recursion works because the current function is registered
before parsing its body.

### 10.2 Function scope

Parameters and all locals share one function-wide name map. Blocks do not
introduce nested local scopes. A parameter can be assigned like a local.

### 10.3 Shadowing

A local or parameter shadows a global. Configured INTVs and calls are checked
only after local/global lookup, so an ordinary variable can also shadow those
names in expression-primary resolution.

## 11. Configuration-defined language surface

### 11.1 Syscalls

Each nonempty `SYSCALL_TABLE.txt` entry has the form:

```text
HEX_CODE PARAMETER_COUNT RETURN_TYPE NAME
```

Return type codes used by the compiler are principally:

- `0`: void;
- `1`: integer;
- `2`: float when present;
- `3`: string.

The table extends the expression-call namespace without changing the grammar.

### 11.2 INTV values

Each `INTV_TABLE.txt` entry maps a source identifier to a hexadecimal INTV
index:

```text
NAME HEX_INDEX
```

INTV references behave as integer lvalues.

### 11.3 Structures

A structure-table block has this shape:

```text
STRUCT_NAME SIZE
OFFSET MEMBER_NAME MEMBER_TYPE [ARRAY_LENGTH dup]
...
end
```

Offsets are hexadecimal. Size parsing accepts C-style base prefixes. Member
types use compiler type spelling, including configured structure names and
pointer stars. `N dup` marks a structure member as an array; bounds are not
enforced by the compiler.

## 12. Generated assembly model

The parser emits a Sango VM assembly file, not native machine code.
`magic_inst.txt` is the reference for instruction behavior.

### 12.1 Functions and stack slots

For a normal function the compiler emits:

- a function label;
- `STACK localCount + 2`;
- lowered body instructions;
- `RETN parameterCount` for an implicit void return;
- `RETVAL parameterCount` for an explicit value return.

Local slot numbers start at 3. Parameter numbers are negative and depend on the
parameter count. Global slots start at 1.

### 12.2 Strings

String literals and function names are deduplicated in a string table. Source
strings are unescaped and packed into `DD` values for the assembler. String
references use `PUSHSTR` with the assigned string index.

### 12.3 Magic table

Each non-extern function produces a magic-table procedure entry containing:

- function entry label;
- parameter count;
- function-name string entry;
- callsign.

Extern functions are omitted.

### 12.4 Labels and reduction

Compiler-generated labels normally have this form:

```text
l<sequence>_<description>
```

With `use_timed_label = 1`, they have this form:

```text
t<minutes-since-epoch>_<sequence>_<description>
```

When `reduce_redundant_labels = 1`, consecutive label-only declarations are
coalesced and all references to removed labels are rewritten to the first
label. This includes compiler-generated labels, source label statements, and
labels inside `__asm`; their provenance does not affect their assembly address.
Indented labels and labels followed only by an assembler comment are included,
while a line such as `MAGIC_0: procedure ...` is not label-only. Reduction is
required because the bundled assembler cannot accept sufficiently long runs of
consecutive labels. Alias references are rewritten only in assembly code;
quoted strings and assembler comments are preserved verbatim.

### 12.5 File trailer

The output contains author strings and a compilation timestamp. The timestamp
is nondeterministic and should be normalized in golden-output tests.

## 13. Recommended parser-generator architecture

A future generated parser should preserve the existing phases rather than put
all behavior into one grammar:

```text
source files
    -> include/define preprocessor
    -> lexer with opaque ASM_BLOCK token
    -> context-free parser
    -> symbol/type/configuration resolution
    -> existing or equivalent assembly lowering
    -> assembly-label reduction
```

An ANTLR grammar can cover the lexer/parser portion, with these qualifications:

- configured structure names should lex as `IDENTIFIER` and be classified by a
  semantic phase;
- declaration-versus-expression ambiguity depends on that type-name set;
- `__asm` is best produced by a dedicated lexer mode or by the existing opaque
  scanner;
- raw macro expansion belongs before ANTLR tokenization;
- symbol-order restrictions and lvalue/type checks belong in semantic passes;
- precedence must reproduce section 9 rather than use C's precedence table.

The generated grammar should initially be a conformance oracle, not a
replacement for production parsing. It should be tested against normalized
outputs from the current compiler before any migration is considered.

## 14. Confirmed implementation quirks and open specification issues

These items must be resolved deliberately in separate compatibility decisions.
They should not change incidentally during optimization.

### 14.1 Lexing and preprocessing

- `#define` is raw substring replacement and can alter comments, strings, and
  larger identifiers.
- source/include paths are relative to the process working directory.
- a `//` comment without a final newline is not matched by the line-comment
  regular expression.
- unterminated strings and assembly blocks lack robust end-of-file handling.
- `__asm` does not support nested braces as a structured language construct.
- file and token storage use fixed capacities without complete bounds checks.
- `1f` is accepted as a float even though the user guide says a decimal point
  is required.

### 14.2 Types and declarations

- the implemented `getType` branch order prevents ordinary use of
  `unsigned int`, `unsigned short`, and `unsigned byte`; bare `unsigned` works.
- configured structure values can enter parser paths even though whole
  structures cannot be represented normally on the VM stack.
- the `naked` marker can be consumed and ignored on a global declaration.
- pointer depth can carry across comma-separated global declarators that omit
  explicit stars.
- calls to later internal functions fail because there is no internal symbol
  predeclaration pass.
- labels are translation-unit-wide rather than function-scoped.

### 14.3 Statements

- empty statements and empty `for` clauses are unsupported.
- the `for` initializer invokes the full statement parser.
- `default` must be the last switch clause.
- case selectors use a primary constant object rather than a full constant
  expression; negative cases are not accepted.
- `break` or `continue` outside a matching context can access an empty target
  stack instead of producing a controlled diagnostic.
- non-void return checking observes the final parsed statement rather than all
  control-flow paths.

### 14.4 Expressions

- precedence and associativity differ from C as listed in section 9.
- `&&` and `||` always evaluate both sides.
- `%=` is tokenized and can be lowered, but the parser does not accept it.
- direct `string*` indexing falls through an invalid element-size path.
- all integer widths and pointers are broadly type-compatible.
- function calls are identifier-specific primaries rather than a general
  postfix call operator.
- direct `.` member access is limited to the compiler's embedded-structure
  traversal path.

### 14.5 Code generation and diagnostics

- some invalid constructs reach fatal internal paths instead of structured
  diagnostics.
- generated assembly includes a timestamp, so raw output is not reproducible.
- inline assembly replacement is textual and can rewrite comments.
- redundant-label reduction is semantically necessary for the bundled
  assembler but is currently implemented as repeated whole-output regex/search
  passes.

## 15. Minimal conformance examples

### 15.1 Representative valid program

```text
int globalValue;
int globalArray[4];

extern void ExternalEffect(int, string);

int AddOne(int value) {
    return value + 1;
}

void main() {
    int i = 0, values[4];
    string message = "hello";

    for (i = 0; i < 4; i++) {
        values[i] = AddOne(i);
    }

    if (values[0] != 0) {
        asynccall ExternalEffect(values[0], message);
    }

    return;
}
```

This example assumes `use_exe_memory_io`, configuration-provided names, and the
extern target are appropriate for the intended game environment.

### 15.2 Precedence examples

Under the current grammar:

```text
a || b && c
```

groups as:

```text
(a || b) && c
```

because `||` and `&&` share a left-associative level.

Similarly:

```text
a | b & c
```

groups as:

```text
(a | b) & c
```

Assignment is also left-associative in the parser; chained assignment should be
avoided even when a particular form passes lvalue checks.

### 15.3 Safe source-style subset

Until the open issues are resolved, new scripts should prefer:

- one declaration before every use of a function or global;
- brace-delimited function and control-flow bodies;
- bare `unsigned` rather than multiword unsigned types;
- no chained comparisons or assignments;
- explicit parentheses around mixed logical/bitwise operators;
- nonempty conventional `for` clauses;
- nonnegative literal switch cases and a last-position `default`;
- `break`/`continue` only inside the matching construct;
- `//` files ending with a newline;
- simple identifier-like macro names that do not occur as substrings elsewhere.

This subset does not redefine the language; it avoids the current parser's most
surprising or unsafe edges.
