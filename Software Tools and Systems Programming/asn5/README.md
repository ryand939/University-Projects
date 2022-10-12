# Assignment 5

This assignment will simulate the use of a printer queue to manage the processing of printer jobs.
Every time a document is requested to be printed, a printer job is created and placed in the printer queue.
The printer will process one job at a time by printing the page(s) of a document. Once all the pages of the
current printer job have been processed, the request is released, and the printer will inspect the printer
queue to retrieve and remove the next job in the queue.
The simulation will operate on a per cycle basis. Whereas a real printer queue is continuous, this
simulation will execute for a given finite number of cycles.
The main program will loop through this process for some finite set of cycles denoted by the defined
preprocessor definition of ITERATIONS representing number of cycles. 