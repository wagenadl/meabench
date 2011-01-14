# perl/Coproc.pm: part of meabench, an MEA recording and analysis tool
# Copyright (C) 2000-2002  Daniel Wagenaar (wagenaar@caltech.edu)
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


package Coproc;

use strict;
use IPC::Open3("open3");
use IO::Handle;
#use Carp;
use POSIX ":sys_wait_h";

=head1 Coproc

Coproc is part of Meabench, (C) Daniel Wagenaar 2000-2002. PLEASE REFER TO
THE SOURCE IN perl/Coproc.pm FOR INFORMATION ON COPYING AND LACK OF WARRANTY.

Coproc provides the basis for driving meabench processes in an Expect like
manner.

=head2 functions

=over

=item spawn("command args")

Create new process

=item send("line")

Sends a line of text (a "command") to the process

=item intr

Sends an INTR signal to the process

=item term

Sends a TERM signal to the process

=item hardkill

Sends a KILL signal to the process

=item close

Close the connection

=item wait

Waits for the process to end. Optional argument prevents blocking. Returns 
non-negative exit code, -1 if timed out or undef if there is no process.

=item poll

Checks if there is any output from the child process, and adds it the buffer
for expect (see below), as well as passing it to the log function. 
Returns -1 on eof, else 1 if anything was read, 0 otherwise. 
Optional argument specifies number of seconds to wait.

Lines are broken at NL, or at "ESC [ 0 m SPC".

=item expect(regex,timeout)

Waits for a certain regex to occur, returning all lines up to and including 
the one in which the regex occurred, or undef if timed out.

=item flush

Clears the expect buffer

=item comment(string)

Writes the comment to the log function.

=item label(sendlabel,recvlabel)

Commands sent to the process will be echoed to the log function,
prefixed by the given sendlabel. All stdout and stderr read from 
the process will be echoed as well, prefixed by recvlabel.

=item log(function_reference)

Specify the function to send all output to. This function must take
three arguments: CODE, SOURCE and TEXT. CODE as a code from the
following table, SOURCE is a string identifier for the process, and
TEXT is the message text.

CODES are:

   1: text to process
   2: text from process
   3: error in process control
   4: informational message in process control (from comment() )

=head2 member variables (private)

=over

=item pid

pid of child process

=item sendfh

fh to write to child

=item recvfh

fh to receive stdout AND stderr from child

=item sendlabel, recvlabel

=item expectbuffer

Array of lines prepared for expect.

=item linebuffer

Growing string of current line from child.

=back

=cut

sub spawn {
  my $this = shift;
  my $class = ref($this) || $this;
  my $self = {};
  bless $self, $class;

  my $cmd = shift;

  $self->{pid} = open3($self->{sendfh},$self->{recvfh},0,
			    $cmd);
  if ($self->{pid}<0) {
    die "Coproc: spawn failed on $cmd.";
  }
  $self->{label} = "coproc";
  $self->{logfun} = undef;
  $self->{expectbuffer} = [];
  $self->{linebuffer} = "";
  $self->{eof} = 0;
  return $self;
}

sub flush {
  my $self = shift;
  return splice(@{$self->{expectbuffer}});
}

sub send { 
  my $self = shift;
  my $cmd = shift;
  chomp $cmd;
  $self->{sendfh}->print("$cmd\n");
  $self->{logfun}(1,$self->{label}, $cmd) if defined($self->{logfun});
}

sub dump {
  my $self = shift;
  print STDERR "Coproc dump:\n";
  for (sort keys %{$self}) {
    print STDERR "  $_ => ", $self->{$_},"\n";
  }
  print STDERR "End of dump.\n";
}

sub poll {
  my $self = shift;
  my $secs = shift || 0;
  my $ret = 0;
  while (1) {
    my ($rin,$win,$ein);
    my ($rout,$wout,$eout);
    $rin=$win=$ein='';
    vec($rin,fileno($self->{recvfh}),1) = 1;
    $ein=$rin;
    my $nfound = select($rout=$rin, $wout=$win, $eout=$ein, $secs);
    return $ret if $nfound==0;
    my @rout = unpack("c*",$rout);
    my @wout = unpack("c*",$wout);
    my @eout = unpack("c*",$eout);
    my $haseout = 0; $haseout |= $_ for (@eout);
    if ($haseout) {
      die "Coproc: error on poll: $!";
    }
    my $buf;
    my $n = sysread($self->{recvfh},$buf,1000);
    if ($n==0) {
      $self->{eof} = 1;
      return -1;
    }
    $ret = 1;
    while ($buf =~ s/^(.*?(\n|(\033\[0m )))//) {
      $self->{linebuffer} .= $1; 
      $self->completeline();
    }
    $self->{linebuffer} .= $buf;
    $secs = 0;
  }
}

sub completeline {
  my $self = shift;
  my $line = $self->{linebuffer};
  chomp $line;
  push @{$self->{expectbuffer}},$line;
  $self->{linebuffer} = "";
  $self->{logfun}(2,$self->{label}, $line) if defined($self->{logfun});
}

sub expect {
  my $self = shift;
  my $pat = shift;
  my $del = shift || 10000;
  my $i;
  while (1) {
#    print STDERR "[expect: new:$newstuff. n=",
#      scalar(@{$self->{expectbuffer}}), ". lines: \n";
    for ($i=0; $i<scalar(@{$self->{expectbuffer}}); $i++) {
#      print STDERR "  [$i] ",$self->{expectbuffer}[$i],"\n";
      if ($self->{expectbuffer}[$i] =~ $pat) {
	my @out = splice(@{$self->{expectbuffer}},0,$i+1);
	return @out;
      }
    }
    my $newstuff = $self->poll($del);
    last unless $newstuff>0;
#    print STDERR " looping again]\n";
  }
  return ();
}

sub label {
  my $self = shift;
  $self->{label} = shift;
}

sub log {
  my $self = shift;
  my $fun = shift;
  $self->{logfun} = $fun;
}

sub comment {
  my $self = shift;
  my $cmt = shift;
  chomp $cmt;
  $self->{logfun}(4,$self->{label},$cmt) if defined($self->{logfun});
}

sub DESTROY {
  my $self = shift;
  $self->close();
  $self->term();
  my $r = $self->wait(1);
  if (defined($r) && $r<0) {
    $self->kill();
    $self->wait(1);
  }
}

sub close {
  my $self = shift;
  if (defined $self->{sendfh}) {
    $self->{sendfh}->close();
    $self->{sendfh} = undef;
  }
  if (defined $self->{recvfh}) {
    $self->{recvfh}->close();
    $self->{recvfh} = undef;
  }
}

sub term {
  my $self = shift;
  if (defined $self->{pid}) {
    kill(15, ($self->{pid}));
  }
}

sub hardkill {
  my $self = shift;
  if (defined $self->{pid}) {
    kill(9, ($self->{pid}));
  }
}

sub intr {
  my $self = shift;
  if (defined $self->{pid}) {
    kill(2, ($self->{pid}));
  }
}

sub wait {
  my $self = shift;
  my $noblock = shift || 0;
  if (!defined($self->{pid})) {
    return undef;
  }
  my $res;
  if ($noblock) {
    $res = waitpid($self->{pid},&WNOHANG);
  } else {
    $res = waitpid($self->{pid},0);
  }
  return $res if $res<0;
  return -1 if $res==0;
  $self->{pid} = undef;
  return $?;
}

1;
