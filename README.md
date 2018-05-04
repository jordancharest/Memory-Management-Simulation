# Memory Management Simulation
The Memory Management Simulator simulates how an OS would manage process memory using three types of contiguous memory allocation schemes (Next Fit, Best Fit, and Worst Fit) and a non-contiguous memory allocation scheme.

## Compile and Run
g++ -std=c++11 *.cpp -o main.exe

./main.exe \<input-file\>

## Input File
The program expects a formatted input file following the following formatting rules:
 - lines that that start with # are ignored
 - each process must be on its own line and take the form: 
    - proc-id mem_frames burst1_arr_time/burst1_run_time <...> burstn_arr_time/burstn_run_time
 - process IDs must be a single character and be unique
 
An example input file may look something like this:
```
\# example input file  <-- commented line
A 45 0/350 400/50
B 28 0/2650
C 58 0/950 1100/100
D 86 0/650 1350/450
E 14 0/1400
F 24 100/380 500/475
G 13 435/815
J 46 550/900
```

- In this example process A requires 45 frames in memory and runs twice. The first time it arrives at time t = 0 ms and runs for 350 ms, the second burst it arrives at t = 400 ms and runs for 50 ms

## Output
Output is written to STDOUT and consists of timestamped events such as process arrival, defragmentation, process exit, etc. as well as a display of the current state of memory. For example, using the example input from above, after starting the Next Fit simulation and after process A is placed, output would appear as shown below (note that available memory is 256 frames and is a tunable parameter):
```
time 0ms: Simulator started (Contiguous -- Next-Fit)
time 0ms: Process A arrived (requires 45 frames)
time 0ms: Placed process A:
================================
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
AAAAAAAAAAAAA...................
................................
................................
................................
................................
................................
................................
================================
```
