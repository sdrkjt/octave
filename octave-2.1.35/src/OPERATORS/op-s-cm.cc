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

#include "mx-s-cm.h"
#include "mx-cm-s.h"

#include "gripes.h"
#include "ov.h"
#include "ov-scalar.h"
#include "ov-cx-mat.h"
#include "ov-re-mat.h"
#include "ov-typeinfo.h"
#include "ops.h"
#include "xdiv.h"
#include "xpow.h"

// scalar by complex matrix ops.

DEFBINOP_OP (add, scalar, complex_matrix, +)
DEFBINOP_OP (sub, scalar, complex_matrix, -)
DEFBINOP_OP (mul, scalar, complex_matrix, *)

DEFBINOP (div, scalar, complex_matrix)
{
  CAST_BINOP_ARGS (const octave_scalar&, const octave_complex_matrix&);

  Matrix m1 = v1.matrix_value ();
  ComplexMatrix m2 = v2.complex_matrix_value ();

  return octave_value (xdiv (m1, m2));
}

DEFBINOP_FN (pow, scalar, complex_matrix, xpow)

DEFBINOP (ldiv, scalar, complex_matrix)
{
  CAST_BINOP_ARGS (const octave_scalar&, const octave_complex_matrix&);

  double d = v1.double_value ();

  if (d == 0.0)
    gripe_divide_by_zero ();

  return octave_value (v2.complex_matrix_value () / d);
}

DEFBINOP_FN (lt, scalar, complex_matrix, mx_el_lt)
DEFBINOP_FN (le, scalar, complex_matrix, mx_el_le)
DEFBINOP_FN (eq, scalar, complex_matrix, mx_el_eq)
DEFBINOP_FN (ge, scalar, complex_matrix, mx_el_ge)
DEFBINOP_FN (gt, scalar, complex_matrix, mx_el_gt)
DEFBINOP_FN (ne, scalar, complex_matrix, mx_el_ne)

DEFBINOP_OP (el_mul, scalar, complex_matrix, *)
DEFBINOP_FN (el_div, scalar, complex_matrix, x_el_div)
DEFBINOP_FN (el_pow, scalar, complex_matrix, elem_xpow)

DEFBINOP (el_ldiv, scalar, complex_matrix)
{
  CAST_BINOP_ARGS (const octave_scalar&, const octave_complex_matrix&);

  double d = v1.double_value ();

  if (d == 0.0)
    gripe_divide_by_zero ();

  return octave_value (v2.complex_matrix_value () / d);
}

DEFBINOP (el_and, scalar, complex_matrix)
{
  CAST_BINOP_ARGS (const octave_scalar&, const octave_complex_matrix&);

  return mx_el_and (v1.double_value (), v2.complex_matrix_value ());
}

DEFBINOP (el_or, scalar, complex_matrix)
{
  CAST_BINOP_ARGS (const octave_scalar&, const octave_complex_matrix&);

  return mx_el_or (v1.double_value (), v2.complex_matrix_value ());
}

DEFCONV (complex_matrix_conv, scalar, complex_matrix)
{
  CAST_CONV_ARG (const octave_scalar&);

  return new octave_complex_matrix (ComplexMatrix (v.matrix_value ()));
}

void
install_s_cm_ops (void)
{
  INSTALL_BINOP (op_add, octave_scalar, octave_complex_matrix, add);
  INSTALL_BINOP (op_sub, octave_scalar, octave_complex_matrix, sub);
  INSTALL_BINOP (op_mul, octave_scalar, octave_complex_matrix, mul);
  INSTALL_BINOP (op_div, octave_scalar, octave_complex_matrix, div);
  INSTALL_BINOP (op_pow, octave_scalar, octave_complex_matrix, pow);
  INSTALL_BINOP (op_ldiv, octave_scalar, octave_complex_matrix, ldiv);
  INSTALL_BINOP (op_lt, octave_scalar, octave_complex_matrix, lt);
  INSTALL_BINOP (op_le, octave_scalar, octave_complex_matrix, le);
  INSTALL_BINOP (op_eq, octave_scalar, octave_complex_matrix, eq);
  INSTALL_BINOP (op_ge, octave_scalar, octave_complex_matrix, ge);
  INSTALL_BINOP (op_gt, octave_scalar, octave_complex_matrix, gt);
  INSTALL_BINOP (op_ne, octave_scalar, octave_complex_matrix, ne);
  INSTALL_BINOP (op_el_mul, octave_scalar, octave_complex_matrix, el_mul);
  INSTALL_BINOP (op_el_div, octave_scalar, octave_complex_matrix, el_div);
  INSTALL_BINOP (op_el_pow, octave_scalar, octave_complex_matrix, el_pow);
  INSTALL_BINOP (op_el_ldiv, octave_scalar, octave_complex_matrix, el_ldiv);
  INSTALL_BINOP (op_el_and, octave_scalar, octave_complex_matrix, el_and);
  INSTALL_BINOP (op_el_or, octave_scalar, octave_complex_matrix, el_or);

  INSTALL_ASSIGNCONV (octave_scalar, octave_complex_matrix, octave_complex_matrix);

  INSTALL_WIDENOP (octave_scalar, octave_complex_matrix, complex_matrix_conv);
}

/*
;;; Local Variables: ***
;;; mode: C++ ***
;;; End: ***
*/
