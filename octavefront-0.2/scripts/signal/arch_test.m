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
## @deftypefn {Function File} {} {[@var{pval}, @var{lm}] =} arch_test (@var{y}, @var{x}, @var{p})
## For a linear regression model
##
## @example
## y = x * b + e
## @end example
##
## @noindent
## perform a Lagrange Multiplier (LM) test of the null hypothesis of no
## conditional heteroscedascity against the alternative of CH(@var{p}).
##
## I.e., the model is
##
## @example
## y(t) = b(1) * x(t,1) + @dots{} + b(k) * x(t,k) + e(t),
## @end example
##
## @noindent
## given @var{y} up to @math{t-1} and @var{x} up to @math{t},
## @math{e}(t) is @math{N(0, h(t))} with
##
## @example
## h(t) = v + a(1) * e(t-1)^2 + @dots{} + a(p) * e(t-p)^2,
## @end example
##
## @noindent
## and the null is @math{a(1)} == @dots{} == @math{a(p)} == 0.
##
## If the second argument is a scalar integer, @math{k}, perform the same
## test in a linear autoregression model of order @math{k}, i.e., with
##
## @example
## [1, y(t-1), @dots{}, y(t-@var{k})]
## @end example
##
## @noindent
## as the @math{t}-th row of @var{x}.
##
## Under the null, LM approximately has a chisquare distribution with
## @var{p} degrees of freedom and @var{pval} is the @math{p}-value (1
## minus the CDF of this distribution at LM) of the test.
##
## If no output argument is given, the @math{p}-value is displayed.
## @end deftypefn

## Author: KH <Kurt.Hornik@ci.tuwien.ac.at>
## Description: Test for conditional heteroscedascity

function [pval, lm] = arch_test (y, X, p)

  if (nargin != 3)
    error ("arch_test needs 3 input arguments");
  endif

  if (! (is_vector (y)))
    error ("arch_test: y must be a vector");
  endif
  T   = length (y);
  y   = reshape (y, T, 1);
  [rx, cx] = size (X);
  if ((rx == 1) && (cx == 1))
    X = autoreg_matrix (y, X);
  elseif (! (rx == T))
    error ("arch_test: either rows(X) == length(y), or X is a scalar");
  endif
  if (! (is_scalar(p) && (rem(p, 1) == 0) && (p > 0)))
    error ("arch_test: p must be a positive integer");
  endif

  [b, v_b, e] = ols (y, X);
  Z    = autoreg_matrix (e.^2, p);
  f    = e.^2 / v_b - ones (T, 1);
  f    = Z' * f;
  lm   = f' * inv (Z'*Z) * f / 2;
  pval = 1 - chisquare_cdf (lm, p);

endfunction