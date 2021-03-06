## Copyright (C) 1995, 1996  Kurt Hornik
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
## @deftypefn {Mapping Function} {} bincoeff (@var{n}, @var{k})
## Return the binomial coefficient of @var{n} and @var{k}, defined as
## @iftex
## @tex
## $$
##  {n \choose k} = {n (n-1) (n-2) \cdots (n-k+1) \over k!}
## $$
## @end tex
## @end iftex
## @ifinfo
##
## @example
## @group
##  /   \
##  | n |    n (n-1) (n-2) ... (n-k+1)
##  |   |  = -------------------------
##  | k |               k!
##  \   /
## @end group
## @end example
## @end ifinfo
##
## For example,
##
## @example
## @group
## bincoeff (5, 2)
##      @result{} 10
## @end group
## @end example
## @end deftypefn

## Author: KH <Kurt.Hornik@ci.tuwien.ac.at>
## Created: 8 October 1994
## Adapted-By: jwe

function b = bincoeff (n, k)

  if (nargin != 2)
    usage ("bincoeff (n, k)");
  endif

  [retval, n, k] = common_size (n, k);
  if (retval > 0)
    error ("bincoeff: n and k must be of common size or scalars");
  endif

  [r, c] = size (n);
  s = r * c;
  n   = reshape (n, s, 1);
  k   = reshape (k, s, 1);
  b   = zeros (s, 1);

  ind = find (! (k >= 0) | (k != real (round (k))) | isnan (n));
  if (any (ind))
    b(ind) = NaN * ones (length (ind), 1);
  endif

  ind = find (k == 0);
  if (any (ind))
    b(ind) = ones (length (ind), 1);
  endif

  ind = find ((k > 0) & ((n == real (round (n))) & (n < 0)));
  if any (ind)
    b(ind) = (-1) .^ k(ind) .* exp (lgamma (abs (n(ind)) + k(ind)) ...
        - lgamma (k(ind) + 1) - lgamma (abs (n(ind))));
  endif

  ind = find ((k > 0) & ((n != real (round (n))) | (n >= k)));
  if (length (ind) > 0)
    b(ind) = exp (lgamma (n(ind) + 1) - lgamma (k(ind) + 1) ...
        - lgamma (n(ind) - k(ind) + 1));
  endif

  ## clean up rounding errors
  ind = find (n == round (n));
  if (any (ind))
    b(ind) = round (b(ind));
  endif

  b = reshape (b, r, c);

endfunction

