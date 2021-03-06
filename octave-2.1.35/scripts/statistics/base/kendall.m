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
## @deftypefn {Function File} {} kendall (@var{x}, @var{y})
## Compute Kendall's @var{tau} for each of the variables specified by
## the input arguments.
##
## For matrices, each row is an observation and each column a variable;
## vectors are always observations and may be row or column vectors.
##
## @code{kendall (@var{x})} is equivalent to @code{kendall (@var{x},
## @var{x})}.
##
## For two data vectors @var{x}, @var{y} of common length @var{n},
## Kendall's @var{tau} is the correlation of the signs of all rank
## differences of @var{x} and @var{y};  i.e., if both @var{x} and
## @var{y} have distinct entries, then
##
## @iftex
## @tex
## $$ \tau = {1 \over n(n-1)} \sum_{i,j} {\rm sign}(q_i-q_j) {\rm sign}(r_i-r_j) $$
## @end tex
## @end iftex
## @ifinfo
## @example
##          1    
## tau = -------   SUM sign (q(i) - q(j)) * sign (r(i) - r(j))
##       n (n-1)   i,j
## @end example
## @end ifinfo
##
## @noindent
## in which the
## @iftex
## @tex
## $q_i$ and $r_i$
## @end tex
## @end iftex
## @ifinfo
## @var{q}(@var{i}) and @var{r}(@var{i})
## @end ifinfo
##  are the ranks of
## @var{x} and @var{y}, respectively.
##
## If @var{x} and @var{y} are drawn from independent distributions,
## Kendall's @var{tau} is asymptotically normal with mean 0 and variance
## @code{(2 * (2@var{n}+5)) / (9 * @var{n} * (@var{n}-1))}.
## @end deftypefn

## Author: KH <Kurt.Hornik@ci.tuwien.ac.at>
## Description: Kendall's rank correlation tau

function tau = kendall (x, y)

  if ((nargin < 1) || (nargin > 2))
    usage ("kendall (x, y)");
  endif

  if (rows (x) == 1)
    x = x';
  endif
  [n, c] = size (x);

  if (nargin == 2)
    if (rows (y) == 1)
      y = y';
    endif
    if (rows (y) != n)
      error ("kendall: x and y must have the same number of observations");
    else
      x = [x, y];
    endif
  endif

  r   = ranks (x);
  m   = sign (kron (r, ones (n, 1)) - kron (ones (n, 1), r));
  tau = cor (m);

  if (nargin == 2)
    tau = tau (1 : c, (c + 1) : columns (x));
  endif

endfunction