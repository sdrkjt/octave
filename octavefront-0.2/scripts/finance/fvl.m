## Copyright (C) 1995, 1996, 1997  Kurt Hornik
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
##
## This program is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this file.  If not, write to the Free Software Foundation,
## 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

## -*- texinfo -*-
## @deftypefn {Function File} {} fvl (@var{r}, @var{n}, @var{l})
## Return the future value at the end of @var{n} periods of an initial
## lump sum investment @var{l}, given a per-period interest rate
## @var{r}.
##
## Note that the rate @var{r} is specified as a fraction (i.e., 0.05,
## not 5 percent).
## @end deftypefn

## Author: KH <Kurt.Hornik@ci.tuwien.ac.at>
## Description: Future value of an initial lump sum investment

function v = fvl (r, n, l)

  if (nargin != 3)
    usage ("fvl (r, n, l)");
  endif

  if (! (is_scalar (r) && (r > -1)))
    error ("fvl: r has to be a scalar > -1");
  elseif (! (is_scalar (n) && (n > 0)))
    error ("fvl: n has to be a positive scalar");
  elseif (! is_scalar (l))
    error ("fvl: l has to be a scalar");
  endif

  v = l * (1 + r)^n;

endfunction