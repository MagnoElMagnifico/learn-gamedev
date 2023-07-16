# learn-gamedev

Learning GameDev from scratch through several dummy projects. Each one is in its
own directory, sorted by creation date.

# Running

You may need CMake and install some dependencies (`libsfml-dev`, [SFML]).

```sh
cmake . -B build
cmake --build build
# or
cmake --build build --target <name>
```

Where `<name>` is the folder name of the project without the number (for
example, `000-test` is just `test`).

[SFML]: https://www.sfml-dev.org

