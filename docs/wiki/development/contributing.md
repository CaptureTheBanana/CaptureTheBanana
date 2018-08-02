# Contributing

Thank you for your interest in contributing to CaptureTheBanana++. We welcome your feedback and contributions.

- Raise an issue
- See open issues

## Contributor Covenant Code of Conduct

See [Contributor Covenant Code of Conduct](https://github.com/CaptureTheBanana/CaptureTheBanana/CODE_OF_CONDUCT.md)

## Code style

The code style is based on the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html). Please run `scripts/run-clang-format.sh` on your files before committing.

Exceptions:

- 4 space indent
- Exceptions are allowed
- Member variables should be prefixed with `m_`.
- Use `//` for comments and `///` for doxygen comments

Include order. Please separate with one empty line:

- System headers
- CaptureTheBanana and third party headers
- Local/target headers

E.g: `engine/scene/Player.hpp`

```
#include <string>

#include <SDL.h>
#include <common/Utils.hpp>

#include "engine/scene/Player.hpp"
#include "engine/utils/Vector2d.hpp"
```

## Workflow

1. See [build instructions](build-instructions.md)
2. Fork this repository and clone it
  - Add the remote: `git remote add upstream https://github.com/CaptureTheBanana/CaptureTheBanana.git`
3. Create your feature branch: `git checkout -b feature`
4. Make modifications and push your branch
  - To format your code, run `scripts/run-clang-format.sh`
5. Rebase your branch if necessary
  - `git fetch upstream`
  - `git checkout master`
  - `git rebase upstream/master`
  - `git checkout feature`
  - `git rebase master`
  - Resolve all conflicts
  - `git push -f origin feature`
6. Create a [pull reques](https://help.github.com/articles/about-pull-requests/) against `master` branch
7. We do have the policy that one logical feature should be one commit. If this PR is read to merge and you have done more then one commit per feature, please squash your commits together. For this check, how many commits you have done and execute `git rebase -i HEAD~<number of commits>`:
  - Select `s` for sqaush for every commit except the first one. Save and exit.
  - Edit all commit messages. Save and exit.

Pull requests are automatically tested and may need correction.
