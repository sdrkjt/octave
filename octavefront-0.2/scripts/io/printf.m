## Copyright (C) 1996, 1997 John W. Eaton
##
## This file is part of Octave.
##
## Octave is free software; you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
##
## Octave is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with Octave; see the file COPYING.  If not, write to the Free
## Software Foundation, 59 Temple Place - Suite 330, Boston, MA
## 02111-1307, USA.

## -*- texinfo -*-
## @deftypefn {Function File} {} printf (@var{template}, @dots{})
## The @code{printf} function prints the optional arguments under the
## control of the template string @var{template} to the stream
## @code{stdout}.
## @end deftypefn
## @seealso{fprintf and sprintf}

## Author: jwe

function retval = printf (fmt, ...)

  retval = -1;

  if (nargin > 0)
    retval = fprintf (stdout, fmt, all_va_args);
  else
    usage ("printf (fmt, ...)");
  endif

endfunction
