 # Easy vulnerability was found
 Buffer where users input is stored located in the middle of program, which allows overflowing the buffer and changing functions located after it. In this case we need to replace first command of the hashing function with a simple jmp to 0C, IP on which admin rights are obtained. To optimise overflowing process, we need to make big enough file and redirect input to it. Success message can also be customized that way: in our case it is "Heavy Honey Sigma Balls!@ GANG".

 ## Example of program-cracking file:
 [Better read in any HEX editor](inputHEX.txt)
