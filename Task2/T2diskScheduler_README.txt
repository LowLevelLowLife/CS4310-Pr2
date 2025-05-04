T2DiskScheduler_README.text


                                           Disk Scheduler


Purpose
    This program simulates scheduling read/write operations on a hard disk. The point is to show how much the harddrive read head travels to service the requests, depending on the algorithm that is used by the device.

How it works
    The program is run from the command line and takes one command line argument, which is the initial position of the harddrive head. Addresses should be between 0 and 4999. The program assumes that a file called "input.txt" is located in the same directory as the program. The program will read that file and then it will implement first come, first served, shortest seek time first, SCAN, and C-SCAN scheduling algorithms on the same data. Then the program will show how much the harddrive head traveled and how many times it changed direction for each algorithm.

    Then the program will simulate 1000 random disk accesses and show the hardrive read statistics for each of the scheduling algorithms as well.


Credits
    Mario Mariotta
