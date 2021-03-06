## Copyright (C) 2007-2013 David Bateman
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
## @deftypefn  {Function File} {} meshz (@var{x}, @var{y}, @var{z})
## @deftypefnx {Function File} {} meshz (@var{z})
## @deftypefnx {Function File} {} meshz (@dots{}, @var{c})
## @deftypefnx {Function File} {} meshz (@dots{}, @var{prop}, @var{val}, @dots{})
## @deftypefnx {Function File} {} meshz (@var{hax}, @dots{})
## @deftypefnx {Function File} {@var{h} =} meshz (@dots{})
## Plot a 3-D wireframe mesh with a surrounding curtain.
##
## The wireframe mesh is plotted using rectangles.  The vertices of the
## rectangles [@var{x}, @var{y}] are typically the output of @code{meshgrid}.
## over a 2-D rectangular region in the x-y plane.  @var{z} determines the
## height above the plane of each vertex.  If only a single @var{z} matrix is
## given, then it is plotted over the meshgrid
## @code{@var{x} = 1:columns (@var{z}), @var{y} = 1:rows (@var{z})}.
## Thus, columns of @var{z} correspond to different @var{x} values and rows
## of @var{z} correspond to different @var{y} values.
##
## The color of the mesh is computed by linearly scaling the @var{z} values
## to fit the range of the current colormap.  Use @code{caxis} and/or
## change the colormap to control the appearance.
##
## Optionally the color of the mesh can be specified independently of @var{z}
## by supplying a color matrix, @var{c}.
##
## Any property/value pairs are passed directly to the underlying surface
## object.
##
## If the first argument @var{hax} is an axes handle, then plot into this axis,
## rather than the current axes returned by @code{gca}.
##
## The optional return value @var{h} is a graphics handle to the created
## surface object.
##
## @seealso{mesh, meshc, contour, surf, surface, waterfall, meshgrid, hidden, shading, colormap, caxis}
## @end deftypefn

function h = meshz (varargin)

  if (! all (cellfun ("isreal", varargin)))
    error ("meshz: X, Y, Z, C arguments must be real");
  endif

  [hax, varargin, nargin] = __plt_get_axis_arg__ ("meshz", varargin{:});

  ## Find where property/value pairs start
  charidx = find (cellfun ("isclass", varargin, "char"), 1);

  have_c = false;
  if (isempty (charidx))
    if (nargin == 2 || nargin == 4) 
      have_c = true;
      charidx = nargin;   # bundle C matrix back into varargin 
    else
      charidx = nargin + 1;
    endif
  endif

  if (charidx == 2)
    z = varargin{1};
    [m, n] = size (z);
    x = 1:n;
    y = (1:m).';
  else
    x = varargin{1};
    y = varargin{2};
    z = varargin{3};
  endif

  if (isvector (x) && isvector (y))
    x = [x(1), x(:).', x(end)];
    y = [y(1); y(:); y(end)];
  else
    x = [x(1,1), x(1,:), x(1,end);
         x(:,1), x, x(:,end);
         x(end,1), x(end,:), x(end,end)];
    y = [y(1,1), y(1,:), y(1,end);
         y(:,1), y, y(:,end);
         y(end,1), y(end,:), y(end,end)];
  endif

  zref = min (z(isfinite (z)));
  z = [zref .* ones(1, columns(z) + 2);
       zref .* ones(rows(z), 1), z, zref .* ones(rows(z), 1);
       zref .* ones(1, columns(z) + 2)];

  if (have_c)
    c = varargin{charidx};
    cref = min (c(isfinite (c)));
    c = [cref .* ones(1, columns(c) + 2);
         cref .* ones(rows(c), 1), c, cref .* ones(rows(c), 1);
         cref .* ones(1, columns(c) + 2)];
    varargin(charidx) = c;
  endif
    
  oldfig = [];
  if (! isempty (hax))
    oldfig = get (0, "currentfigure");
  endif
  unwind_protect
    hax = newplot (hax);
    htmp = mesh (x, y, z, varargin{charidx:end});
  unwind_protect_cleanup
    if (! isempty (oldfig))
      set (0, "currentfigure", oldfig);
    endif
  end_unwind_protect

  if (nargout > 0)
    h = htmp;
  endif

endfunction


%!demo
%! clf;
%! colormap ('default');
%! Z = peaks ();
%! meshz (Z);
%! title ('meshz() plot of peaks() function');

%!demo
%! clf;
%! colormap ('default');
%! Z = peaks ();
%! subplot (1,2,1)
%!  mesh (Z);
%!  daspect ([2.5, 2.5, 1]);
%!  title ('mesh() plot');
%! subplot (1,2,2)
%!  meshz (Z);
%!  daspect ([2.5, 2.5, 1]);
%!  title ('meshz() plot');

%!demo
%! clf;
%! colormap ('default');
%! [X,Y,Z] = peaks ();
%! [fx, fy] = gradient (Z); 
%! C = sqrt (fx.^2 + fy.^2);
%! meshz (X,Y,Z,C);
%! title ('meshz() plot with color determined by gradient');

