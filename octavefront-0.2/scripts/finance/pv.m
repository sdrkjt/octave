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
## @deftypefn {Function File} {} pv (@var{r}, @var{n}, @var{p}, @var{l}, @var{method})
## Returns the present value of an investment that will pay off @var{p} for @var{n}
## consecutive periods, assuming an interest @var{r}.
##
## The optional argument @var{l} may be used to specify an additional
## lump-sum payment made at the end of @var{n} periods.
##
## The optional argument @var{method} may be used to specify whether
## payments are made at the end (@code{"e"}, default) or at the
## beginning (@code{"b"}) of each period.
##
## Note that the rate @var{r} is specified as a fraction (i.e., 0.05,
## not 5 percent).
## @end deftypefn
## @seealso{pmt, nper, rate, and npv}

## Author: KH <Kurt.Hornik@ci.tuwien.ac.at>
## Description: Present value of an investment

function v = pv (r, n, p, l, m)

  if ((nargin < 3) || (nargin > 5))
    usage ("pv (r, n, p, l, method)");
  endif

  if (! (is_scalar (r) && (r > -1)))
    error ("pv: r must be a scalar > -1");
  elseif (! (is_scalar (n) && (n > 0)))
    error ("pv: n must be a positive scalar");
  elseif (! is_scalar (p))
    error ("pv: p must be a scalar");
  endif

  if (r != 0)
    v = p * (1 - (1 + r)^(-n)) / r;
  else
    v = p * n;
  endif

  if (nargin > 3)
    if (nargin == 5)
      if (! isstr (m))
        error ("pv: `method' must be a string");
      endif
    elseif (isstr (l))
      m = l;
      l = 0;
    else
      m = "e";
    endif
    if (strcmp (m, "b"))
      v = v * (1 + r);
    endif
    if (is_scalar (l))
      v = v + pvl (r, n, l);
    else
      error ("pv: l must be a scalar");
    endif
  endif

endfunction

