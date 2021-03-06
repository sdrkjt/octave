/*

Copyright (C) 1996, 1997 John W. Eaton

This file is part of Octave.

Octave is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.

Octave is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with Octave; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#if !defined (octave_dld_function_h)
#define octave_dld_function_h 1

#if defined (__GNUG__)
#pragma interface
#endif

#include <string>

#include "oct-shlib.h"

#include "ov-fcn.h"
#include "ov-builtin.h"
#include "ov-typeinfo.h"

class octave_shlib;

class octave_value;
class octave_value_list;

// Dynamically-linked functions.

class
octave_dld_function : public octave_builtin
{
public:

  octave_dld_function (octave_builtin::fcn ff, const octave_shlib& shl,
		       const std::string& nm = std::string (),
		       const std::string& ds = std::string ());

  ~octave_dld_function (void);

  void mark_fcn_file_up_to_date (const octave_time& t) { t_checked = t; }

  std::string fcn_file_name (void) const;

  octave_time time_parsed (void) const;

  octave_time time_checked (void) const { return t_checked; }

  bool is_system_fcn_file (void) const { return system_fcn_file; }

  bool is_builtin_function (void) const { return false; }

  bool is_dld_function (void) const { return true; }

private:

  octave_dld_function (void);

  octave_dld_function (const octave_dld_function& m);

  octave_shlib sh_lib;

  // The time the file was last checked to see if it needs to be
  // parsed again.
  mutable octave_time t_checked;

  // True if this function came from a file that is considered to be a
  // system function.  This affects whether we check the time stamp
  // on the file to see if it has changed.
  bool system_fcn_file;

  DECLARE_OV_TYPEID_FUNCTIONS_AND_DATA

  DECLARE_OCTAVE_ALLOCATOR
};

#endif

/*
;;; Local Variables: ***
;;; mode: C++ ***
;;; End: ***
*/
