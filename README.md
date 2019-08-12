THIS SHELL WORKS ONLY ON LINUX BASED SYSTEMS !
All the commands for the shell are implemented by us. Hence they require two external libraries to make the code work . They are 
curl and readline . 
First run "bash deps.sh" from the current directory to install the libraries .
Next run "bash run.sh" to run the makefile and execute the shell .

Commands supported : 

Convention : Entities within () are optional . Entities within <> are mandatory .

The commands that we implemented are : 
1 . ls : List files and folders in the current directory .
		 Syntax : ls (foldername)
2 . disp : Display contents of a file .
		 Syntax : disp <filename>
3 . history : Show the commands executed in the current session . Number of commands to store can be given in the limits.h file
		 Syntax : history
4 . sort : Sorts the contents of a file .
 		 Syntax : sort <filename>
5 . grep : Displays lines of a file which match the given text
		 Syntax : grep <filename> <text>
6 . who : Displays the user currently logged in .
		 Syntax : who
7 . wget : Downloads a file . (Uses the curl library which may not be available in vanilla linux distros) .
		 Syntax : wget <url> <filename>
8 . cpy : Copies contents of one file to other . The destination and source files must exist .
		 Syntax : cpy <src> <dst> 
9 . cwd : Change working directory . 
		 Syntax : cwd <directory>
10 . pwd : Print current working diretory .
 		 Syntax : pwd
11 . mkdir : Create a new directory .
		 Syntax : mkdir <directory>

Apart from the above explicit commands , the shell also supports pseudo commands . For example !! runs the first element in the history .
Note that history is indexed from 1 and the first element is the oldest command executed . !<num> executes the num-th command in the 
history .

Apart from the above commands , the terminal also has an auto completion feature . You can press [Tab] button after partially typing a 
command to complete  it . After the command , if the first argument is a file name , then you can also attempt to autocomplete the file
name by pressing [Tab] . All command are available for autocomplete except pwd .

Output and input redirects are supported for selected commands . Output redirect is applicable for all except cpy , wget , mkdir . 
Input redirect is applicable only for sort and disp .

The commands can be chained(piped) using the '|' operator . This command sends the ouput of previous command to the input of next command .
Consequently , one argument of the second command can be dropped . For example : 'ls | sort | grep .c' will perform ls first , give the 
output to the sort command and then the sorted output os given to the grep command . Note that the first argument is not required in this 
case . THe first argument is the sorted output itself . If you give two arguments in this case , the whole string will be considered as 
search string .

Code structure :

shell_pipe.c : This file contains the main function and links and the other files into a single file . This file is responsible for creating
		child processes for executing the input commands .
parse.c : This file contains functions to parse the input command .
utils.c : This file contains many utility functions which are helpful and used in many places .
comms.c : This file contains the implementations of all the commands .
headers.h : Contains prototypes of all the functions used in parse.c , utils.c and comms.c and also some custom data types .
limits.h : This file contains the limits that the shell works under .  THe values in this file can be tweaked .

Shortcomings :

The shell is bounded by the values in limits.h . The shared memory size is fixed , and all the commands work around this restriction .
For example , if you have a file of size 1GB , you cant use the use the disp command to show the contents of the file . Only first 
MAX_SHARED_MEMORY bytes are shown .
Auto complete is not available for one command because we were getting segmentation fault for more than ten commands .

Team Contribution :

Yashwanth Betha (140101013) : implemented command chaining and sort , ls , grep commands .
Pavan Karthik Boddeda (140101014) : implemented history , disp and !! and !<num> .
Mahesh Kumar Chelimilla (140101015) : implemented and who , cpy and tested the code and corrected memory leaks .
George Veneel Doggs (140101021) : implemented the auto complete feature and remaining commands .
