# UnixUndelete
Variation of rm on Unix. Allows users to safely remove files into a trashbin.
Implemented in both Bash and C. C implementation only has srm, unrm, and trash.

Commands:  
srm- safe version of "rm", moves files into a trashbin  
unrm- takes inputted files out of the trashbin into cwd  
lsrm- performs "ls -l" on all files in trashbin  
durm- runs "du" on trashbin to show how much disk space is left  
trash- permanently deletes all files in trashbin  

Notes:  
-any flags provided with "srm" will simply run with /bin/rm  
-works with directories  
-"srm" and "unrm" are called with as many arguments; any files that don't exist will be ignored   
