# Introduction

Shell++ is a programming language that aims bring features from modern languages,
as facility to manipulate data structures, object oriented programming,
functional programming and others, to shell script.
https://alexst07.github.io/shell-plus-plus/

# Features
 * easy to manipulate data structure, as in Python
 * lambda functions
 * classes
 * executes commands like in Bash
 * pipe, redirection to or from files, sub shell
 * dynamic typing
 * glob and recursive glob
 * closures
 * reference counter
 * operator overriding

# Why another programming language?
I wanted a language that runs shell commands like Bash, and manipulate data structure with the ease of Python, I searched, but I did not find any that met my requirements, so I decided to use my free time to create one.

This is still in development, but it has already made life easier for me in many day-to-day tasks, maybe this might be useful for someone else, so I shared it on github.

However everything here can change without warning, because to date, the only purpose of it is MAKE MY LIFE EASIER AND ONLY THAT.

# Examples
Follows some examples just to get an idea of how the language works.

## Hello World
```shell
echo Hello World
print("Hello World")
```
In Shell++ you can use an other program as echo, or a native function print.

# List comprehension
```shell
files = [f for f in $(ls) if path(f).size("M") > 50]
```
In this example files will be an array with all files listed by ls command with size bigger than 50Mb.

## Reading file line by line
```python
for line in file("example.txt") {
  print("line: ", line)
}
```

## String operations
### Length
```python
str = "example"
print("length: ", len(str)) # length: 7
```


### Upper case and lower case
```php
x = "HELLO"
echo ${x} # HELLO
echo ${x.lower()} # hello
echo ${x.upper()} # HELLO
```

### Substring
```python
str = "string test"
str1 = str[2:] # ring test"
str2 = str[:-2] # string te
str3 = str[2:4] # ri
```

### Replace
```python
filename = "Path of the bash is /bin/bash"
print("After Replacement: ", filename.replace("bash", "sh"))
# After Replacement: Path of the sh is /bin/sh
print("length: ", len(str))
```

### Pass variables to command
```php
echo ${t} # Hello
echo ${t + " World"} # Concatenate string
# ${} solve an expression and return the value to echo
```

## Command examples
### Pipeline
```shell
echo "Test" | cat # simple pipeline
ls src* | grep -e "test" # using glob

# using variables content as command
cip = "ipconfig"
cgrep = ["grep", "-e", "10\..*"]
${cip} | ${cgrep} # pass an array to command
```

### Redirecting to and from file
```shell
cat < filename.txt
cat < filename.txt | grep test
cat < filename.txt | grep test > result.txt

# using variables
filename = "filename.txt"
result = "result.txt"
cat < ${filename} | grep test > ${result}
```
### Concatenate file

```shell
echo line1 > test.txt
echo lene2 >> test.txt
```
test.txt has this content:
line1
lene2

### Redirecting error output
```shell
prog_test 2> /dev/null
```
Any output on stderr go to /dev/null.

### Assign command to variable
```php
p = $(prog_test -opt 1)

# check result
if p {
  print(p)
} else {
  print("error: ", p.err())
}
```
Check the status result of the command prog_test, if prog_test
exited normally with status 0, the result is true and the line
print(p) is executed, if not, the else is executed.

### Iterating command result
```php
for f in $(ls -a mypath) {
  print("file: ", f)
}
```
Print all lines generated by ls command.

### Execute command from simple variabel
```shell
ex = "ls"
${ex} # execute ls commad
```

### Execute command from array variabel
```shell
scp = ["sch", "-i", "path/file.pem", "user@addr:path"]
${scp} # execute command from variable array
```
It is possible execute a command from an array, using this feature
you have much more flexibility to mount the command with its arguments
using arrays.

### Glob and recursive glob
```shell
file *.txt
```
Glob in this case works like in bash.

```php
for f in %%*.txt% {
  file ${f}
}
```
iterate over all txt files recursively

```shell
file ${%%*.txt%}
```
This peace of code has the same output from above

## Path
### Check if a path or file exists
```php
if path("/home/me/file.txt").exists() {
  print ("file exists")
}
```

### Comparing path
```php
p1 = path("/home/../home/me/file.txt")
p2 = path("/home/me/file.txt")

print(p1 == p2) # true

p1 = path("/home/me/file.txt")
p2 = path("/home/me/file2.txt")

print(p1 == p2) # false
```
In Shell++ you can compare relatives path, it means,
using path object you compare path not string.

## Array
In Shell++ is easy create and access array, it works like in Python

```python
array1 = ['physics', 'chemistry', 1997, 2000];
array2 = [1, 2, 3, 4, 5, 6, 7 ];

print "array1[0]: ", array1[0]
print "array2[1:5]: ", array2[1:5]
```
Output:
```
array1[0]:  physics
array2[1:5]:  [2, 3, 4, 5]
```

## Map
Map is a hash map object, it works like dictionary in Python.

```python
m = {'Name': 'Zara', 'Age': 7, 'Class': 'First'}

print("m['Name']: ", m['Name'])
print("m['Age']: ", m['Age'])
```
Output:
```
m['Name']:  Zara
m['Age']:  7
```

## Functions
In Shell++ function is not a command as in Bash, functions in Shell++ are similar with Python.
```go
func test(a, b = 5) {
  m = {"sum": a+b, "sub":a-b}
  return m
}

print(test(6)["sum"])
```
Output:
```
11
```

### Lambda

```go
func ftest(a) {
  v = ["echo", "ls", a]

  # closures
  return func(x) {
    return v.append(x)
  }
}
```

## Classes
```php
# Complex number
class Complex {
  func __init__(r, i) {
    this.r = r
    this.i = i
  }

  func __add__(n) {
    return Complex(n.r + this.r, n.i + this.i)
  }

  func __sub__(n) {
    return Complex(n.r - this.r, n.i - this.i)
  }

  func __print__() {
    return string(this.r) + " + " + string(this.i) + "i"
  }
}

c1 = Complex(2, 3)
c2 = Complex(1, 2)
c = c1 + c2

print(c)
```
Output:
```
3 + 5i
```

## Try-Catch
```php
try {
  git clone git@github.com:alexst07/shell-plus-plus.git
} catch InvalidCmdException as ex {
  print("git not installed [msg: ", ex, "]")
}
```
Shell++ has exception control, when a command is not found an InvalidCmdException is thrown.

## Quick-sort

```go
func quicksort(A, lo, hi) {
  if lo < hi {
    p = partition(A, lo, hi)
    A = quicksort(A, lo, p - 1)
    A = quicksort(A, p + 1, hi)
  }

  return A
}

func partition(A, lo, hi) {
  pivot = A[hi]
  i = lo
  j = lo

  for j in range(lo, hi) {
    if A[j] <= pivot {
      A[i], A[j] = A[j], A[i]
      i = i + 1
    }
  }

  A[i], A[hi] = A[hi], A[i]
  return i
}

a = [1,4,6,4,10,11,22,100,3,3,9]
print(quicksort(a, 0, len(a) - 1))
```
Output:
```
[1, 3, 3, 4, 4, 6, 9, 10, 11, 22, 100]
```

## Check if a command exists
If you need execute a no standard application, sometimes you need to check if the application is installed, with Shell it is a way.

```php
func exist_cmd(name) {
  c = $(whereis ${name})

  arr = string(c).trim().split(":")

  if arr[1] == "" {
    return false
  }

  return true
}
```
the command whereis inside $() have as result an object cmd, you can convert its result to string and the handle it.
# Building

## Requirements:
  * A compiler that supports C++ 14 (gcc or clang)
  * Boost
  * Readline
  * CMake
  * Linux

## Compiling

### Ubuntu
First you need intall the requirements to build Shell++
```
# apt-get install -y build-essential
# apt-get install -y libboost-all-dev libreadline6 libreadline6-dev git cmake
```

With the requirements installed, make a clone of the repository
```
$ git clone git@github.com:alexst07/shell-plus-plus.git
```
After the clone, change to directory of the project
```
$ cd shell-plus-plus
```
To compile the project, you need create a directory when the cmake files will
be generated, and so, build the project

```
$ mkdir build
$ cd build
$ cmake ..
$ make
```
Now you can already use the Shell++, the binary is generated at shell/ directory.
To use Shell++ to execute a script, first, create an example script called my.shpp.

```
echo my first script in shell++
```
In the build path, call the binary to execute your script

```
$ ./shell/shpp my.shpp
```
You should see the output:
```
my first script in shell++
```
If you saw this output your build is working, if you want install Shell++.

```
$ sudo make install
```
