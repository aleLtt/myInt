# myInt
*PURPOSE & INFO*:

This repository contains a very easy to use C++ code for arbitrary precision positive integer numbers. The purpose is to create provide a code that allows the user to easily manage integers number represented by an arbitrary number of bits. 
The code is thought to make it very easy to use in project where very large integer numbers are needed, but can still work for low numbers as well. It will be sufficient to download the single file and include it in your main() file, without the need to download and setup an entire library.

The code is not completely optimized, meaning that it has been thought and to designed to be optimized as well as possible, but no further optimization post-processing has been applied. However, operations by using myInt object ads a non-significant overhead with respect to operations performed with the standard C library. The only exception is for the multiplication operation, which may add a different overhead depending on the size of the two multiplying factors. (e.g. multiplication between two 128-bit numbers required t < 0.3s of time).

HOW TO USE IT:

The code is just a single C++ class where a myInt object is instantiated and can be handled quite like as a standard integer number. The class overwrites than all the standard operators (arithmetical, logical, bitwise and shift) so that it will be possible to perform common number operations as if you were manipulating standard integer variables.
To make a very simple example, in order to make a sum/and/xor/shift between two myInt numbers A and B, it will be sufficient to type:

myInt* C= *A + *B;
bool C= *A && *B;
myInt* C= *A ^ *B;
myInt* C= *A << 2;

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




