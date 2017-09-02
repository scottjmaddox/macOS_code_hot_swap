# macOS code hot swap

This is a minimal C99 implementation of code hot swapping (a.k.a. code hot reloading) for macOS (a.k.a. Mac OS X, a.k.a. darwin). This basic approach can be used to enable live coding in many types of applications include video games, GUI applications, and web servers.

The basic idea is to split all of your application logic into separate *initialize* and *main loop* functions, and then to compile those functions into a dynamic library that can be hot swapped by a static main executable stub. Any state that should persist between hot swaps is stored inside a single massive state struct, which is allocated and initialized by `init` and then repeatedly passed to the `main_loop` function, where all of the application logic should reside.

To build and test this project, open two terminals in the root project directory. In the first terminal, run `make && cd build && ./main`. Then, after a few seconds, run `touch src/hot_main.c && make` in the second terminal. To stop running, in the first terminal, hold the control key and press the C button (i.e. press `Ctrl+C`). You should see something like this:

First terminal:

```sh
$ make && cd build && ./main
mkdir -p build
cc -g -std=c99 -Wall -Wextra -Wno-unused-function -Wno-unused-variable   \
		-dynamiclib -current_version 1.0 -compatibility_version 1.0 \
		-o build/hot_main.dylib \
		src/hot_main.c
cc -g -std=c99 -Wall -Wextra -Wno-unused-function -Wno-unused-variable   \
		-o build/main \
		src/main.c
x: 1, y: 1
x: 2, y: 2
Attempting hot swap.
No changes to swap.
x: 3, y: 3
x: 4, y: 4
Attempting hot swap.
No changes to swap.
x: 5, y: 5
x: 6, y: 6
Attempting hot swap.
Attempting to load changed module.
Hot swap complete.
x: 7, y: 1
x: 8, y: 2
Attempting hot swap.
No changes to swap.
x: 9, y: 3
^C
```

Second terminal:

```sh
$ touch src/hot_main.c && make
cc -g -std=c99 -Wall -Wextra -Wno-unused-function -Wno-unused-variable   \
		-dynamiclib -current_version 1.0 -compatibility_version 1.0 \
		-o build/hot_main.dylib \
		src/hot_main.c
```

Note that the value of `x` persists across the hot swap, while the value of `y` resets. This is because `x` is stored in the persistent state struct, while `y` is a static variable inside the dynamic library and is initialized to zero each time the dynamic library is loaded.

Also note that a hot swap is only attempted when the main loop returns a non-zero value for `hot_swap`. This can be used to periodically attempt a hot swap, or to create a keyboard hotkey that triggers an attempt, for example.

# Porting

This has not been tested on linux, but it should be trivial to port (if it doesn't already work). A similar approach will also work for Windows, though the function calls needed to load a dynamic library would be different.

# License

Copyright (c) 2017 by Scott J Maddox

All source code in this project is dedicated to the public domain under the terms of the (Creative Commons *CC0 1.0 Universal* license)[https://creativecommons.org/publicdomain/zero/1.0/].