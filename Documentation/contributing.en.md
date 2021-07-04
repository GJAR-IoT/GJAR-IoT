# Contributing to GJAR IoT

*(Prerequisite: working with Git, GitHub - in need, see the [docs](https://git-scm.com/book/en/v2).)*

*This guide is about contributing code and other resources, for contributing to the docs, see [How to Contribute to the Documentation](how-to-docs.md).*

0. Fork the relevant repository to which you want to contribute (one of [GJAR-IoT](https://github.com/GJAR-IoT/GJAR-IoT), [GJAR-IoT-Backend](https://github.com/GJAR-IoT/GJAR-IoT-Backend), [GJAR-IoT-Frontend](https://github.com/GJAR-IoT/GJAR-IoT-Frontend), [GJAR-IoT-Node](https://github.com/GJAR-IoT/GJAR-IoT-Node)). It is unlikely you need to fork GJAR-IoT itself, unless you need to manually run changelog generation or make changes to general documentation.
1. (Optional, but recommended.) Create a new branch for your changes, with a sensible and descriptive name (use the name of the feature in the branch name).
2. Make the changes, write the code, commit as appropriate.
3. Push the changes to your fork.
4. Create a Pull Request (PR) containing an (optional, but recommended) summary and purpose of the changes, and a **required** changelog entry in the format below.

## PR Format

At the top of a PR, there should be a general description of changes and their purpose. If accepting the changes closes any issues, they should be referenced using the phrase `Fixes #<number of issue>, #<another>`. This can be omitted if it add no information contained below.

Following that is a **required** changelog entry in the format:

```md
Changelog:
+ added features
+ starting with a plus sign (and space)
- removed stuff with a minus
* changes, tweaks, fixes with an asterisk
```

Remember this will be included in the changelog, so the items must be clear, unambiguous, descriptive, and consise. If unsure whether an item is an addition or a removal, asterisk ('change') is likely the most appropriate type.
