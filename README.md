# Umbra Technical Assessment Scaffold

This repo is set up so you can write the solution yourself without spending time on boilerplate.

## Included

- `src/propulsion_server.cpp`: default C++ placeholder server entrypoint
- `tests/test_propulsion_server.cpp`: default C++ unit test skeleton
- `src/propulsion_server.c`: optional C version
- `tests/test_propulsion_server.c`: optional C test version
- `Makefile`: build, run, and test targets

## Commands

```bash
make build
make run
make test
make build-c
make test-c
```

## Suggested Approach

- Keep the solution small enough to discuss comfortably in an interview.
- Solve against standard input and standard output unless you have a strong reason to use TCP.
- Fill in the four behavior tests first: fire, overwrite, cancel, repeated use.
- The default targets now prefer C++.
- If you decide to stay in C, use `make build-c`, `make run-c`, and `make test-c`.

## Notes

- The test scaffold uses only the standard library and `assert`.
- The current placeholders intentionally fail until you replace them.
