# TEAM:PTZ
- **Members:**
	- [Patrick Fowley](https://github.com/Fowley-P)
	- [Tobu Widjaja](https://github.com/Tobu9009)
	- [Zavier Jenkins](https://github.com/ZJen64)

- **Name:**
	- ptracer - process tracer

- **Synopsis:**
	- ptracer [-h] [-f file] command [args]


- **Description:**
	- command: Specifies the program that your program needs to execute and trace (referred to as "tracee").
	- args: Specifies a list of arguments to provided to command when it is run.
	- -f file: In addition to normal output, export the results to a text file in a (tabular) tab-separated format. You can use this structured data to perform additional automated analysis. Note that this option does not affect the format of standard output.
	- h: Print the help summary.
	- The ptracer() function collects system call and signal information from a running program.
	- The program generates a log of system calls and signals of the tracee. The following functions are expected:

		- **Help summary:** Print a help summary in case of insufficient/invalid arguments (to standard error).
		- **Tracing system calls:** Print any of a selection of system calls, their arguments, and results.
		- **Tracing signals:** Same as above for signals.
		
 - **Missing Functionality**
 	- Can run, but cannot trace, grandchild threads and any of their children

- **Additional Functionality**
	- None
	
- **Member/Task assignment:**
	- "Members of PTZ actively collabrorated on each part of the code with varying focuses based on intensity"

	| Member        | Task          | Task Completion  | Contribution  |
	| ------------- |:-------------:| -----:| -----:|
	| Patrick      | Primary Code writer, code organization, debugging | ~80% | 33.4% |
	| Tobu      | Research, Debugging, Secondary Code writer     |   ~80%| 33.3%|
	| Zavier | Recording, Documents, Debugging, Third Code Writer(clean up)     |    ~80%| 33.3% |
	
	
