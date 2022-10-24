# myInt
**PURPOSE & INFO**: 
This repository contains a very easy to use C++ code for arbitrary precision positive integer numbers. The purpose is to create provide a code that allows the user to easily manage integers number represented by an arbitrary number of bits. 
The code is thought to make it very easy to use in project where very large integer numbers are needed, but can still work for low numbers as well. It will be sufficient to download the single file and include it in your main() file, without the need to download and setup an entire library.

The code is not completely optimized, meaning that it has been thought and to designed to be optimized as well as possible, but no further optimization post-processing has been applied. However, operations by using myInt object ads a non-significant overhead with respect to operations performed with the standard C library. The only exception is for the multiplication operation, which may add a different overhead depending on the size of the two multiplying factors. (e.g. multiplication between two 128-bit numbers required t < 0.3s of time).

**HOW TO USE IT**:
The code is just a single C++ class where a myInt object is instantiated and can be handled quite like as a standard integer number. The class overwrites than all the standard operators (arithmetical, logical, bitwise and shift) so that it will be possible to perform common number operations as if you were manipulating standard integer variables.
To make a very simple example, in order to make a sum/and/xor/shift between two myInt numbers A and B, it will be sufficient to type:

myInt* C= *A + *B;\n
bool C= *A && *B;\n
myInt* C= *A ^ *B;\n
myInt* C= *A << 2;\n

or

*A += *B;
*A &= *B;
*A ^= *B;
*A <<= 2;

Not only standard operations, myInt objects are also provided with functions that enables to used addtional features to allow users to easily manipulate the object itslef. As for example, with a myInt object it is possible to:
- perform a modulo sum
- circular shift
- create a new object by extracting an arbitrary block of bits from the considered object
- parsing an object into a standard-defined interger value
- learn the number of bits the number is represented by

**WORKING PRINCIPLE**: A myInt number is nothing than a vector of *unsigned short* variables. The number 65178 would then be represented according to the following logic:
*vector <unsigned short> a(5)* where a[0]= 6, a[1]= 5; a[2]= 1, and so on.
  
The code takes care of possible overflows/underflows, as well as the manage of the number of *unsigned short* needed to represent the given number and increasing or decreasing them when necessary.
The choice of using a vector of short values is due to the aim of using less storage memory as possible.
  
**NOTE**:
1) The code assumes to work just with positive integers.
2) I suggest to create myInt objects only with the *new* operator, so to allocate the space into the heap and access to the objectonly through pointer. Not doing that may create memory corruption when creating, deleting or manipulating myInt objects inside a function. No memory related issue has been reported if accessing through pointers.
3) The code is endianess independend, meaning that at the object initialization time it also takes care of the endianess of the machine for the correct allocation and manging of variables.
4) Some functions do not work properly for some extreme conditions. The cases known so far are reported inside the code as comments, but for most of the applications and cases no issue has been reported so far. The code is still under development, known bus will be fixed as sson as possible.
