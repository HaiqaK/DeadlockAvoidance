# CP386 - Operating Systems - Assignment 4

## Motivation 
## Installation

1. Download the repository onto your system
2. Rename makefile.mk to makefile, run:
  ```bash
  mv makefile.mk Makefile
  ```
3. Makefile is in the same folder as your .c file to compiled run
  ```base
  make
  ```
4. This will compile your program with warnings turned into errors. If you instead run
  ```bash
  make warning_OK
  ```
  this will compile your program with warning enabled but not fatal. If successful (ie gcc didnt complain) you should now have a file name Assignment_4 (with         no file extention)
5. To run the program use 
  ```bash
  ./Assignment_4 a b c ... y z 
  ``` 

## Screenshots

![image](https://i.paste.pics/9PNNQ.png)
![image](https://i.paste.pics/9PNNU.png)

## Individual Contribution 

All function - Haiqa Khurshid

## Features

* RQ - Allocated Resources
* RL - Release Resources 
* * - Current State
* Run - Current Sequence 
* Exit - Exit Program

## Tests

Refer to Screenshots

## Code Example 

Functions :
```
  int **readFile(char* fileName);
  void *threadRun(void *t);
  int *safetySeq();
  void spd(int *data, int a);         // single pointer data
  void dpd(int **data, int a, int b); // double pointer dadta
```

## Authors 
Haiqa Khurshid
Github: @HaiqaK

## Credits 

## License 
