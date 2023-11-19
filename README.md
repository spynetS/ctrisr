## CTRISR

## Discription
Ctrisr is a Tetris clone written in c with a terminal renderer library
i made, [the library](https://github.com/spynetS/printer)

The printer library is only written with libc.
I move the cursor with terminal prints like 
```c
    printf("\033[%d;%dH", y, x);
```

## Mechanics

* A preview of where the shape is going to land
* You move sideways with AD, rotate with W and drop on SPACE
> I have not figured out how to use the arrow keys so this is a improvment potential
* With the E key you can hold or get the holded shape.
* The next 3 shapes are showed.

## Install

### Source
Git clone and then run ./run.sh. That simple script will
just build it and output to a.out

### Aur
There is a package on aur [here](https://aur.archlinux.org/packages/ctrisr-git)
>to install
```
yay -S ctrisr-git
```
> then run
```
ctrisr
```



