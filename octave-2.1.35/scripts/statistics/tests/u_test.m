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
## @deftypefn {Function File} {[@var{pval}, @var{z}] =} u_test (@var{x}, @var{y}, @var{alt})
## For two samples @var{x} and @var{y}, perform a Mann-Whitney U-test of
## the null hypothesis PROB (@var{x} > @var{y}) == 1/2 == PROB (@var{x}
## < @var{y}).  Under the null, the test statistic @var{z} approximately
## follows a standard normal distribution.  Note that this test is
## equivalent to the Wilcoxon rank-sum test.
##
## With the optional argument string @var{alt}, the alternative of
## interest can be selected.  If @var{alt} is @code{"!="} or
## @code{"<>"}, the null is tested against the two-sided alternative
## PROB (@var{x} > @var{y}) != 1/2.  If @var{alt} is @code{">"}, the
## one-sided alternative PROB (@var{x} > @var{y}) > 1/2 is considered.
## Similarly for @code{"<"}, the one-sided alternative PROB (@var{x} >
## @var{y}) < 1/2 is considered,  The default is the two-sided case.
##
## The p-value of the test is returned in @var{pval}.
##
## If no output argument is given, the p-value of the test is displayed.
## @end deftypefn

## This implementation is still incomplete---for small sample sizes,
## the normal approximation is rather bad ...

## Author: KH <Kurt.Hornik@ci.tuwien.ac.at>
## Description: Mann-Whitney U-test

function [pval, z] = u_test (x, y, alt)

  if ((nargin < 2) || (nargin > 3))
    usage ("[pval, z] = u_test (x, y, alt)");
  endif

  if (! (is_vector (x) && is_vector (y)))
    error ("u_test: both x and y must be vectors");
  endif

  n_x  = length (x);
  n_y  = length (y);
  r    = ranks ([(reshape (x, 1, n_x)), (reshape (y, 1, n_y))]);
  z    = (sum (r(1 : n_x)) - n_x * (n_x + n_y + 1) / 2) ...
           / sqrt (n_x * n_y * (n_x + n_y + 1) / 12);

  cdf  = stdnormal_cdf (z);

  if (nargin == 2)
    alt  = "!=";
  endif

  if (! isstr (alt))
    error("u_test: alt must be a string");
  endif
  if (strcmp (alt, "!=") || strcmp (alt, "<>"))
    pval = 2 * min (cdf, 1 - cdf);
  elseif (strcmp (alt, ">"))
    pval = cdf;
  elseif (strcmp (alt, "<"))
    pval = 1 - cdf;
  else
    error ("u_test: option %s not recognized", alt);
  endif

  if (nargout == 0)
    printf ("  pval: %g\n", pval);
  endif

endfunction
