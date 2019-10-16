# ATREM (Another Tiny Regular Expression Matcher)

## What is it all about?
This project was started for the sake of practicing in the C programming language. As a starting point I chose [this](https://www.cs.princeton.edu/courses/archive/spr09/cos333/beautiful.html) article by Brian Kernighan, but except that, there are lots of other resources that will be used and, of cource, cited here. This is supposed to be a library when it's released (actually, sounds like a joke to me, considering current state of the project).

## Current state
Right now the repository comprised only of the functionality that is present in the [original article](https://www.cs.princeton.edu/courses/archive/spr09/cos333/beautiful.html) and some [tests](#tests).

## Plans 
I plan this library to be compliant with IEEE POSIX ERE (Extended Regular Expression) standard.
My current goal is to include all the necessary [tests](#tests) for the functionality that is avaiable in the [original article](https://www.cs.princeton.edu/courses/archive/spr09/cos333/beautiful.html). After that, I will move on to implementing the rest of the planned functionality with consequent addition of tests.

## Tests
For now, the tests are divided into 2 groups: functionality tests and unit tests. The former ones are already written and reside in the 'tests' directory. Their design is completely made up by me and they are totally bloated with unnecessary garbage, so don't even bother with them. The latter ones are WIP, and they will certainly differ from the former ones. Their design is inspired by the [Eric Radmand's article](http://eradman.com/posts/tdd-in-c.html) on test-driven development in C.
