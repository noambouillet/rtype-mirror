## Commit messages

Commits follow a simplified [Conventional Commits](https://www.conventionalcommits.org/)
format. A `commit-msg` Git hook rejects any commit that does not match.

```
<type>(<optional scope>): <description>

<optional body>
```

### Types

| Type       | Use for                                             |
|------------|-----------------------------------------------------|
| `feat`     | A new feature                                       |
| `fix`      | A bug fix                                           |
| `docs`     | Documentation only                                  |
| `style`    | Formatting, no code change (clang-format, etc.)     |
| `refactor` | Code change that neither fixes a bug nor adds a feature |
| `perf`     | Performance improvement                             |
| `test`     | Adding or fixing tests                              |
| `build`    | Build system or dependencies (CMake, package manager) |
| `ci`       | CI/CD configuration                                 |
| `revert`   | Reverting a previous commit                         |
| `update`   | Updating existing content or data (assets, configs) |
| `del`      | Removing files or code                              |
| `merge`    | Manual merge commits                                |

### Rules

- The first line must match `type(scope): description`. The scope is optional
  and names the subsystem (`network`, `render`, `ecs`, `protocol`...).
- Keep the first line short (about 72 characters), in the imperative mood:
  "add", not "added".
- Use the body (after a blank line) to explain **why**, not what.
- There are no exceptions: merge and revert commits must also follow the
  format (e.g. `merge: feature X into main`, e.g. `revert: feat(network): add ACK`).

### Examples

```
feat(network): add UDP packet acknowledgement
fix(render): stop starfield scrolling depending on frame rate
docs: add protocol specification
build(cmake): pin SFML version in vcpkg manifest
```

### Enabling the hook

After cloning, run once:

```sh
git config core.hooksPath .githooks
```

The hook file must be named `commit-msg`, be executable
(`chmod +x .githooks/commit-msg`), and requires Python 3.
