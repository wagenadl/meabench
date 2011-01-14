/* common/directory.H: part of meabench, an MEA recording and analysis tool
** Copyright (C) 2000-2002  Daniel Wagenaar (wagenaar@caltech.edu)
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

// directory.H - cd, mkdir and ls commands for inclusion into main.C files

/* Defined functions are:                             
                                                      
     void cd(int argc, char **args)                   
     void ls(int argc, char **args)                   
     void mkdir(int argc, char **args)                
                                                      
     and for private use:                             
                                                      
     void allsys(char *cmdbase, int argc, char **args)

   Recommended code for Cmdr::Cmap inclusion:         

  { cd, "cd", 0, 1, "[directory name]", },
  { ls, "ls", 0, 100, "[ls args]", },
  { mkdir, "mkdir", 1, 100, "mkdir args", },

*/

#ifndef DIRECTORY_H

#define DIRECTORY_H

#include <string>
#include <stdlib.h>

#include <common/MEAB.H>

void cd(int argc=0, char **args=0) {
  if (argc) {
    if (chdir(args[0]))
      throw SysErr("cd","Cannot change directory");
  }
  char buf[1000];
  if (getcwd(buf,1000)==0)
    throw SysErr("cd","Cannot obtain working directory");
  fprintf(stderr,"Working directory: %s\n",buf);
  char *term = getenv("TERM");
  if (term && !strcmp(term,"xterm")) {
    if (MEAB::progname != "")
      fprintf(stderr,"\e]0;%s: %s\07",MEAB::progname.c_str(),buf);
    else
      fprintf(stderr,"\e]0;%s\07",buf);
  }
}

void allsys(char const *cmdbase, int argc, char **args) {
  string cmd=cmdbase;
  for (int i=0; i<argc; i++) {
    cmd+=" ";
    cmd+=args[i];
  }
  if (system(cmd.c_str())) 
    perror("system call failed in directory.H");
}

void ls(int argc, char **args) {
  allsys("/bin/ls",argc,args);
}

void mkdir(int argc, char **args) {
  allsys("/bin/mkdir",argc,args);
}

#endif
