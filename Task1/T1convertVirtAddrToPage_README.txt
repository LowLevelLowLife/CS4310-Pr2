T1convertVirtAddrToPage_README.text


                                            Virtual Address to Page Address Converter


Purpose
    This is a command line program that will tell you the page and offset for a virtual address, to demonstrate the process of making virtual memory and to understand how processes' code is broken into pieces to enable multiprogramming on modern computers and have processes that are much larger than the physical memory.

How it works
    After the program has been initiated, it will ask you for the size of the pages in the program. This page size is assumed to be in kibibytes (2^10 bytes).

    Then the program will ask you for the raw virtual address for which you want to know the page number and offset.

    After entering an address, the program will display the address and the page number and offset that it would occur in.

    The program does not have error checking/input validation, so please be patient if unexpected output or behaivor occurs.

Credits
    Mario Mariotta
