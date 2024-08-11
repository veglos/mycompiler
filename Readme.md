# Let's build a compiler

https://www.youtube.com/watch?v=vcSijrRsrY0&t=1380s

## Requirements

1. Install NASM (NASM version 2.14)

## Run

1. run nasm in order to create the object file

```sh
nasm -felf64 test.asm
```

2. link it with GNU liner in order to create the executable file

```sh
ld test.o -o test
```

3. run the executable file
```sh
./test
```


## Documentation

### Linux System Call Table
https://chromium.googlesource.com/chromiumos/docs/+/master/constants/syscalls.md
