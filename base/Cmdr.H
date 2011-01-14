/* base/Cmdr.H: part of meabench, an MEA recording and analysis tool
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

// Cmdr.H

#ifndef CMDR_H

#define CMDR_H

#include <base/Error.H>

class Linebuf;

namespace Cmdr {
  const int MAXTOKS = 100;
  const int MAXCMDLEN = 20;
  const int MAXUSAGELEN = 200;
  const int BUFLEN = 1000;
  typedef void (*Foo)(int, char **);
  struct Cmap {
    Foo foo;
    char cmd[MAXCMDLEN];
    int argc_min;
    int argc_max;
    char usage[MAXUSAGELEN];
  };
  extern char **tokenize(char *src, bool coloncomma=0); //:f tokenize
  /*:D tokenize(src,coloncomma) parses a string up into space or colon/comma
       separated constituents, and returns an array of pointers to the parts.
   *:A src: string to tokenize, in the form "xxx yyy zzz ..." or
            "xxx:yyy,zzz,..."
     . coloncomma: true to use :, style strings
   *:R zero terminated array of tokens.
   *:S The source is modified as per stringtok.
   */
  extern bool isprefix(char const *short_a, char const *long_a); //:f isprefix
  /*:D isprefix(a,b) returns true iff a is a prefix of b.
   *:A short_a: potential prefix
    .  long_a:  haystack
   *:R true/false
   *:N If the first arg is the empty string, true is returned always.
   */
  extern Cmap *parse(char *cmd, Cmap *cmds) throw(Error); //:f parse
  /*:D parse(cmd, cmds) tries to look up cmd in the list of commands cmds, and
       if it can be found (potentially as an abbreviation), the corresponding
       Cmap pointer is returned.
   *:A cmd: command (abbreviation) to look for
    .  cmds: set of commands ({0,0} terminated array)
   *:R Pointer to Cmap entry found.
   *:E Exception thrown if the cmd is unknown or ambiguous
   */
  extern void loop(char const *name, Cmap *cmds, Linebuf *src=0); //:f loop
  /*:D loop(name, cmds) performs an interactive loop asking the user for
       commands to map through the given set of commands. The loop exits
       on eof or after the quit command.
   *:A name: name to print as a prompt
    .  cmds: list of commands to recognize
    .  src: Linebuf to read lines from, or 0 to read from stdin
   *:R -
   *:N If unknown commands are typed, a message is written to stderr, but no
       other special action is undertaken.
   *:E Exceptions of type "Error" thrown by the command are caught and
       reported on stderr, other exceptions are passed on.
  */
  extern bool exec(char *cmdwargs, Cmap *cmds, bool coloncomma=false) //:+
    throw(Error); //:f exec
  /*:D exec(cmdwargs, cmds) executes the given command, with arguments
       optionally following it. Returns true
       if the command caused the quit() function to be called.
   *:A cmdwargs: command and arguments as for tokenize()
     . cmds: the set of available commands
     . coloncomma: true to use :, style strings
   *:R true iff quit() was called.
   *:S cmdwargs is modified as per strtok/tokenize.
   *:E Exceptions are thrown if the command is unknown or ambiguous, or
       if the command throws an exception.
   */
  extern bool exec(int argc, char **argv, Cmap *cmds) throw(Error); //:f exec
  /*:D exec(argc, argv, cmds) runs exec on all command line arguments in order.
   *:A argc, argv: as passed to main
    .  cmds: as before
   *:R true iff quit() was called
   *:S argv is modified as per strtok/tokenize.
   *:E Exceptions are thrown if any command is unknown or ambiguous, or
       if any command throws an exception.
   */
  extern void quit(int,char**); //:f quit
  /*:D quit() flags to exec() or to loop() that the program should be
       terminated.
   *:A -
   *:R -
   */
  extern void enable_shell(bool on=true);
};

#endif
