# VByte
Compression algorithm to store integers.

# Function:
This algorithm takes a string of integers from an input file and compresses them to a continuous string of bytes.
The first bit of every byte denotes if this byte is the last byte of the current number.
This algorithm also includes an improved location query system, to solve the problem of finding numbers by index faster.

# Why VByte works for compression
When storing integers by them selves they reserve a certain amount of bytes. When sending these integers they are set to be the same size.
With small numbers this leads to alot of useless bytes with all 0:s on it. With VByte we only include the important bytes in to the encoded data.
