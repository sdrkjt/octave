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

#if !defined (octave_char_matrix_h)
#define octave_char_matrix_h 1

#if defined (__GNUG__)
#pragma interface
#endif

#include <cstdlib>

#include <iostream>
#include <string>

#include "mx-base.h"
#include "oct-alloc.h"
#include "str-vec.h"

#include "error.h"
#include "ov-base.h"
#include "ov-base-mat.h"
#include "ov-typeinfo.h"

class Octave_map;
class octave_value_list;

class tree_walker;

// Character matrix values.

class
octave_char_matrix : public octave_base_matrix<charMatrix>
{
public:

  octave_char_matrix (void)
    : octave_base_matrix<charMatrix> () { }

  octave_char_matrix (const charMatrix& chm, bool = false)
    : octave_base_matrix<charMatrix> (chm) { }

  octave_char_matrix (char c)
    : octave_base_matrix<charMatrix> (c) { }

  octave_char_matrix (const char *s)
    : octave_base_matrix<charMatrix> (s) { }

  octave_char_matrix (const std::string& s)
    : octave_base_matrix<charMatrix> (s) { }

  octave_char_matrix (const string_vector& s)
    : octave_base_matrix<charMatrix> (s) { }

  octave_char_matrix (const octave_char_matrix& chm)
    : octave_base_matrix<charMatrix> (chm) { }

  ~octave_char_matrix (void) { }

  octave_value *clone (void) { return new octave_char_matrix (*this); }

  bool is_char_matrix (void) const { return true; }
  bool is_real_matrix (void) const { return true; }

  bool is_real_type (void) const { return true; }

  bool valid_as_scalar_index (void) const;

  double double_value (bool = false) const;

  double scalar_value (bool frc_str_conv = false) const
    { return double_value (frc_str_conv); }

  Matrix matrix_value (bool = false) const { return Matrix (matrix); }

  Complex complex_value (bool = false) const;

  ComplexMatrix complex_matrix_value (bool = false) const
    { return ComplexMatrix (matrix); }

  charMatrix char_matrix_value (bool = false) const
    { return matrix; }

  octave_value convert_to_str (void) const
    { return octave_value (matrix, true); }

protected:

  DECLARE_OCTAVE_ALLOCATOR

  DECLARE_OV_TYPEID_FUNCTIONS_AND_DATA
};

#endif

/*
;;; Local Variables: ***
;;; mode: C++ ***
;;; End: ***
*/
