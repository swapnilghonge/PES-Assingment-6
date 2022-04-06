# PES-Assingment-6<\br>
Breakfast Serial<\br>

As a way to allow your company’s development and QA teams to more fully exercise a new board under<\br>
development, you have been given the task of setting up an initial command processor running over the<\br>
serial line. Your manager, a lifelong fan of sugar in the morning, has given this project the code name<\br>
BreakfastSerial, since it will be the first thing done on this board.<\br>
You will need to develop the following:<\br>
1) A circular buffer implementation. You will need two circular buffer objects, one each for the<\br>
transmit and receive directions. These may be allocated statically (the preferred approach), or<\br>
they may be allocated dynamically at initialization time1. Each circular buffer should have a<\br>
capacity of 256 bytes.<\br>
2) Test code to exercise your circular buffer. You may wish to adapt the automated tests you<\br>
created for Assignment 2. This code should run at startup if the DEBUG define is set in your<\br>
code, in order to give you confidence that your circular buffers are solid.<\br>
3) Code to configure UART0 and send and receive data over it. Parameters for this assignment<\br>
are specified below. Your implementation should be fully interrupt based2. The UART solution<\br>
should be built atop your circular buffer implementation.<\br>
4) Glue code that ties your UART communication code into the standard C library functions.<\br>
After this glue code is working, a call to printf() or putchar() on the device should result in<\br>
characters being sent over UART0 to the PC, and a call to getchar() should result in reading a<\br>
character that the user typed on the PC.<\br>
5) A command processor that can accept some very simple interactive commands (specified<\br>
below) and take action on the device.<\br>
