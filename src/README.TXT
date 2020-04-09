##############__HW3 In Course: Distributed and parallel computing__##############

Made by: Tamir Reznik 
	I.D: 308346170

whats in project: 

6 files: 

	- HW3_Sequential.c		source file to compile and execute (execute as sequential program not parallel **do not run with mpi!**)
	- HW3_Static.c			source file to compile and execute (execute as multi processes program not dequential **run with mpi!**)
	- HW3_Dynamic.c			source file to compile and execute (execute as multi processes program not dequential **run with mpi!**)
	
	- static.h				header file used in HW3_Static.c (necessary for execute HW3_Static.c)
	- dynamic.h				header file used in HW3_Dynamic.c (necessary for execute HW3_Dynamic.c)
	
	- README.TXT 			<---#--->
	
How to run:
	
	two ways to run the file (ubuntu enviorment):
	
	1) use the terminal:
			- gcc [C_FILE_NAME] -lm -o [EXECUTABLE_NAME] 		|command to compile c files that are sequential!
			- mpicc [C_FILE_NAME] -lm -o [EXECUTABLE_NAME] 		|command to compile c files that are not sequential!
			
			- ./[EXECUTABLE_FILE_NAME] to run the compiled sequential program file
			- mpiexe -np [#NUMBER_OF_PROCESSES] [EXECUTABLE_FILE_NAME] to run the parallel program file
			
	2) use the eclipse
	
		if sequential file: 
			- make sure there no multiple main in file (mark as comment the other files if needed)
			- build file
			- run 
	
		if not sequential file:
			- Build project 
			- configre execution settings (number of processes, name of executable, etc.)
			- run
			###NOTE!! - For run with eclipse need to make sure that only one file available (all other can be as comment for avoid multiple main in project)###
			###__in my test i run it with the terminal work's better for me__###