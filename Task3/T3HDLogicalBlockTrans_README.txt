T3HDLogicalBlockTrans_README.text


                                Hard Drive Logical Block to Physical Sector Translator


Purpose
    This program translates hard drive logical block addresses to physical addresses. It takes a logical block number, the number of cylinders, tracks, and sectors in the harddrive and then displays the physical address as a 3-tuple < cylinder #, track #, sector #>.

How it works
    The program is run from the command line, all the information is entered iteratively by prompts, and then it will automatically display the physical address of the block.

    The program has minor error checking for invalid addresses given the input, but not much else


Credits
    Mario Mariotta
