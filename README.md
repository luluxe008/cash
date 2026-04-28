# Cash, a bash-not-so-like shell
A shell written in C like no one ever did. For now it's really basic and it's just for fun/education/training.

I have high hopes for it.

# Building
Clone the repo `git clone https://github.com/luluxe008/cash` and build it with `make`

# Usage
it can run executable BUT, it does not replace variable like `$PATH`.

# known issues
- For now, `argv[0]` for the called program may be incorrect.
- I assume malloc, calloc, realloc and other allocating function never fail.


# License
See LICENSE.

## More notes
I try to make it good, so i check with valgrind.