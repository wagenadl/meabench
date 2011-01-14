function y = loadraw(fn, range) 
% y=LOADRAW(fn) reads the raw MEA datafile fn and stores the result in y.
% y=LOADRAW(fn,range) reads the raw MEA datafile fn and converts the
% digital values to voltages by multiplying by range/2048. 
% Range values 0,1,2,3 are interpreted specially:
% 
% range value   electrode range (uV)    auxillary range (mV)
%      0               3410                 4092
%      1               1205                 1446
%      2                683                  819.6
%      3                341                  409.2
% 
% "electrode range" is applied to channels 0..59, auxillary range is
% applied to channels 60..63. Note that channel HW is stored in the
% (HW+1)-th row of the output.

% matlab/loadraw.m: part of meabench, an MEA recording and analysis tool
% Copyright (C) 2000-2002  Daniel Wagenaar (wagenaar@caltech.edu)
%
% This program is free software; you can redistribute it and/or modify
% it under the terms of the GNU General Public License as published by
% the Free Software Foundation; either version 2 of the License, or
% (at your option) any later version.
%
% This program is distributed in the hope that it will be useful,
% but WITHOUT ANY WARRANTY; without even the implied warranty of
% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
% GNU General Public License for more details.
%
% You should have received a copy of the GNU General Public License
% along with this program; if not, write to the Free Software
% Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


if nargin>=2
  if ~isempty(find([0 1 2 3]==range))
    ranges= [ 3410,1205,683,341 ];
    range = ranges(range+1);
    auxrange = range*1.2;
  else
    auxrange = range;
  end
else
  range = 1;
  auxrange = 1;
end

fh = fopen(fn,'rb');
y = fread(fh,[64 inf],'int16');
fclose(fh);

if ~isnan(range)
  fprintf(2,'Converting for range %g uV [and %g mV]\n',range,auxrange);
  y(1:60,:) = y(1:60,:) * range/2048;
  y(61:64,:)= y(61:64,:) * auxrange/2048;
end
