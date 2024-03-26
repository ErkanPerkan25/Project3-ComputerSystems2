# Project3-ComputerSystems2

implementing a programming language memory manager, implement a garbage collection system

Author: Eric Hansson

- What platform you developed your solution on (i.e Linux, OS/X, Windows (God forbid!), ...) <br>
Arch Linux

- any special details on how to compile your project <br>
Used a Makefile which creates a executable called `exe`, this is done by running this command:
```
make
```
- any special details on how to run your project <br>
To run the program you need to give the program a input file:
```
./exe 
```
When run it will as you for a heap size and input file.

- any known bugs your project has, and possible fixes to those bugs <br>
The program has a bug when it comes to the references. It does not go by the right allocated memory block. To fix this, I would have to re-implement the way I
decied to create the list of block pointers. It did make it a bit messy, but got most of it working.

- an overview of how you solved the project. <br>
For each step I took in this project I would hand-trace every detail to get the hang of what is going on in the program. And then I would slowly start to 
implement piece by piece. I would test each little piece to see if it would work, and then move on if it did.
