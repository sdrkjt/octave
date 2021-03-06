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

#if defined (__GNUG__)
#pragma implementation
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "mx-cm-s.h"

#include "gripes.h"
#include "ov.h"
#include "ov-cx-mat.h"
#include "ov-re-mat.h"
#include "ov-scalar.h"
#include "ov-typeinfo.h"
#include "ops.h"
#include "xdiv.h"
#include "xpow.h"

// complex matrix by scalar ops.

DEFBINOP_OP (add, complex_matrix, scalar, +)
DEFBINOP_OP (sub, complex_matrix, scalar, -)
DEFBINOP_OP (mul, complex_matrix, scalar, *)

DEFBINOP (div, complex_matrix, scalar)
{
  CAST_BINOP_ARGS (const octave_complex_matrix&, const octave_scalar&);

  double d = v2.double_value ();

  if (d == 0.0)
    gripe_divide_by_zero ();

  return octave_value (v1.complex_matrix_value () / d);
}

DEFBINOP_FN (pow, complex_matrix, scalar, xpow)

DEFBINOP (ldiv, complex_matrix, scalar)
{
  CAST_BINOP_ARGS (const octave_complex_matrix&, const octave_scalar&);

  ComplexMatrix m1 = v1.complex_matrix_value ();
  Matrix m2 = v2.matrix_value ();

  return octave_value (xleftdiv (m1, m2));
}

DEFBINOP_FN (lt, complex_matrix, scalar, mx_el_lt)
DEFBINOP_FN (le, complex_matrix, scalar, mx_el_le)
DEFBINOP_FN (eq, complex_matrix, scalar, mx_el_eq)
DEFBINOP_FN (ge, complex_matrix, scalar, mx_el_ge)
DEFBINOP_FN (gt, complex_matrix, scalar, mx_el_gt)
DEFBINOP_FN (ne, complex_matrix, scalar, mx_el_ne)

DEFBINOP_OP (el_mul, complex_matrix, scalar, *)

DEFBINOP (el_div, complex_matrix, scalar)
{
  CAST_BINOP_ARGS (const octave_complex_matrix&, const octave_scalar&);

  double d = v2.double_value ();

  if (d == 0.0)
    gripe_divide_by_zero ();

  return octave_value (v1.complex_matrix_value () / d);
}

DEFBINOP_FN (el_pow, complex_matrix, scalar, elem_xpow)

DEFBINOP (el_ldiv, complex_matrix, scalar)
{
  CAST_BINOP_ARGS (const octave_complex_matrix&, const octave_scalar&);

  return x_el_div (v2.double_value (), v1.complex_matrix_value ());
}

DEFBINOP_FN (el_and, complex_matrix, scalar, mx_el_and)
DEFBINOP_FN (el_or, complex_matrix, scalar, mx_el_or)

DEFASSIGNOP_FN (assign, complex_matrix, scalar, assign)

void
install_cm_s_ops (void)
{
  INSTALL_BINOP (op_add, octave_complex_matrix, octave_scalar, add);
  INSTALL_BINOP (op_sub, octave_complex_matrix, octave_scalar, sub);
  INSTALL_BINOP (op_mul, octave_complex_matrix, octave_scalar, mul);
  INSTALL_BINOP (op_div, octave_complex_matrix, octave_scalar, div);
  INSTALL_BINOP (op_pow, octave_complex_matrix, octave_scalar, pow);
  INSTALL_BINOP (op_ldiv, octave_complex_matrix, octave_scalar, ldiv);
  INSTALL_BINOP (op_lt, octave_complex_matrix, octave_scalar, lt);
  INSTALL_BINOP (op_le, octave_complex_matrix, octave_scalar, le);
  INSTALL_BINOP (op_eq, octave_complex_matrix, octave_scalar, eq);
  INSTALL_BINOP (op_ge, octave_complex_matrix, octave_scalar, ge);
  INSTALL_BINOP (op_gt, octave_complex_matrix, octave_scalar, gt);
  INSTALL_BINOP (op_ne, octave_complex_matrix, octave_scalar, ne);
  INSTALL_BINOP (op_el_mul, octave_complex_matrix, octave_scalar, el_mul);
  INSTALL_BINOP (op_el_div, octave_complex_matrix, octave_scalar, el_div);
  INSTALL_BINOP (op_el_pow, octave_complex_matrix, octave_scalar, el_pow);
  INSTALL_BINOP (op_el_ldiv, octave_complex_matrix, octave_scalar, el_ldiv);
  INSTALL_BINOP (op_el_and, octave_complex_matrix, octave_scalar, el_and);
  INSTALL_BINOP (op_el_or, octave_complex_matrix, octave_scalar, el_or);

  INSTALL_ASSIGNOP (op_asn_eq, octave_complex_matrix, octave_scalar, assign);
}

/*
;;; Local Variables: ***
;;; mode: C++ ***
;;; End: ***
*/
