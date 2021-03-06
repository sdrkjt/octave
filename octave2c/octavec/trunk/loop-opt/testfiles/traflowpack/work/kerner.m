function [desc, w, f_f, f_dfdu, f_g] = kerner()
% KERNER    TraFlow-modelsettingsfile for Kerners model

% Copyright (C) 2002 Arthur van Dam, Delft, The Netherlands
% 
% This file is part of TraFlowPACK.
% 
% TraFlowPACK is free software; you can redistribute it and/or modify
% it under the terms of the GNU General Public License as published by
% the Free Software Foundation; either version 2 of the License, or
% (at your option) any later version.
% 
% TraFlowPACK is distributed in the hope that it will be useful,
% but WITHOUT ANY WARRANTY; without even the implied warranty of
% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
% GNU General Public License for more details.
% 
% You should have received a copy of the GNU General Public License
% along with TraFlowPACK; if not, write to the Free Software
% Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
% 
% More info at: http://www.inro.tno.nl/five/traflow/

density  = struct('symbol','\rho', 'name','traffic density');
speed    = struct('symbol','V', 'name','average velocity');
w        = [density; speed];

f_f = 'kerner_f';
f_dfdu = 'kerner_dfdu';
f_g = 'kerner_g';

desc = sprintf('Kerner''s  2-eq model.\nRestricted to one single roadway-lane, one single user-class.');