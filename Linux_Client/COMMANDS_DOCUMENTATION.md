# Commands Documentation

### Overview

The following environment variables are listed in the GRBL version flashed onto the arduino uno.

**Note:** in v0.9 of GRBL

### Commands

``` bash
Grbl 0.9j ['$' for help]
$0 = 10    (step pulse, usec)
$2 = 0    (step port invert mask:00000000)
$5 = 0    (limit pins invert, bool)
$6 = 0    (probe pin invert, bool)
$3 = 0    (dir port invert mask:00000000)
$10 = 3    (status report mask:00000011)
$1 = 25    (step idle delay, msec)
$12 = 0.002    (arc tolerance, mm)
$20 = 0    (soft limits, bool)
$21 = 1    (hard limits, bool)
$22 = 0    (homing cycle, bool)
$23 = 0    (homing dir invert mask:00000000)
$13 = 0    (report inches, bool)
$11 = 0.010    (junction deviation, mm)
$4 = 0    (step enable invert, bool)
$24 = 25.000    (homing feed, mm/min)
$27 = 1.000    (homing pull-off, mm)
$101 = 6.198    (y, step/mm)
$26 = 250    (homing debounce, msec)
$110 = 500.000    (x max rate, mm/min)
$100 = 5.020    (x, step/mm)
$25 = 500.000    (homing seek, mm/min)
$112 = 500.000    (z max rate, mm/min)
$121 = 10.000    (y accel, mm/sec^2)
$120 = 10.000    (x accel, mm/sec^2)
$130 = 200.000    (x max travel, mm)
$111 = 500.000    (y max rate, mm/min)
$102 = 25.000    (z, step/mm)
$131 = 200.000    (y max travel, mm)
$122 = 10.000    (z accel, mm/sec^2)
$132 = 200.000    (z max travel, mm)
```