## Copyright (C) 2012-2013 Rik Wehbring
##
## This file is part of Octave.
##
## Octave is free software; you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 3 of the License, or (at
## your option) any later version.
##
## Octave is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with Octave; see the file COPYING.  If not, see
## <http://www.gnu.org/licenses/>.

## -*- texinfo -*-
## @deftypefn  {Function File} {@var{map} =} lines ()
## @deftypefnx {Function File} {@var{map} =} lines (@var{n})
## Create color colormap.  This colormap is composed of the list of colors
## in the current axes @qcode{"ColorOrder"} property.  The default is blue,
## green, red, cyan, pink, yellow, and gray.
## The argument @var{n} must be a scalar.
## If unspecified, the length of the current colormap, or 64, is used.
## @seealso{colormap}
## @end deftypefn

## PKG_ADD: colormap ("register", "lines");
## PKG_DEL: colormap ("unregister", "lines");

function map = lines (n)

  if (nargin == 0)
    n = rows (colormap);
  elseif (nargin == 1)
    if (! isscalar (n))
      error ("lines: N must be a scalar");
    endif
  else
    print_usage ();
  endif

  if (n == 1)
    map = [0, 0, 1];
  elseif (n > 1)
    C = get (gca, "colororder");
    nr = rows (C);
    map = C(rem (0:(n-1), nr) + 1, :);
  else
    map = zeros (0, 3);
  endif

endfunction


%!demo
%! ## Show the 'lines' colormap as an image
%! image (1:64, linspace (0, 1, 64), repmat ((1:64)', 1, 64));
%! axis ([1, 64, 0, 1], "ticy", "xy");
%! colormap (lines (64));

