/* base/Cmdr.C: part of meabench, an MEA recording and analysis tool
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

// Cmdr.C

#include "Cmdr.H"
#include "Sigint.H"
#include "Error.H"
#include "Linebuf.H"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//extern "C" {
//#include <readline/readline.h>
//#include <readline/history.h>
//};

namespace Cmdr {

  static char *tokens[MAXTOKS];
  static bool letsquit;
  static int argc; // set by tokenize for use by exec and loop
  // unlike C-standard, argc does not include the command, so for "rec foo",
  // argc would be 1 (not 2).
  static bool shell_enabled = true;

  void enable_shell(bool on) {
    shell_enabled = on;
  }
  
  bool isprefix(char const *short_a, char const *long_a) {
    while (*short_a && *short_a==*long_a) {
      short_a++; long_a++;
    }
    return *short_a==0;
  }

  char **tokenize(char *src, bool coloncomma) {
    int l=strlen(src);
    if (l>0 && src[l-1]=='\n')
      src[l-1]=0; // remove newline
    tokens[0] = strtok(src,coloncomma?":":" ");
    int n=1;
    while ((tokens[n++]=strtok(0,coloncomma?",":" ")))
      ;
    argc = n-2;
    return tokens;
  }

  Cmap *parse(char *cmd, Cmap *cmds) throw(Error) {
    Cmap *f=0;
    while (cmds->foo) {
      if (isprefix(cmd,cmds->cmd)) {
	if (f)
	  throw Error("Cmdr",string("Ambiguous command: ")+cmd);
	else
	  f = cmds;
      }
      cmds++;
    }
    if (f)
      return f;
    throw Error("Cmdr",string("Unknown command: ")+cmd);
  }

  void quit(int argc, char **argv) {
    letsquit = true;
  }

  void help(int argc, char **argv, Cmap *cmds) {
    if (argc) {
      for (int i=0; i<argc; i++) {
	try {
	  Cmap *x = parse(argv[i],cmds);
	  throw(Usage(x->cmd,x->usage));
	} catch (Error const &e) {
	  e.report();
	}
      }
    } else {
      fprintf(stderr,"Commands are:\n");
      while (cmds->foo) {
	char buf[MAXUSAGELEN];
	char *x = strchr(cmds->usage,'\n');
	if (x) {
	  strncpy(buf,cmds->usage,x-cmds->usage);
	  buf[x-cmds->usage] = 0;
	} else
	  strcpy(buf,cmds->usage);
	fprintf(stderr,"  %s %s\n",cmds->cmd,buf);
	cmds++;
      }
      if (shell_enabled)
	fprintf(stderr,"  ! - shell escape\n");
    }
  }
  
  bool exec(char *cmdwargs, Cmap *cmds, bool coloncomma) throw(Error) {
    letsquit = false;
    while (*cmdwargs==' ')
      cmdwargs++; // skip initial spaces
    if (*cmdwargs == '!' && shell_enabled) {
      // shell command
      if (system(cmdwargs+1))
	throw SysErr("Cmdr","Shell failed");
      return false;
    }
    char **args = tokenize(cmdwargs, coloncomma);
    if (!args[0])
      return 0; // null command, no action
    if (!strcmp(args[0],"?")) {
      help(argc,args+1,cmds);
      return false;
    }
    Cmap *c = parse(args[0], cmds);
    if (argc<c->argc_min || argc>c->argc_max)
      throw Usage(c->cmd,c->usage);
    (c->foo)(argc,args+1);
    return letsquit;
  }

  bool exec(int argc, char **argv, Cmap *cmds) throw(Error) {
    for (int i=1; i<argc; i++)
      if (exec(argv[i],cmds,true))
	return true;
    return false;
  }

  void loop(char const *name, Cmap *cmds, Linebuf *src) {
    letsquit = false;
    string prompt = "\e[32m\e[1m";
    prompt+=name;
    prompt+=">\e[0m ";
    fflush(stderr); fflush(stdout);
    setvbuf(stderr,0,_IOLBF,1024);
    setvbuf(stdout,0,_IOLBF,1024);
    while (!feof(stdin) && !letsquit) {
      Sigint::clear();
      ////char *b = readline(prompt.c_str());
      fprintf(stderr,"%s%s",prompt.c_str(),src?src->currentcontents():"");
      fflush(stderr); fflush(stdout);
      char buf[BUFLEN];
      char *b;
      if (src) {
	try {
	  b = strcpy(buf,src->readline());
	} catch (Expectable const &e) {
	  if (e.msg().find("EOF") != string::npos)
	    b = 0;
	  else
	    throw;
	} catch (Error const &e) {
	  fprintf(stderr,"\n");
	  e.report();
	  b = buf; *buf=0;
	}
      } else {
	b = fgets(buf,BUFLEN,stdin);
      }
      if (b) {
	try {
	  exec(b,cmds,false);
	} catch (Error const &e) {
	  fprintf(stderr,"\n");
	  e.report();
	}
	////delete b;
      } else {
	letsquit = true; // eof!
      }
    }
    fprintf(stderr,"\n");
  }
};
