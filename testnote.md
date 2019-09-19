# Test Note
## How zero-padding in this code works?

### 2d conv
zero padding is calculated in this way
```C
if (opt.p) {
       opt.Pw = (opt.w - 1) * 2; 
       opt.Ph = (opt.h - 1) * 2;
    } else {
       opt.Pw = 0; 
       opt.Ph = 0;
    }
```
But it is not keeping the input and the output the same deminsion or at least similar deminsions.

Also, it does not consider the influences of different stride.

With the stride, the output deminsion should be:
${{H - kh + Ph + Sh}\over{Sh}}\times{{W - kw + Pw + Sw}\over{Sw}}$
If we want to maintain the size of the input, we need to set 
```C
if (opt.p) {
       opt.Pw = opt.Wx * opt.Sw + opt.w - opt.Wx - opt.Sw; 
       opt.Ph = opt.Hx * opt.Sh + opt.h - opt.Hx - opt.Sh; 
    } else {
       opt.Pw = 0; 
       opt.Ph = 0;
    }
```

Compare with the old code
```C
(base) wirelessprv-10-194-151-13:Convolutional_N jinboci$ ./test -x 5 -y 5 -kw 3 -kh 3 -vs 1 -hs 1 -p -o -k 1 -m 2
get_options
Options:
	Number of iterations: 	-c 1
	Image size: 	-x 5	-y 5
	Kernel size: 	-kw 3	-kh 3
	Striding: 	-vs 1	-hs 1
	Padding: 	-vp 4	-hp 4
	Kernel Type: 		-k 1 - 2D
	Implementation style:	-m 2 - external 0-padding
	Output: 		-o 1 - enabled

run_test 
opt.Wx 5
opt.w 3
opt.Pw 4
opt.Ph 4
Input matrix: 
0 1 2 3 4 
5 6 7 8 9 
10 11 12 13 14 
15 16 17 18 19 
20 21 22 23 24 

Kernel: 
0 1 2 
3 4 5 
6 7 8 

Output matrix: 
0 8 23 44 65 46 24 
40 88 142 175 208 136 66 
105 210 312 348 384 240 111 
180 345 492 528 564 345 156 
255 480 672 708 744 450 201 
130 232 304 319 334 184 72 
40 62 65 68 71 24 0 

Number of floating-point operations: 441
Elapsed time: 4e-06
FLOPS: 1.1025e+08

```
The current output
```C
(base) wirelessprv-10-194-151-13:Convolutional_N jinboci$ ./test -x 5 -y 5 -kw 3 -kh 3 -vs 1 -hs 1 -p -o -k 1 -m 2
get_options
Options:
	Number of iterations: 	-c 1
	Image size: 	-x 5	-y 5
	Kernel size: 	-kw 3	-kh 3
	Striding: 	-vs 1	-hs 1
	Padding: 	-vp 2	-hp 2
	Kernel Type: 		-k 1 - 2D
	Implementation style:	-m 2 - external 0-padding
	Output: 		-o 1 - enabled

run_test 
opt.Wx 5
opt.w 3
opt.Pw 2
opt.Ph 2
Input matrix: 
0 1 2 3 4 
5 6 7 8 9 
10 11 12 13 14 
15 16 17 18 19 
20 21 22 23 24 

Kernel: 
0 1 2 
3 4 5 
6 7 8 

Output matrix: 
88 142 175 208 136 
210 312 348 384 240 
345 492 528 564 345 
480 672 708 744 450 
232 304 319 334 184 

Number of floating-point operations: 225
Elapsed time: 5e-06
FLOPS: 4.5e+07
```

which is the same as my python code's output


Now test whether the zero padding is really correct with different strides:
- stride 2, 2
    ```C
    (base) wirelessprv-10-194-151-13:Convolutional_N jinboci$ ./test -x 5 -y 5 -kw 3 -kh 3 -vs 2 -hs 2 -p -o -k 1 -m 2
    get_options
    Options:
        Number of iterations: 	-c 1
        Image size: 	-x 5	-y 5
        Kernel size: 	-kw 3	-kh 3
        Striding: 	-vs 2	-hs 2
        Padding: 	-vp 6	-hp 6
        Kernel Type: 		-k 1 - 2D
        Implementation style:	-m 2 - external 0-padding
        Output: 		-o 1 - enabled

    run_test 
    opt.Wx 5
    opt.w 3
    opt.Pw 6
    opt.Ph 6
    Input matrix: 
    0 1 2 3 4 
    5 6 7 8 9 
    10 11 12 13 14 
    15 16 17 18 19 
    20 21 22 23 24 

    Kernel: 
    0 1 2 
    3 4 5 
    6 7 8 

    Output matrix: 
    0 0 0 0 0 
    0 88 175 136 0 
    0 345 528 345 0 
    0 232 319 184 0 
    0 0 0 0 0 

    Number of floating-point operations: 225
    Elapsed time: 4e-06
    FLOPS: 5.625e+07

    ```
    the same as output 2d in my python code:
    ```python
    output2d:
    [[  0.   0.   0.   0.   0.]
    [  0.  88. 175. 136.   0.]
    [  0. 345. 528. 345.   0.]
    [  0. 232. 319. 184.   0.]
    [  0.   0.   0.   0.   0.]]
    ```
- stride 5, 6
  ```C
        (base) wirelessprv-10-194-151-13:Convolutional_N jinboci$ ./test -x 5 -y 5 -kw 3 -kh 3 -vs 5 -hs 6 -p -o -k 1 -m 2
    get_options
    Options:
        Number of iterations: 	-c 1
        Image size: 	-x 5	-y 5
        Kernel size: 	-kw 3	-kh 3
        Striding: 	-vs 5	-hs 6
        Padding: 	-vp 18	-hp 22
        Kernel Type: 		-k 1 - 2D
        Implementation style:	-m 2 - external 0-padding
        Output: 		-o 1 - enabled

    run_test 
    opt.Wx 5
    opt.w 3
    opt.Pw 22
    opt.Ph 18
    Input matrix: 
    0 1 2 3 4 
    5 6 7 8 9 
    10 11 12 13 14 
    15 16 17 18 19 
    20 21 22 23 24 

    Kernel: 
    0 1 2 
    3 4 5 
    6 7 8 

    Output matrix: 
    0 0 0 0 0 
    0 0 0 0 0 
    0 0 528 0 0 
    0 0 0 0 0 
    0 0 0 0 0 

    Number of floating-point operations: 225
    Elapsed time: 7e-06
    FLOPS: 3.21429e+07
  ```
  in python code: (attention, the input of my python's function input is the half padding)
  ```python
  output2d:
    [[  0.   0.   0.   0.   0.]
    [  0.   0.   0.   0.   0.]
    [  0.   0. 528.   0.   0.]
    [  0.   0.   0.   0.   0.]
    [  0.   0.   0.   0.   0.]]
  ```

## Test with different sizes of kernels for the 2d convolution
Random test:
```C
(base) wirelessprv-10-194-151-13:Convolutional_N jinboci$ ./test -x 5 -y 3 -kw 2 -kh 4 -vs 3 -hs 1 -p -o -k 1 -m 2
get_options
Options:
	Number of iterations: 	-c 1
	Image size: 	-x 5	-y 3
	Kernel size: 	-kw 2	-kh 4
	Striding: 	-vs 3	-hs 1
	Padding: 	-vp 7	-hp 1
	Kernel Type: 		-k 1 - 2D
	Implementation style:	-m 2 - external 0-padding
	Output: 		-o 1 - enabled

run_test 
opt.Wx 5
opt.w 2
opt.Pw 1
opt.Ph 7
Input matrix: 
0 1 2 3 4 
5 6 7 8 9 
10 11 12 13 14 

Kernel: 
0 1 
2 3 
4 5 
6 7 

Output matrix: 
7 20 33 46 24 
124 139 154 169 74 
0 0 0 0 0 

Number of floating-point operations: 120
Elapsed time: 5e-06
FLOPS: 2.4e+07

```
also the same as the output in the python, but attention, the input of this test function is tricky, sometimes vertical axis first, sometimes, horizontal first.
```python
output2d:
 [[  7.  20.  33.  46.  24.]
 [124. 139. 154. 169.  74.]
 [  0.   0.   0.   0.   0.]]
```



## Test with different sizes of kernels for the two 1d convolution

It seems that the mode 1 and mode 2 are similar, the ouput the same results
### One simple case
- mode 1
  ```C
    (base) wirelessprv-10-194-151-13:Convolutional_N jinboci$ ./test -x 5 -y 5 -kw 3 -kh 3 -vs 1 -hs 1 -p -o -k 2 -m 1
    get_options
    Options:
        Number of iterations: 	-c 1
        Image size: 	-x 5	-y 5
        Kernel size: 	-kw 3	-kh 3
        Striding: 	-vs 1	-hs 1
        Padding: 	-vp 2	-hp 2
        Kernel Type: 		-k 2 - Separable
        Implementation style:	-m 1 - inline 0-padding
        Output: 		-o 1 - enabled

    run_test 
    opt.Wx 5
    opt.w 3
    opt.Pw 2
    opt.Ph 2
    Input matrix: 
    0 1 2 3 4 
    5 6 7 8 9 
    10 11 12 13 14 
    15 16 17 18 19 
    20 21 22 23 24 

    2 kernels: 
    0 
    1 
    2 
    0 1 2 

    Output matrix: 
    36 45 54 63 22 
    81 90 99 108 37 
    126 135 144 153 52 
    171 180 189 198 67 
    62 65 68 71 24 

    Number of floating-point operations: 130
    Elapsed time: 5e-06
    FLOPS: 2.6e+07

  ```
- mode 2
    ```C
    (base) wirelessprv-10-194-151-13:Convolutional_N jinboci$ ./test -x 5 -y 5 -kw 3 -kh 3 -vs 1 -hs 1 -p -o -k 2 -m 2
    get_options
    Options:
        Number of iterations: 	-c 1
        Image size: 	-x 5	-y 5
        Kernel size: 	-kw 3	-kh 3
        Striding: 	-vs 1	-hs 1
        Padding: 	-vp 2	-hp 2
        Kernel Type: 		-k 2 - Separable
        Implementation style:	-m 2 - external 0-padding
        Output: 		-o 1 - enabled

    run_test 
    opt.Wx 5
    opt.w 3
    opt.Pw 2
    opt.Ph 2
    Input matrix: 
    0 1 2 3 4 
    5 6 7 8 9 
    10 11 12 13 14 
    15 16 17 18 19 
    20 21 22 23 24 

    2 kernels: 
    0 
    1 
    2 
    0 1 2 

    Output matrix: 
    36 45 54 63 22 
    81 90 99 108 37 
    126 135 144 153 52 
    171 180 189 198 67 
    62 65 68 71 24 

    Number of floating-point operations: 180
    Elapsed time: 2e-06
    FLOPS: 9e+07

    ```
    same as the output of my python program
    ```python
    output1d2:
    [[ 36.  45.  54.  63.  22.]
    [ 81.  90.  99. 108.  37.]
    [126. 135. 144. 153.  52.]
    [171. 180. 189. 198.  67.]
    [ 62.  65.  68.  71.  24.]]
    ```

### One random case:
- mode 1

    ```C
        (base) wirelessprv-10-194-151-13:Convolutional_N jinboci$ ./test -x 5 -y 3 -kw 2 -kh 4 -vs 3 -hs 1 -p -o -k 2 -m 1
    get_options
    Options:
        Number of iterations: 	-c 1
        Image size: 	-x 5	-y 3
        Kernel size: 	-kw 2	-kh 4
        Striding: 	-vs 3	-hs 1
        Padding: 	-vp 7	-hp 1
        Kernel Type: 		-k 2 - Separable
        Implementation style:	-m 1 - inline 0-padding
        Output: 		-o 1 - enabled

    run_test 
    opt.Wx 5
    opt.w 2
    opt.Pw 1
    opt.Ph 7
    Input matrix: 
    0 1 2 3 4 
    5 6 7 8 9 
    10 11 12 13 14 

    2 kernels: 
    0 
    1 
    2 
    3 
    0 1 

    Output matrix: 
    3 6 9 12 0 
    28 31 34 37 0 
    0 0 0 0 0 

    Number of floating-point operations: 47
    Elapsed time: 4e-06
    FLOPS: 1.175e+07

    ```
- mode 2
    ```C
    (base) wirelessprv-10-194-151-13:Convolutional_N jinboci$ ./test -x 5 -y 3 -kw 2 -kh 4 -vs 3 -hs 1 -p -o -k 2 -m 2
    get_options
    Options:
        Number of iterations: 	-c 1
        Image size: 	-x 5	-y 3
        Kernel size: 	-kw 2	-kh 4
        Striding: 	-vs 3	-hs 1
        Padding: 	-vp 7	-hp 1
        Kernel Type: 		-k 2 - Separable
        Implementation style:	-m 2 - external 0-padding
        Output: 		-o 1 - enabled

    run_test 
    opt.Wx 5
    opt.w 2
    opt.Pw 1
    opt.Ph 7
    Input matrix: 
    0 1 2 3 4 
    5 6 7 8 9 
    10 11 12 13 14 

    2 kernels: 
    0 
    1 
    2 
    3 
    0 1 

    Output matrix: 
    3 6 9 12 0 
    28 31 34 37 0 
    0 0 0 0 0 

    Number of floating-point operations: 102
    Elapsed time: 1e-06
    FLOPS: 1.02e+08
    ```
    Also the same as the output of my function.
    ```python
    output1d2:
    [[ 3.  6.  9. 12.  0.]
    [28. 31. 34. 37.  0.]
    [ 0.  0.  0.  0.  0.]]
    ```
## A crazy test, bigger matrix
- 2d, the same as my python code
  ```
    output2d:
    [[     0.      0.      0.      0.      0.      0.      0.      0.      0.
        0.      0.      0.      0.      0.      0.      0.      0.      0.
        0.      0.      0.      0.      0.]
    [     0.      0.      0.      0.      0.      0.      0.      0.      0.
        0.      0.      0.      0.      0.      0.      0.      0.      0.
        0.      0.      0.      0.      0.]
    [     0.      0.      0.      0.      0.      0.      0.      0.      0.
        0.      0.      0.      0.      0.      0.      0.      0.      0.
        0.      0.      0.      0.      0.]
    [     0.      0.      0.      0.      0.      0.      0.      0.      0.
        0.      0.      0.      0.      0.      0.      0.      0.      0.
        0.      0.      0.      0.      0.]
    [     0.      0.      0.      0.      0.      0.      0.      0.      0.
        0.      0.      0.      0.      0.      0.      0.      0.      0.
        0.      0.      0.      0.      0.]
    [     0.      0.      0.      0.      0.      0.      0.      0.      0.
        0.      0.      0.      0.      0.      0.      0.      0.      0.
        0.      0.      0.      0.      0.]
    [     0.      0.      0.      0.      0.      0.      0.      0.      0.
        4270.   7357.   9282.  11207.   8770.      0.      0.      0.      0.
        0.      0.      0.      0.      0.]
    [     0.      0.      0.      0.      0.      0.      0.      0.      0.
    29700.  42350.  45325.  48300.  34500.      0.      0.      0.      0.
        0.      0.      0.      0.      0.]
    [     0.      0.      0.      0.      0.      0.      0.      0.      0.
    60750.  83405.  86380.  89355.  62100.      0.      0.      0.      0.
        0.      0.      0.      0.      0.]
    [     0.      0.      0.      0.      0.      0.      0.      0.      0.
    91800. 124460. 127435. 130410.  89700.      0.      0.      0.      0.
        0.      0.      0.      0.      0.]
    [     0.      0.      0.      0.      0.      0.      0.      0.      0.
    122850. 165515. 168490. 171465. 117300.      0.      0.      0.      0.
        0.      0.      0.      0.      0.]
    [     0.      0.      0.      0.      0.      0.      0.      0.      0.
    153900. 206570. 209545. 212520. 144900.      0.      0.      0.      0.
        0.      0.      0.      0.      0.]
    [     0.      0.      0.      0.      0.      0.      0.      0.      0.
    184950. 247625. 250600. 253575. 172500.      0.      0.      0.      0.
        0.      0.      0.      0.      0.]
    [     0.      0.      0.      0.      0.      0.      0.      0.      0.
    32485.  39886.  40341.  40796.  24925.      0.      0.      0.      0.
        0.      0.      0.      0.      0.]
    [     0.      0.      0.      0.      0.      0.      0.      0.      0.
        0.      0.      0.      0.      0.      0.      0.      0.      0.
        0.      0.      0.      0.      0.]
    [     0.      0.      0.      0.      0.      0.      0.      0.      0.
        0.      0.      0.      0.      0.      0.      0.      0.      0.
        0.      0.      0.      0.      0.]
    [     0.      0.      0.      0.      0.      0.      0.      0.      0.
        0.      0.      0.      0.      0.      0.      0.      0.      0.
        0.      0.      0.      0.      0.]
    [     0.      0.      0.      0.      0.      0.      0.      0.      0.
        0.      0.      0.      0.      0.      0.      0.      0.      0.
        0.      0.      0.      0.      0.]
    [     0.      0.      0.      0.      0.      0.      0.      0.      0.
        0.      0.      0.      0.      0.      0.      0.      0.      0.
        0.      0.      0.      0.      0.]
    [     0.      0.      0.      0.      0.      0.      0.      0.      0.
        0.      0.      0.      0.      0.      0.      0.      0.      0.
        0.      0.      0.      0.      0.]]
  ```
  ```C
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 4270 7357 9282 11207 8770 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 29700 42350 45325 48300 34500 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 60750 83405 86380 89355 62100 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 91800 124460 127435 130410 89700 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 122850 165515 168490 171465 117300 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 153900 206570 209545 212520 144900 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 184950 247625 250600 253575 172500 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 32485 39886 40341 40796 24925 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
  ```
- 1D
    ```C
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 2190 3010 3745 4480 2390 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 14300 16030 17080 18130 9000 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 28100 30520 31570 32620 15900 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 41900 45010 46060 47110 22800 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 55700 59500 60550 61600 29700 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 69500 73990 75040 76090 36600 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 83300 88480 89530 90580 43500 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 8790 9331 9436 9541 4580 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
    ```
    the same as my python code?
## Test valgrind

It seems that there is memory leaks of valgrind in MacOS???

## A summary
Most bugs are in my python function