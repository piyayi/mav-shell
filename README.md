# mav-shell

compile on command line like this: gcc msh.c -o msh —std=c99 

Mav shell searches in the following PATH order:  
1. Current working directory,  
2. /usr/local/bin 
3. /usr/bin 
4. /bin 

takes additional commands:
1. History
2. showpids
3. cd

limitations: takes 10 commandline parameters at a time
