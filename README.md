# Deanna Liu Homework 4 #

## Part II

#### Program 1: Fibonacci
* Returns the Nth Fibonacci Number.

```c
$ make progfib
$ ./hw4_progfib 16
```

#### Program 2: Counter
* Counts the number of occurrences of zeros, ones and two's in 3 files named: **file1.dat**, **file2.dat**, and **file3.dat**
* Each file contains two lines. The first line contains a number *N*. The second line contains *N* space-separated numbers. It won't do anything unless file is formatted correctly.
```
$ make progcnt
$ ./hw4_progcnt
```

#### Program 3: Parallel Quicksort
* Reads from a file.
* File should contain two lines. The first line contains a number *N*. The second line contains *N* space-separated numbers. It won't do anything unless file is formatted correctly.
* Sorts the second line using two threads.
```c
$ make progqsort
$ ./hw4_progqsort filename
```

## Part III 

* Redo Homework #3 using threads instead.

```c
$ make proghw3
$ ./artist_manager
shell> help
shell> hire [N]
shell> fire [X]
shell> fireall
shell> list
shell> exit
```

* arguments are held in an array called `my_argv` which is like a copy of main's `*argv[]` 
* begin the program by filling the array `my_argv` with the commands.
   - method called `fill_argv()` fills the arg_v, separating the user input by a space. 
* `help` prints all the possible commands and a short information about them.
* `hire [N]` where N is an integer. Creates N number of "processes" or "artists". The PID and the Status is stored in a linked list called `artists` in `artist_ds.c`
* `fire [X]` where X is the TID (Thread ID) - can be found by using `list`. This basically kills or "fires" the thread/"artist". Data about them is removed from `artists` as well. 
* `fireall` "Fires" or kills all the thread/"artists". Data about them is removed from `artists` as well.
* `list` - prints out all the current artists/thread in the database in the form of [TID] [STATUS]. In this program, the status are all **WAITING**. 
* `exit` - fires all of the employees. Cleans up database. Flush. Byebye.

### Data Structure
* `artists` is a linked-list 
   - Has 3 attributes:
       1. TID
       2. sig
       3. Pointer to NEXT
* `my_argv` is an array that holds the command and if there is a number that follows it, then it will be in the next index.

## Part IV
* Makeshift Garbage Collector
* 5 art museums at New Stony Brook where every morning all of the museums open their door to the public and in each of the museums there will be one person who opened it and we consider that person as the first visitor. Usually, art museums close in the evening, but in the
city of New Stony Brook, an art museum cannot close if there is at least one visitor. However, once it is evening and there are no visitors in the art museum, it can close its doors. 

```c
$ make 
$ ./museum_overseer
shell> info
shell> in N X
shell> out N X
shell> start
shell> exit
```
* arguments are held in an array called `my_argv` which is like a copy of main's `*argv[]` 
* begin the program by filling the array `my_argv` with the commands.
   - method called `fill_argv()` fills the arg_v, separating the user input by a space. 
* `info` prints out the museums data in the form of **<MUSEUM NUMBER>:<NUMBER OF VISITORS>:<STATUS>**
   - Creates a thread that calls the `museum_info()` function.
* `in [N] [X]` where N & X is an integer. N is the Museum number and X is the number of visitors. Creates X number of threads or "visitors". Each one of these threads increment the global variable counter for museum *N*. The museum number and the status is stored in an array called `museum_ds` & `museum_ds_status`
   - Creates a thread that calls the `visitor_in(int N)` function, *X* amount of times.
   - This command won't do anything if:
     - Museum *N* is closed after `start` is called
     - X is greater than the amount of visitors that are currently in the museum
     - N is not a valid number between 1-5 inclusive -- since there are only 5 museums.
* `out [N] [X]` where N & X is an integer. N is the Museum number and X is the number of visitors. Creates X number of threads or "visitors".  Each one of these threads decrement the global variable counter for museum *N* The museum number and the status is stored in an array called `museum_ds` & `museum_ds_status`
   -  Creates a thread that calls the `visitor_out(int N)` function, *X* amount of times.
   - This command won't do anything if:
     - Museum *N* is closed after `start` is called
     - X is greater than the amount of visitors that are currently in the museum
     - N is not a valid number between 1-5 inclusive -- since there are only 5 museums.
   - Once `start` command has started, if there are 0 visitors after this command, the museum will close.
* `start` creates a thread that will check every three seconds if there are any open art museums with zero visitors. If there
is such an art museum then that thread should close that art museum using museum_close(int N) function, which changes the museums status from **OPEN** to **CLOSED**. 
   - Checks if all the museums are closed as well. If it is, then the program will exit as the "day has ended since everyone is gone and no more visitors are allowed"
* `exit` exits the programs
   - Calls the `museum_clean()` function and closes all the museums using `museum_close()` if they are still open when exiting. 
   - Free up the data structures. 
   - Exit thread, byebye.

### Data Structure
* `museum_ds` is an int array that holds the amount of visitors in each node. 
   - The index + 1 corresponds to the Museum Number.
   - i.e. museum_ds[0] holds information about Museum 1
* `musesum_ds_status` is also a char array that holds the status of each musesum
   - Status' include **OPEN** and **CLOSED**
   - Works hand in hand with `museum_ds`
   - The index + 1 corresponds to the Museum Number.
   - i.e. museum_ds_status[0] holds information about Museum 1. 



# Garbage-Collector
