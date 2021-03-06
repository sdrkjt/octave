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

#if !defined (octave_mx_op_defs_h)
#define octave_mx_op_defs_h 1

#include "mx-inlines.cc"

#define BIN_OP_DECL(R, OP, X, Y) \
  extern R OP (const X&, const Y&)

class boolMatrix;

#define CMP_OP_DECL(OP, X, Y) \
  extern boolMatrix OP (const X&, const Y&)

#define BOOL_OP_DECL(OP, X, Y) \
  extern boolMatrix OP (const X&, const Y&)

#define TBM boolMatrix (1, 1, true)
#define FBM boolMatrix (1, 1, false)
#define NBM boolMatrix ()

// vector by scalar operations.

#define VS_BIN_OP_DECLS(R, V, S) \
  BIN_OP_DECL (R, operator +, V, S); \
  BIN_OP_DECL (R, operator -, V, S); \
  BIN_OP_DECL (R, operator *, V, S); \
  BIN_OP_DECL (R, operator /, V, S);

#define VS_BIN_OP(R, F, OP, V, S) \
  R \
  F (const V& v, const S& s) \
  { \
    int len = v.length (); \
 \
    R r (len); \
 \
    for (int i = 0; i < len; i++) \
      r.elem(i) = v.elem(i) OP s; \
 \
    return r; \
  }

#define VS_BIN_OPS(R, V, S) \
  VS_BIN_OP (R, operator +, +, V, S) \
  VS_BIN_OP (R, operator -, -, V, S) \
  VS_BIN_OP (R, operator *, *, V, S) \
  VS_BIN_OP (R, operator /, /, V, S)

#define VS_OP_DECLS(R, V, S) \
  VS_BIN_OP_DECLS(R, V, S)

// scalar by vector by operations.

#define SV_BIN_OP_DECLS(R, S, V) \
  BIN_OP_DECL (R, operator +, S, V); \
  BIN_OP_DECL (R, operator -, S, V); \
  BIN_OP_DECL (R, operator *, S, V); \
  BIN_OP_DECL (R, operator /, S, V);

#define SV_BIN_OP(R, F, OP, S, V) \
  R \
  F (const S& s, const V& v) \
  { \
    int len = v.length (); \
 \
    R r (len); \
 \
    for (int i = 0; i < len; i++) \
      r.elem(i) = s OP v.elem(i); \
 \
    return r; \
  }

#define SV_BIN_OPS(R, S, V) \
  SV_BIN_OP (R, operator +, +, S, V) \
  SV_BIN_OP (R, operator -, -, S, V) \
  SV_BIN_OP (R, operator *, *, S, V) \
  SV_BIN_OP (R, operator /, /, S, V)

#define SV_OP_DECLS(R, S, V) \
  SV_BIN_OP_DECLS(R, S, V)

// vector by vector operations.

#define VV_BIN_OP_DECLS(R, V1, V2) \
  BIN_OP_DECL (R, operator +, V1, V2); \
  BIN_OP_DECL (R, operator -, V1, V2); \
  BIN_OP_DECL (R, product,    V1, V2); \
  BIN_OP_DECL (R, quotient,   V1, V2);

#define VV_BIN_OP(R, F, OP, V1, V2) \
  R \
  F (const V1& v1, const V2& v2) \
  { \
    R r; \
 \
    int v1_len = v1.length (); \
    int v2_len = v2.length (); \
 \
    if (v1_len != v2_len) \
      gripe_nonconformant (#OP, v1_len, v2_len); \
    else \
      { \
	r.resize (v1_len); \
 \
	for (int i = 0; i < v1_len; i++) \
	  r.elem(i) = v1.elem(i) OP v2.elem(i); \
      } \
 \
    return r; \
  }

#define VV_BIN_OPS(R, V1, V2) \
  VV_BIN_OP (R, operator +, +, V1, V2) \
  VV_BIN_OP (R, operator -, -, V1, V2) \
  VV_BIN_OP (R, product,    *, V1, V2) \
  VV_BIN_OP (R, quotient,   /, V1, V2)

#define VV_OP_DECLS(R, V1, V2) \
  VV_BIN_OP_DECLS(R, V1, V2)

// matrix by scalar operations.

#define MS_BIN_OP_DECLS(R, M, S) \
  BIN_OP_DECL (R, operator +, M, S); \
  BIN_OP_DECL (R, operator -, M, S); \
  BIN_OP_DECL (R, operator *, M, S); \
  BIN_OP_DECL (R, operator /, M, S);

#define MS_BIN_OP(R, OP, M, S, F) \
  R \
  OP (const M& m, const S& s) \
  { \
    int nr = m.rows (); \
    int nc = m.cols (); \
 \
    R r (nr, nc); \
 \
    if (nr > 0 && nc > 0) \
      F ## _vs (r.fortran_vec (), m.data (), nr * nc, s); \
 \
    return r; \
  }

#define MS_BIN_OPS(R, M, S) \
  MS_BIN_OP (R, operator +, M, S, mx_inline_add) \
  MS_BIN_OP (R, operator -, M, S, mx_inline_subtract) \
  MS_BIN_OP (R, operator *, M, S, mx_inline_multiply) \
  MS_BIN_OP (R, operator /, M, S, mx_inline_divide)

#define MS_CMP_OP_DECLS(M, S) \
  CMP_OP_DECL (mx_el_lt, M, S); \
  CMP_OP_DECL (mx_el_le, M, S); \
  CMP_OP_DECL (mx_el_ge, M, S); \
  CMP_OP_DECL (mx_el_gt, M, S); \
  CMP_OP_DECL (mx_el_eq, M, S); \
  CMP_OP_DECL (mx_el_ne, M, S);

#define MS_CMP_OP(F, OP, M, MC, S, SC, EMPTY_RESULT) \
  boolMatrix \
  F (const M& m, const S& s) \
  { \
    boolMatrix r; \
 \
    int nr = m.rows (); \
    int nc = m.cols (); \
 \
    if (nr == 0 || nc == 0) \
      r = EMPTY_RESULT; \
    else \
      { \
        r.resize (nr, nc); \
 \
        for (int j = 0; j < nc; j++) \
          for (int i = 0; i < nr; i++) \
	    r.elem(i, j) = MC (m.elem(i, j)) OP SC (s); \
      } \
 \
    return r; \
  }

#define MS_CMP_OPS(M, CM, S, CS) \
  MS_CMP_OP (mx_el_lt, <,  M, CM, S, CS, NBM) \
  MS_CMP_OP (mx_el_le, <=, M, CM, S, CS, NBM) \
  MS_CMP_OP (mx_el_ge, >=, M, CM, S, CS, NBM) \
  MS_CMP_OP (mx_el_gt, >,  M, CM, S, CS, NBM) \
  MS_CMP_OP (mx_el_eq, ==, M,   , S,   , FBM) \
  MS_CMP_OP (mx_el_ne, !=, M,   , S,   , TBM)

#define MS_BOOL_OP_DECLS(M, S) \
  BOOL_OP_DECL (mx_el_and, M, S); \
  BOOL_OP_DECL (mx_el_or,  M, S); \

#define MS_BOOL_OP(F, OP, M, S, ZERO) \
  boolMatrix \
  F (const M& m, const S& s) \
  { \
    boolMatrix r; \
 \
    int nr = m.rows (); \
    int nc = m.cols (); \
 \
    if (nr != 0 && nc != 0) \
      { \
        r.resize (nr, nc); \
 \
        for (int j = 0; j < nc; j++) \
          for (int i = 0; i < nr; i++) \
	    r.elem(i, j) = (m.elem(i, j) != ZERO) OP (s != ZERO); \
      } \
 \
    return r; \
  }

#define MS_BOOL_OPS(M, S, ZERO) \
  MS_BOOL_OP (mx_el_and, &&, M, S, ZERO) \
  MS_BOOL_OP (mx_el_or,  ||, M, S, ZERO)

#define MS_OP_DECLS(R, M, S) \
  MS_BIN_OP_DECLS (R, M, S) \
  MS_CMP_OP_DECLS (M, S) \
  MS_BOOL_OP_DECLS (M, S) \

// scalar by matrix operations.

#define SM_BIN_OP_DECLS(R, S, M) \
  BIN_OP_DECL (R, operator +, S, M); \
  BIN_OP_DECL (R, operator -, S, M); \
  BIN_OP_DECL (R, operator *, S, M); \
  BIN_OP_DECL (R, operator /, S, M);

#define SM_BIN_OP(R, OP, S, M, F) \
  R \
  OP (const S& s, const M& m) \
  { \
    int nr = m.rows (); \
    int nc = m.cols (); \
 \
    R r (nr, nc); \
 \
    if (nr > 0 && nc > 0) \
      F ## _sv (r.fortran_vec (), s, m.data (), nr * nc); \
 \
    return r; \
  }

#define SM_BIN_OPS(R, S, M) \
  SM_BIN_OP (R, operator +, S, M, mx_inline_add) \
  SM_BIN_OP (R, operator -, S, M, mx_inline_subtract) \
  SM_BIN_OP (R, operator *, S, M, mx_inline_multiply) \
  SM_BIN_OP (R, operator /, S, M, mx_inline_divide)

#define SM_CMP_OP_DECLS(S, M) \
  CMP_OP_DECL (mx_el_lt, S, M); \
  CMP_OP_DECL (mx_el_le, S, M); \
  CMP_OP_DECL (mx_el_ge, S, M); \
  CMP_OP_DECL (mx_el_gt, S, M); \
  CMP_OP_DECL (mx_el_eq, S, M); \
  CMP_OP_DECL (mx_el_ne, S, M);

#define SM_CMP_OP(F, OP, S, SC, M, MC, EMPTY_RESULT) \
  boolMatrix \
  F (const S& s, const M& m) \
  { \
    boolMatrix r; \
 \
    int nr = m.rows (); \
    int nc = m.cols (); \
 \
    if (nr == 0 || nc == 0) \
      r = EMPTY_RESULT; \
    else \
      { \
        r.resize (nr, nc); \
 \
        for (int j = 0; j < nc; j++) \
          for (int i = 0; i < nr; i++) \
	    r.elem(i, j) = SC (s) OP MC (m.elem(i, j)); \
      } \
 \
    return r; \
  }

#define SM_CMP_OPS(S, CS, M, CM) \
  SM_CMP_OP (mx_el_lt, <,  S, CS, M, CM, NBM) \
  SM_CMP_OP (mx_el_le, <=, S, CS, M, CM, NBM) \
  SM_CMP_OP (mx_el_ge, >=, S, CS, M, CM, NBM) \
  SM_CMP_OP (mx_el_gt, >,  S, CS, M, CM, NBM) \
  SM_CMP_OP (mx_el_eq, ==, S,   , M,   , FBM) \
  SM_CMP_OP (mx_el_ne, !=, S,   , M,   , TBM)

#define SM_BOOL_OP_DECLS(S, M) \
  BOOL_OP_DECL (mx_el_and, S, M); \
  BOOL_OP_DECL (mx_el_or,  S, M); \

#define SM_BOOL_OP(F, OP, S, M, ZERO) \
  boolMatrix \
  F (const S& s, const M& m) \
  { \
    boolMatrix r; \
 \
    int nr = m.rows (); \
    int nc = m.cols (); \
 \
    if (nr != 0 && nc != 0) \
      { \
        r.resize (nr, nc); \
 \
        for (int j = 0; j < nc; j++) \
          for (int i = 0; i < nr; i++) \
	    r.elem(i, j) = (s != ZERO) OP (m.elem(i, j) != ZERO); \
      } \
 \
    return r; \
  }

#define SM_BOOL_OPS(S, M, ZERO) \
  SM_BOOL_OP (mx_el_and, &&, S, M, ZERO) \
  SM_BOOL_OP (mx_el_or,  ||, S, M, ZERO)

#define SM_OP_DECLS(R, S, M) \
  SM_BIN_OP_DECLS (R, S, M) \
  SM_CMP_OP_DECLS (S, M) \
  SM_BOOL_OP_DECLS (S, M) \

// matrix by matrix operations.

#define MM_BIN_OP_DECLS(R, M1, M2) \
  BIN_OP_DECL (R, operator +, M1, M2); \
  BIN_OP_DECL (R, operator -, M1, M2); \
  BIN_OP_DECL (R, product,    M1, M2); \
  BIN_OP_DECL (R, quotient,   M1, M2);

#define MM_BIN_OP(R, OP, M1, M2, F) \
  R \
  OP (const M1& m1, const M2& m2) \
  { \
    R r; \
 \
    int m1_nr = m1.rows (); \
    int m1_nc = m1.cols (); \
 \
    int m2_nr = m2.rows (); \
    int m2_nc = m2.cols (); \
 \
    if (m1_nr != m2_nr || m1_nc != m2_nc) \
      gripe_nonconformant (#OP, m1_nr, m1_nc, m2_nr, m2_nc); \
    else \
      { \
	r.resize (m1_nr, m1_nc); \
 \
	if (m1_nr > 0 && m1_nc > 0) \
	  F ## _vv (r.fortran_vec (), m1.data (), m2.data (), m1_nr * m1_nc); \
      } \
 \
    return r; \
  }

#define MM_BIN_OPS(R, M1, M2) \
  MM_BIN_OP (R, operator +, M1, M2, mx_inline_add) \
  MM_BIN_OP (R, operator -, M1, M2, mx_inline_subtract) \
  MM_BIN_OP (R, product,    M1, M2, mx_inline_multiply) \
  MM_BIN_OP (R, quotient,   M1, M2, mx_inline_divide)

#define MM_CMP_OP_DECLS(M1, M2) \
  CMP_OP_DECL (mx_el_lt, M1, M2); \
  CMP_OP_DECL (mx_el_le, M1, M2); \
  CMP_OP_DECL (mx_el_ge, M1, M2); \
  CMP_OP_DECL (mx_el_gt, M1, M2); \
  CMP_OP_DECL (mx_el_eq, M1, M2); \
  CMP_OP_DECL (mx_el_ne, M1, M2);

#define MM_CMP_OP(F, OP, M1, C1, M2, C2, ONE_MT_RESULT, TWO_MT_RESULT) \
  boolMatrix \
  F (const M1& m1, const M2& m2) \
  { \
    boolMatrix r; \
 \
    int m1_nr = m1.rows (); \
    int m1_nc = m1.cols (); \
 \
    int m2_nr = m2.rows (); \
    int m2_nc = m2.cols (); \
 \
    if (m1_nr == m2_nr && m1_nc == m2_nc) \
      { \
	if (m1_nr == 0 && m1_nc == 0) \
	  r = TWO_MT_RESULT; \
	else \
	  { \
	    r.resize (m1_nr, m1_nc); \
 \
	    for (int j = 0; j < m1_nc; j++) \
	      for (int i = 0; i < m1_nr; i++) \
		r.elem(i, j) = C1 (m1.elem(i, j)) OP C2 (m2.elem(i, j)); \
	  } \
      } \
    else \
      { \
	if ((m1_nr == 0 && m1_nc == 0) || (m2_nr == 0 && m2_nc == 0)) \
	  r = ONE_MT_RESULT; \
	else \
	  gripe_nonconformant (#F, m1_nr, m1_nc, m2_nr, m2_nc); \
      } \
 \
    return r; \
  }

#define MM_CMP_OPS(M1, C1, M2, C2) \
  MM_CMP_OP (mx_el_lt, <,  M1, C1, M2, C2, NBM, NBM) \
  MM_CMP_OP (mx_el_le, <=, M1, C1, M2, C2, NBM, NBM) \
  MM_CMP_OP (mx_el_ge, >=, M1, C1, M2, C2, NBM, NBM) \
  MM_CMP_OP (mx_el_gt, >,  M1, C1, M2, C2, NBM, NBM) \
  MM_CMP_OP (mx_el_eq, ==, M1,   , M2,   , FBM, TBM) \
  MM_CMP_OP (mx_el_ne, !=, M1,   , M2,   , TBM, FBM)

#define MM_BOOL_OP_DECLS(M1, M2) \
  BOOL_OP_DECL (mx_el_and, M1, M2); \
  BOOL_OP_DECL (mx_el_or,  M1, M2);

#define MM_BOOL_OP(F, OP, M1, M2, ZERO) \
  boolMatrix \
  F (const M1& m1, const M2& m2) \
  { \
    boolMatrix r; \
 \
    int m1_nr = m1.rows (); \
    int m1_nc = m1.cols (); \
 \
    int m2_nr = m2.rows (); \
    int m2_nc = m2.cols (); \
 \
    if (m1_nr == m2_nr && m1_nc == m2_nc) \
      { \
	if (m1_nr != 0 || m1_nc != 0) \
	  { \
	    r.resize (m1_nr, m1_nc); \
 \
	    for (int j = 0; j < m1_nc; j++) \
	      for (int i = 0; i < m1_nr; i++) \
		r.elem(i, j) = (m1.elem(i, j) != ZERO) \
                                OP (m2.elem(i, j) != ZERO); \
	  } \
      } \
    else \
      { \
	if ((m1_nr != 0 || m1_nc != 0) && (m2_nr != 0 || m2_nc != 0)) \
	  gripe_nonconformant (#F, m1_nr, m1_nc, m2_nr, m2_nc); \
      } \
 \
    return r; \
  }

#define MM_BOOL_OPS(M1, M2, ZERO) \
  MM_BOOL_OP (mx_el_and, &&, M1, M2, ZERO) \
  MM_BOOL_OP (mx_el_or,  ||, M1, M2, ZERO)

#define MM_OP_DECLS(R, M1, M2) \
  MM_BIN_OP_DECLS (R, M1, M2) \
  MM_CMP_OP_DECLS (M1, M2) \
  MM_BOOL_OP_DECLS (M1, M2)

// scalar by diagonal matrix operations.

#define SDM_BIN_OP_DECLS(R, S, DM) \
  BIN_OP_DECL (R, operator +, S, DM); \
  BIN_OP_DECL (R, operator -, S, DM);

#define SDM_BIN_OP(R, OP, S, DM, OPEQ) \
  R \
  OP (const S& s, const DM& dm) \
  { \
    int nr = dm.rows (); \
    int nc = dm.cols (); \
 \
    R r (nr, nc, s); \
 \
    int len = dm.length (); \
 \
    for (int i = 0; i < len; i++) \
      r.elem(i, i) OPEQ dm.elem(i, i); \
 \
    return r; \
}

#define SDM_BIN_OPS(R, S, DM) \
  SDM_BIN_OP (R, operator +, S, DM, +=) \
  SDM_BIN_OP (R, operator -, S, DM, -=)

#define SDM_OP_DECLS(R, S, DM) \
  SDM_BIN_OP_DECLS(R, S, DM)

// diagonal matrix by scalar operations.

#define DMS_BIN_OP_DECLS(R, DM, S) \
  BIN_OP_DECL (R, operator +, DM, S); \
  BIN_OP_DECL (R, operator -, DM, S);

#define DMS_BIN_OP(R, OP, DM, S, SGN) \
  R \
  OP (const DM& dm, const S& s) \
  { \
    int nr = dm.rows (); \
    int nc = dm.cols (); \
 \
    R r (nr, nc, SGN s); \
 \
    int len = dm.length (); \
 \
    for (int i = 0; i < len; i++) \
      r.elem(i, i) += dm.elem(i, i); \
 \
    return r; \
  }

#define DMS_BIN_OPS(R, DM, S) \
  DMS_BIN_OP (R, operator +, DM, S, ) \
  DMS_BIN_OP (R, operator -, DM, S, -)

#define DMS_OP_DECLS(R, DM, S) \
  DMS_BIN_OP_DECLS(R, DM, S)

// matrix by diagonal matrix operations.

#define MDM_BIN_OP_DECLS(R, M, DM) \
  BIN_OP_DECL (R, operator +, M, DM); \
  BIN_OP_DECL (R, operator -, M, DM); \
  BIN_OP_DECL (R, operator *, M, DM);

#define MDM_BIN_OP(R, OP, M, DM, OPEQ) \
R \
OP (const M& m, const DM& dm) \
{ \
  R r; \
 \
  int m_nr = m.rows (); \
  int m_nc = m.cols (); \
 \
  int dm_nr = dm.rows (); \
  int dm_nc = dm.cols (); \
 \
  if (m_nr != dm_nr || m_nc != dm_nc) \
    gripe_nonconformant (#OP, m_nr, m_nc, dm_nr, dm_nc); \
  else \
    { \
      r.resize (m_nr, m_nc); \
 \
      if (m_nr > 0 && m_nc > 0) \
	{ \
	  r = R (m); \
 \
	  int len = dm.length (); \
 \
	  for (int i = 0; i < len; i++) \
	    r.elem(i, i) OPEQ dm.elem(i, i); \
	} \
    } \
 \
  return r; \
}

#define MDM_MULTIPLY_OP(R, M, DM, ZERO) \
R \
operator * (const M& m, const DM& dm) \
{ \
  R r; \
 \
  int m_nr = m.rows (); \
  int m_nc = m.cols (); \
 \
  int dm_nr = dm.rows (); \
  int dm_nc = dm.cols (); \
 \
  if (m_nc != dm_nr) \
    gripe_nonconformant ("operator *", m_nr, m_nc, dm_nr, dm_nc); \
  else \
    { \
      r.resize (m_nr, dm_nc, ZERO); \
 \
      if (m_nr > 0 && m_nc > 0 && dm_nc > 0) \
	{ \
	  for (int j = 0; j < dm.length (); j++) \
	    { \
	      if (dm.elem(j, j) == 1.0) \
		{ \
		  for (int i = 0; i < m_nr; i++) \
		    r.elem(i, j) = m.elem(i, j); \
		} \
	      else if (dm.elem(j, j) != ZERO) \
		{ \
		  for (int i = 0; i < m_nr; i++) \
		    r.elem(i, j) = dm.elem(j, j) * m.elem(i, j); \
		} \
	    } \
	} \
    } \
 \
  return r; \
}

#define MDM_BIN_OPS(R, M, DM, ZERO) \
  MDM_BIN_OP (R, operator +, M, DM, +=) \
  MDM_BIN_OP (R, operator -, M, DM, -=) \
  MDM_MULTIPLY_OP (R, M, DM, ZERO)

#define MDM_OP_DECLS(R, M, DM) \
  MDM_BIN_OP_DECLS(R, M, DM)

// diagonal matrix by matrix operations.

#define DMM_BIN_OP_DECLS(R, DM, M) \
  BIN_OP_DECL (R, operator +, DM, M); \
  BIN_OP_DECL (R, operator -, DM, M); \
  BIN_OP_DECL (R, operator *, DM, M);

#define DMM_BIN_OP(R, OP, DM, M, OPEQ, PREOP) \
R \
OP (const DM& dm, const M& m) \
{ \
  R r; \
 \
  int dm_nr = dm.rows (); \
  int dm_nc = dm.cols (); \
 \
  int m_nr = m.rows (); \
  int m_nc = m.cols (); \
 \
  if (dm_nr != m_nr || dm_nc != m_nc) \
    gripe_nonconformant (#OP, dm_nr, dm_nc, m_nr, m_nc); \
  else \
    { \
      if (m_nr > 0 && m_nc > 0) \
	{ \
	  r = R (PREOP m); \
 \
	  int len = dm.length (); \
 \
	  for (int i = 0; i < len; i++) \
	    r.elem(i, i) OPEQ dm.elem(i, i); \
	} \
      else \
	r.resize (m_nr, m_nc); \
    } \
 \
  return r; \
}

#define DMM_MULTIPLY_OP(R, DM, M, ZERO) \
R \
operator * (const DM& dm, const M& m) \
{ \
  R r; \
 \
  int dm_nr = dm.rows (); \
  int dm_nc = dm.cols (); \
 \
  int m_nr = m.rows (); \
  int m_nc = m.cols (); \
 \
  if (dm_nc != m_nr) \
    gripe_nonconformant ("operator *", dm_nr, dm_nc, m_nr, m_nc); \
  else \
    { \
      r.resize (dm_nr, m_nc, ZERO); \
 \
      if (dm_nr > 0 && dm_nc > 0 && m_nc > 0) \
	{ \
	  for (int i = 0; i < dm.length (); i++) \
	    { \
	      if (dm.elem(i, i) == 1.0) \
		{ \
		  for (int j = 0; j < m_nc; j++) \
		    r.elem(i, j) = m.elem(i, j); \
		} \
	      else if (dm.elem(i, i) != ZERO) \
		{ \
		  for (int j = 0; j < m_nc; j++) \
		    r.elem(i, j) = dm.elem(i, i) * m.elem(i, j); \
		} \
	    } \
	} \
    } \
 \
  return r; \
}

#define DMM_BIN_OPS(R, DM, M, ZERO) \
  DMM_BIN_OP (R, operator +, DM, M, +=, ) \
  DMM_BIN_OP (R, operator -, DM, M, +=, -) \
  DMM_MULTIPLY_OP (R, DM, M, ZERO)

#define DMM_OP_DECLS(R, DM, M) \
  DMM_BIN_OP_DECLS(R, DM, M)

// diagonal matrix by diagonal matrix operations.

#define DMDM_BIN_OP_DECLS(R, DM1, DM2) \
  BIN_OP_DECL (R, operator +, DM1, DM2); \
  BIN_OP_DECL (R, operator -, DM1, DM2); \
  BIN_OP_DECL (R, product, DM1, DM2);

#define DMDM_BIN_OP(R, OP, DM1, DM2, F) \
  R \
  OP (const DM1& dm1, const DM2& dm2) \
  { \
    R r; \
 \
    int dm1_nr = dm1.rows (); \
    int dm1_nc = dm1.cols (); \
 \
    int dm2_nr = dm2.rows (); \
    int dm2_nc = dm2.cols (); \
 \
    if (dm1_nr != dm2_nr || dm1_nc != dm2_nc) \
      gripe_nonconformant (#OP, dm1_nr, dm1_nc, dm2_nr, dm2_nc); \
    else \
      { \
	r.resize (dm1_nr, dm1_nc); \
 \
	if (dm1_nr > 0 && dm1_nc > 0) \
	  F ## _vv (r.fortran_vec (), dm1.data (), dm2.data (), \
		    dm1_nr * dm2_nc); \
      } \
 \
    return r; \
  }

#define DMDM_BIN_OPS(R, DM1, DM2) \
  DMDM_BIN_OP (R, operator +, DM1, DM2, mx_inline_add) \
  DMDM_BIN_OP (R, operator -, DM1, DM2, mx_inline_subtract) \
  DMDM_BIN_OP (R, product,    DM1, DM2, mx_inline_multiply)

#define DMDM_OP_DECLS(R, DM1, DM2) \
  DMDM_BIN_OP_DECLS (R, DM1, DM2)

#endif

/*
;;; Local Variables: ***
;;; mode: C++ ***
;;; End: ***
*/
