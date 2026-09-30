# Coding Style

Formatting and comment conventions are based on the
[Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html).
Our naming convention differs from Google's (see [Naming](#naming)).
Commit and Git workflow rules are in [contributing.md](contributing.md).

## Naming

| Element                                                 | Convention             | Example         |
|---------------------------------------------------------|------------------------|-----------------|
| Classes, structs, enums                                 | PascalCase             | `EntityManager` |
| Enum values                                             | PascalCase             | `Direction::Up` |
| Functions (including `constexpr` functions)             | camelCase              | `spawnEnemy()`  |
| Local variables, parameters                             | camelCase              | `playerCount`   |
| Public members                                          | camelCase, no prefix   | `position`      |
| Private/protected members                               | `_` prefix + camelCase | `_health`       |
| Constants (`constexpr`, global `const`, `static const`) | UPPER_CASE             | `MAX_PLAYERS`   |
| Namespaces                                              | lowercase              | `network`       |
| Macros (avoid them)                                     | UPPER_CASE, `RTYPE_` prefix | `RTYPE_ASSERT` |

Notes:

- The `_` prefix tells the reader at a glance that a variable is a class
  member and not a local variable or parameter.
- Never use `_` followed by an uppercase letter (`_Health`) or a double
  underscore (`__health`): these names are reserved by the C++ standard.
- Plain data holders are a `struct` with public members and no prefix.
  Anything with invariants is a `class` with private members.
- Anything declared `constexpr` is a constant, even inside a function
  (`constexpr int MAX_SPEED = 5;`).
- A plain `const` local computed at runtime is NOT a constant: it stays
  camelCase (`const int remaining = health - amount;`).
- Prefer `enum class`. Values are always accessed with their scope
  (`Direction::Up`), so they use PascalCase.
- clang-tidy calls PascalCase `CamelCase`. Both mean the same thing.

```cpp
enum class Direction { Up, Down, Left, Right };

constexpr int MAX_PLAYERS = 4;

class Player {
 public:
  explicit Player(int health);
  void takeDamage(int amount);

  float speed;

 private:
  static constexpr int MAX_HEALTH = 100;
  int _health;
};

Player::Player(int health) : speed(1.0f), _health(health)
{
}

void Player::takeDamage(int amount)
{
  const int remaining = _health - amount;
  _health = remaining;
}
```

These rules are enforced by clang-tidy (see [Tooling](#tooling)).

## Language rules

- Target C++20.
- Use `#pragma once` in headers. Put all code in a namespace.
- No raw `new` / `delete`. Use `std::make_unique`. Use `std::shared_ptr` only
  when ownership is truly shared.
- Use `const` wherever possible. Mark overrides with `override`. Use `nullptr`,
  never `NULL` or `0`.
- Prefer return values over output parameters. Pass large read-only inputs by
  `const&`.
- Never trust network input: check sizes and ranges before reading or
  allocating. Malformed packets must never crash the client or server.
- Keep functions short and focused. Prefer composition over inheritance.
- Include what you use. Include order is handled by clang-format.

## Comments

| Syntax | Meaning               | Read by Doxygen / IDE hover? |
|--------|-----------------------|------------------------------|
| `///`  | Documentation comment | Yes                          |
| `//`   | Normal comment        | No                           |

- Use `///` **above function definitions in `.cpp` files** to describe what
  each function is for and how to use it. See `SKILLS/coding-style.md` for
  project-specific placement rules.
- Use `//` **inside function bodies** for **how or why** notes (tricky logic,
  workarounds, non-obvious choices).
- Don't state the obvious in either kind.

### Function documentation

Almost every function gets a `///` comment directly above its definition.
Skip it only for simple, obvious functions (trivial accessors, etc.).

- The first sentence is the summary. It starts with a verb phrase, with "This
  function" as the implied subject: "Opens the file", not "Open the file".
- Add a short paragraph only for behavior that is not obvious (edge cases,
  side effects, ownership, complexity).
- Describe **use**, not implementation. Use complete sentences.

Mention only when not obvious:

- Meaning of inputs and outputs (units, ranges, preconditions). Wrap argument
  names in `backticks` so tools can index them.
- Whether a pointer/reference may be null, and what happens if it is.
- Whether the object keeps a reference/pointer after the call returns.
- Whether an output argument is appended to or overwritten.
- Performance implications and thread-safety.

Not needed:

- `@param` / `@return` for things the signature already says. Use a tag only
  when it adds information the signature can't (units, lifetime, sentinel
  values such as "-1 if not found").
- Comments on constructors/destructors, unless they do something non-trivial
  (e.g. take ownership).
- Overloads: one shared comment before the first overload.
- Overrides: only document what differs from the base function.

```cpp
/// Returns an iterator positioned at the first entry lexically greater than
/// or equal to `startWord`. Returns a null pointer if there is no such entry.
/// The iterator must not outlive the table.
std::unique_ptr<Iterator> Table::getIterator(std::string_view startWord) const
{
  return findIterator(startWord);
}

/// Resamples the audio buffer to a new sample rate.
///
/// @param ratio Output rate / input rate (e.g. 0.5 halves the rate).
/// @param buf   Modified in place; must not be empty.
void resample(AudioBuffer& buf, double ratio)
{
  buf.resample(ratio);
}
```

## Tooling

Style is enforced by tools, not by review comments. Two config files at the
repository root define the rules. Install `clang-format` and `clang-tidy`
(LLVM 16 or newer recommended).

### `.clang-format`

```yaml
BasedOnStyle: Google
ColumnLimit: 0
IndentWidth: 4
TabWidth: 4
UseTab: ForContinuationAndIndentation
AccessModifierOffset: -4
AlignAfterOpenBracket: DontAlign
FixNamespaceComments: false
BreakBeforeBraces: Custom
BraceWrapping:
  AfterFunction: true
AllowShortFunctionsOnASingleLine: None
```

We use the Google preset without automatic line wrapping. Source indentation
uses four-column tabs for indentation. Access labels align with the class
declaration, and class and struct members have one tab of indentation.
Wrapped parameter lines use one continuation tab instead of aligning under
the opening parenthesis.
Clang-format does not add namespace closing comments and places each function
definition's opening brace on the next line. This includes short and inline
functions; control statements keep their Google-style braces. Do not commit
the output of
`clang-format -dump-config`: it pins every option and breaks on other
clang-format versions.

### `.clang-tidy`

```yaml
Checks: >
  -*,
  clang-analyzer-*,
  bugprone-*,
  modernize-*,
  performance-*,
  readability-*,
  -modernize-use-trailing-return-type,
  -readability-magic-numbers,
  -readability-identifier-length
WarningsAsErrors: '*'
HeaderFilterRegex: '^(.*/)?(src|include)/.*'
FormatStyle: file
CheckOptions:
  # Types
  - { key: readability-identifier-naming.ClassCase,               value: CamelCase }
  - { key: readability-identifier-naming.StructCase,              value: CamelCase }
  - { key: readability-identifier-naming.EnumCase,                value: CamelCase }
  - { key: readability-identifier-naming.EnumConstantCase,        value: CamelCase }
  - { key: readability-identifier-naming.NamespaceCase,           value: lower_case }

  # Functions and variables
  - { key: readability-identifier-naming.FunctionCase,            value: camelBack }
  - { key: readability-identifier-naming.VariableCase,            value: camelBack }
  - { key: readability-identifier-naming.ParameterCase,           value: camelBack }
  - { key: readability-identifier-naming.LocalConstantCase,       value: camelBack }

  # Class members
  - { key: readability-identifier-naming.PublicMemberCase,        value: camelBack }
  - { key: readability-identifier-naming.PrivateMemberCase,       value: camelBack }
  - { key: readability-identifier-naming.PrivateMemberPrefix,     value: _ }
  - { key: readability-identifier-naming.ProtectedMemberCase,     value: camelBack }
  - { key: readability-identifier-naming.ProtectedMemberPrefix,   value: _ }

  # Constants and macros
  - { key: readability-identifier-naming.ConstantCase,            value: UPPER_CASE }
  - { key: readability-identifier-naming.ConstexprVariableCase,   value: UPPER_CASE }
  - { key: readability-identifier-naming.ClassConstantCase,       value: UPPER_CASE }
  - { key: readability-identifier-naming.GlobalConstantCase,      value: UPPER_CASE }
  - { key: readability-identifier-naming.StaticConstantCase,      value: UPPER_CASE }
  - { key: readability-identifier-naming.MacroDefinitionCase,     value: UPPER_CASE }
```

- `HeaderFilterRegex` limits warnings to our own headers. Make sure
  third-party sources (fetched by the package manager) are not under a folder
  matching it.
- `FormatStyle: file` makes `--fix` produce code that respects `.clang-format`.
- `WarningsAsErrors: '*'` makes clang-tidy fail when it finds a warning.

### Commands

Run from the repository root.

```sh
# Format one file in place
clang-format -i src/foo.cpp

# Format the whole project
git ls-files -z '*.cpp' '*.hpp' | xargs -0 -r clang-format -i

# Check only, change nothing (what CI runs)
git ls-files -z '*.cpp' '*.hpp' | xargs -0 -r clang-format --dry-run --Werror
```

Static analysis needs a build directory containing `compile_commands.json`.
CI skips this step until `src` contains a `.cpp` file; after that, the project
must provide a root `CMakeLists.txt`.

```sh
# 1. Configure once, so clang-tidy knows the real build flags
cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# 2. Analyze one file
clang-tidy -p build src/foo.cpp

# 3. Analyze and apply automatic fixes
clang-tidy -p build --fix src/foo.cpp

# 4. Analyze the whole project (anchored to our sources, not dependencies)
run-clang-tidy -p build -quiet "$PWD/src/.*\.cpp"
```

### Git hooks

Hooks live in `.githooks/`. Enable them once after cloning:

```sh
git config core.hooksPath .githooks
```

- `pre-commit`: refuses the commit if staged C++ files are not formatted.
- `commit-msg`: checks the commit message format (see
  [contributing.md](contributing.md)).

Both files must be executable (`chmod +x .githooks/*`). The `commit-msg` hook
requires Python 3.
