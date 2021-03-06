## Copyright (C) 2000 Daniel Calvelo
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
## @deftypefn {Function File} {} base2dec (@var{s}, @var{b})
## Convert @var{s} from a string of digits of base @var{b} into an
## integer.
##
## @example
## base2dec ("11120", 3)
##      @result{} 123
## @end example
##
## If @var{s} is a matrix, returns a column vector with one value per
## row of @var{s}.  If a row contains invalid symbols then the
## corresponding value will be NaN.  Rows are right-justified before
## converting so that trailing spaces are ignored.
##
## If @var{b} is a string, the characters of @var{b} are used as the
## symbols for the digits of @var{s}. Space (' ') may not be used as a
## symbol.
##
## @example
## base2dec ("yyyzx", "xyz")
##      @result{} 123
## @end example
##
## @end deftypefn
## @seealso{dec2base, dec2bin, bin2dec, hex2dec, dec2hex}

## Author: Daniel Calvelo <dcalvelo@yahoo.com>
## Adapted-by: Paul Kienzle <pkienzle@kienzle.powernet.co.uk>

function out = base2dec (d, base)

  if (nargin != 2)
    usage ("n = base2dec('d', base)");
  endif

  symbols = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  if (isstr (base))
    symbols = base;
    base = length (symbols);
    if (any (diff (sort (toascii (symbols))) == 0))
      error ("base2dec: symbols representing digits must be unique.");
    endif
  elseif (! is_scalar (base))
    error ("base2dec: cannot convert from several bases at once.");
  elseif (base < 2 || base > length (symbols))
    error ("base2dec: base must be between 2 and 36 or a string of symbols");
  else
    d = toupper (d);
  endif

  ## Right justify the values before anything else.
  d = strjust (d, "right");

  ## Lookup value of symbols in symbol table, with invalid symbols
  ## evaluating to NaN and space evaluating to 0.
  table = NaN * ones (256, 1);
  table (toascii (symbols (1 : base))) = 0 : base-1;
  table (toascii (" ")) = 0;
  d = reshape (table (toascii (d)), size (d));

  ## Multiply the resulting digits by the appropriate power and
  ## sum the rows.
  out = d * (base .^ (columns(d)-1 : -1 : 0)');

endfunction
