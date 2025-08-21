```
                            _ 
  __ _ _ __ ___   __ _ _ __| |
 / _` | '_ ` _ \ / _` | '__| |
| (_| | | | | | | (_| | |  | |
 \__, |_| |_| |_|\__, |_|  |_|
 |___/           |___/        
 
 g   a   m   e   g   i   r   l
```

# gmgrl
gmgrl (Game Girl) is a gameboy emulator written in C

## purpose
You might be thinking that there are plenty of gameboy emulators out there, and you're right.
But this is a learning project for me, both to experiment with creating a C project in my own style, but also
to learn about emulators in general. This is an area I've had a lot of interest in, and figured this was a good
place to jump in the deep end. What better way to learn than to write one for my favorite system.

I'm also hoping to keep this project clean by writing well document code, and commits. Something
I've traditionally been bad at in my personal projects. Often I end up keeping them private simply
due to the sporadic nature of my commits, and embarassement of the code itself as I am learning a new subject.
(WIP - hacks, barely working, commiting just to save progress... we've all been there)

So, hopefully this project will be a good learning experience for me, and let's see where it goes.

## tech
gmgrl is written in C, and (at least hopefully) will have 0 dependencies outside of a C compiler.

## build 
use the meta build program to build gmgrl
- first ensure the output directory exists
    - `mkdir bin`
- compile the build program
    - `cc build.c -o bin/build`
- run the build program
    - `./bin/build`

*you can simply use the following command*
- `mkdir bin ; cc build.c -o bin/build && ./bin/build`

## run
run gmgrl
- `./bin/gmgrl`

if you aren't making changes to the build program you can also run the build program with the `--run` flag
*for most development, this will be the canonical way to run the build program once it's been built*
- `./bin/build --run`
