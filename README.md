# Concurrent Programming Using Processes

This project involves concurrent programming using Windows processes that solves the Minimum Distance Problem by dividing the problem into a number of subproblems, each of which is solved by a separate process. The number of processes is equal to the number of cores available on the device this code is running on.<br>
The parent.cpp file divides the problem into subproblems and gives each of them to the child.cpp file, which receives back the answers after a determined time, and after comparing the answers, it chooses the best of them. The process of giving and receiving messages between parent and child source files happens with shared memmory.<br>
This project was written in 2019 and developed in C++, and it is one of my academic projects.