## Copyright (C) 2012-2013 Martin Helm
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
## @deftypefn  {Function File} {} shrinkfaces (@var{p}, @var{sf})
## @deftypefnx {Function File} {@var{nfv} =} shrinkfaces (@var{p}, @var{sf})
## @deftypefnx {Function File} {@var{nfv} =} shrinkfaces (@var{fv}, @var{sf})
## @deftypefnx {Function File} {@var{nfv} =} shrinkfaces (@var{f}, @var{v}, @var{sf})
## @deftypefnx {Function File} {[@var{nf}, @var{nv}] =} shrinkfaces (@dots{})
##
## Reduce the faces area for a given patch, structure or explicit faces
## and points matrices by a scale factor @var{sf}.  The structure
## @var{fv} must contain the fields @qcode{"faces"} and @qcode{"vertices"}. 
## If the factor @var{sf} is omitted then a default of 0.3 is used.
##
## Given a patch handle as the first input argument and no output
## parameters, perform the shrinking of the patch faces in place and
## redraw the patch.
##
## If called with one output argument, return a structure with fields
## @qcode{"faces"}, @qcode{"vertices"}, and @qcode{"facevertexcdata"}
## containing the data after shrinking which can then directly be used as an
## input argument for the @code{patch} function.
##
## Performing the shrinking on faces which are not convex can lead to
## undesired results.
##
## For example,
##
## @example
## @group
## [phi r] = meshgrid (linspace (0, 1.5*pi, 16), linspace (1, 2, 4));
## tri = delaunay (phi(:), r(:));
## v = [r(:).*sin(phi(:)) r(:).*cos(phi(:))];
## clf ()
## p = patch ("Faces", tri, "Vertices", v, "FaceColor", "none");
## fv = shrinkfaces (p);
## patch (fv)
## axis equal
## grid on
## @end group
## @end example
##
## @noindent
## draws a triangulated 3/4 circle and the corresponding shrunken
## version.
## @seealso{patch}
## @end deftypefn

## Author: Martin Helm <martin@mhelm.de>

function [nf, nv] = shrinkfaces (varargin)

  if (nargin < 1 || nargin > 3 || nargout > 2)
    print_usage ();
  endif
  
  sf = 0.3;
  p = varargin{1};
  colors = [];

  if (ishandle (p) && nargin < 3)
    faces = get (p, "Faces");
    vertices = get (p, "Vertices");
    colors = get (p, "FaceVertexCData");
    if (nargin == 2)
      sf = varargin{2};
    endif
  elseif (isstruct (p) && nargin < 3)
    faces = p.faces;
    vertices = p.vertices;
    if (isfield (p, "facevertexcdata"))
      colors = p.facevertexcdata;
    endif
    if (nargin == 2)
      sf = varargin{2};
    endif
  elseif (ismatrix (p) && nargin >= 2 && ismatrix (varargin{2}))
    faces = p;
    vertices = varargin{2};
    if (nargin == 3)
      sf = varargin{3};
    endif
  else
    print_usage ();
  endif
  
  if (! isscalar (sf) || sf <= 0)
    error ("shrinkfaces: scale factor must be a positive scalar");
  endif

  n = columns (vertices);
  if (n < 2 || n > 3)
    error ("shrinkfaces: only 2-D and 3-D patches are supported");
  endif

  m = columns (faces);
  if (m < 3)
    error ("shrinkfaces: faces must consist of at least 3 vertices");
  endif

  v = vertices(faces'(:), :);
  if (isempty (colors) || rows (colors) == rows (faces))
    c = colors;
  elseif (rows (colors) == rows (vertices))
    c = colors(faces'(:), :);
  else
    ## Discard inconsistent color data.
    c = [];
  endif
  sv = rows (v);
  ## we have to deal with a probably very large number of vertices, so
  ## use sparse we use as midpoint (1/m, ..., 1/m) in generalized
  ## barycentric coordinates.
  midpoints = full (kron ( speye (sv / m), ones (m, m) / m) * sparse (v));
  v = sqrt (sf) * (v - midpoints) + midpoints;
  f = reshape (1:sv, m, sv / m)';
  
  switch (nargout)
    case 0
      if (ishandle (p))
        ## avoid exceptions
        set (p, "FaceVertexCData", [], "CData", []);
        set (p, "Vertices", v, "Faces", f, "FaceVertexCData", c);
      else
        nf = struct ("faces", f, "vertices", v, "facevertexcdata", c);
      endif
    case 1
      nf = struct ("faces", f, "vertices", v, "facevertexcdata", c);
    case 2
      nf = f;
      nv = v;
  endswitch

endfunction


%!demo
%! clf;
%! faces = [1 2 3; 1 3 4];
%! vertices = [0 0; 1 0; 1 1; 0 1];
%! patch ('Faces', faces, 'Vertices', vertices, 'FaceColor', 'none');
%! fv = shrinkfaces (faces, vertices, 0.25);
%! patch (fv);
%! axis equal;

%!demo
%! clf;
%! faces = [1 2 3 4; 5 6 7 8];
%! vertices = [0 0; 1 0; 2 1; 1 1; 2 0; 3 0; 4 1; 3.5 1];
%! patch ('Faces', faces, 'Vertices', vertices, 'FaceColor', 'none');
%! fv = shrinkfaces (faces, vertices, 0.25);
%! patch (fv);
%! axis equal;
%! grid on;

%!demo
%! clf;
%! faces = [1 2 3 4];
%! vertices = [-1 2; 0 0; 1 2; 0 1];
%! patch ('Faces', faces, 'Vertices', vertices, 'FaceColor', 'none');
%! fv = shrinkfaces (faces, vertices, 0.25);
%! patch (fv);
%! axis equal;
%! grid on;
%! title 'faces which are not convex are clearly not allowed'

%!demo
%! clf;
%! [phi r] = meshgrid (linspace (0, 1.5*pi, 16), linspace (1, 2, 4));
%! tri = delaunay (phi(:), r(:));
%! v = [r(:).*sin(phi(:)) r(:).*cos(phi(:))];
%! p = patch ('Faces', tri, 'Vertices', v, 'FaceColor', 'none');
%! fv = shrinkfaces (p);
%! patch (fv);
%! axis equal;
%! grid on;

%!demo
%! clf;
%! N = 10;  % N intervals per axis
%! [x, y, z] = meshgrid (linspace (-4,4,N+1));
%! val = x.^3 + y.^3 + z.^3;
%! fv = isosurface (x, y, z, val, 3, z);
%!
%! p = patch ('Faces', fv.faces, 'Vertices', fv.vertices, 'FaceVertexCData', ...
%!            fv.facevertexcdata, 'FaceColor', 'interp', 'EdgeColor', 'black');
%! axis equal;
%! view (115, 30);
%! drawnow;
%! shrinkfaces (p, 0.6);

%!shared faces, vertices, nfv, nfv2
%! faces = [1 2 3];
%! vertices = [0 0 0; 1 0 0; 1 1 0];
%! nfv = shrinkfaces (faces, vertices, 0.7);
%! nfv2 = shrinkfaces (nfv, 1/0.7);
%!assert (isfield (nfv, "faces"))
%!assert (isfield (nfv, "vertices"))
%!assert (size (nfv.faces), [1 3])
%!assert (size (nfv.vertices), [3 3])
%!assert (norm (nfv2.vertices - vertices), 0, 2*eps)

