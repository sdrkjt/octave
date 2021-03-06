## Copyright (C) 2000 Paul Kienzle
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
## @deftypefn {Function File} {} strjust (@var{s}, ["left"|"right"|"center"])
## Shift the non-blank text of @var{s} to the left, right or center of
## the string.  If @var{s} is a string array, justify each string in the
## array.  Null characters are replaced by blanks.  If no justification
## is specified, then all rows are right-justified.
## @end deftypefn

function x = strjust (x, just)

  if (nargin < 1 || nargin > 2)
    usage ("strjust (s, ['left'|'right'|'center'])");
  endif

  if (nargin == 1)
    just = "right";
  endif

  just = tolower (just);

  dfi = do_fortran_indexing;
  unwind_protect
    do_fortran_indexing = 1;

    ## convert nulls to blanks
    idx = find (toascii (x) == 0);
    if (! isempty (idx))
      x(idx) = " ";
    endif

    ## For all cases, left, right and center, the algorithm is the same.
    ## Find the number of blanks at the left/right end to determine the
    ## shift, rotate the row index by using mod with that shift, then
    ## translate the shifted row index into an array index.
    [nr, nc] = size (x);
    idx = (x' != " ");
    if (strcmp (just, "right"))
      [N, hi] = max (cumsum (idx));
      shift = hi;
    elseif (strcmp (just, "left"))
      [N, lo] = max (cumsum (flipud (idx)));
      shift = (nc - lo);
    else
      [N, hi] = max (cumsum (idx));
      [N, lo] = max (cumsum (flipud (idx)));
      shift = ceil (nc - (lo-hi)/2);
    endif
    idx = rem (ones(nr,1)*[0:nc-1] + shift'*ones(1,nc), nc);
    x = x (idx*nr + [1:nr]'*ones(1,nc));

  unwind_protect_cleanup
    do_fortran_indexing = dfi;
  end_unwind_protect

endfunction
