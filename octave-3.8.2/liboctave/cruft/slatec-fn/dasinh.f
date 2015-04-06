*DECK DASINH
      DOUBLE PRECISION FUNCTION DASINH (X)
C***BEGIN PROLOGUE  DASINH
C***PURPOSE  Compute the arc hyperbolic sine.
C***LIBRARY   SLATEC (FNLIB)
C***CATEGORY  C4C
C***TYPE      DOUBLE PRECISION (ASINH-S, DASINH-D, CASINH-C)
C***KEYWORDS  ARC HYPERBOLIC SINE, ASINH, ELEMENTARY FUNCTIONS, FNLIB,
C             INVERSE HYPERBOLIC SINE
C***AUTHOR  Fullerton, W., (LANL)
C***DESCRIPTION
C
C DASINH(X) calculates the double precision arc hyperbolic
C sine for double precision argument X.
C
C***REFERENCES  (NONE)
C***ROUTINES CALLED  D1MACH, DCSEVL, INITDS
C***REVISION HISTORY  (YYMMDD)
C   770601  DATE WRITTEN
C   890531  Changed all specific intrinsics to generic.  (WRB)
C   890531  REVISION DATE from Version 3.2
C   891214  Prologue converted to Version 4.0 format.  (BAB)
C***END PROLOGUE  DASINH
      DOUBLE PRECISION X, ASNHCS(39), ALN2, SQEPS, XMAX, Y,
     1  DCSEVL, D1MACH
      LOGICAL FIRST
      SAVE ASNHCS, ALN2, NTERMS, XMAX, SQEPS, FIRST
      DATA ASNHCS(  1) / -.1282003991 1738186343 3721273592 68 D+0     /
      DATA ASNHCS(  2) / -.5881176118 9951767565 2117571383 62 D-1     /
      DATA ASNHCS(  3) / +.4727465432 2124815640 7252497560 29 D-2     /
      DATA ASNHCS(  4) / -.4938363162 6536172101 3601747902 73 D-3     /
      DATA ASNHCS(  5) / +.5850620705 8557412287 4948352593 21 D-4     /
      DATA ASNHCS(  6) / -.7466998328 9313681354 7550692171 88 D-5     /
      DATA ASNHCS(  7) / +.1001169358 3558199265 9661920158 12 D-5     /
      DATA ASNHCS(  8) / -.1390354385 8708333608 6164722588 86 D-6     /
      DATA ASNHCS(  9) / +.1982316948 3172793547 3173602371 48 D-7     /
      DATA ASNHCS( 10) / -.2884746841 7848843612 7472728003 17 D-8     /
      DATA ASNHCS( 11) / +.4267296546 7159937953 4575149959 07 D-9     /
      DATA ASNHCS( 12) / -.6397608465 4366357868 7526323096 81 D-10    /
      DATA ASNHCS( 13) / +.9699168608 9064704147 8782931311 79 D-11    /
      DATA ASNHCS( 14) / -.1484427697 2043770830 2466583656 96 D-11    /
      DATA ASNHCS( 15) / +.2290373793 9027447988 0401843789 83 D-12    /
      DATA ASNHCS( 16) / -.3558839513 2732645159 9789426513 10 D-13    /
      DATA ASNHCS( 17) / +.5563969408 0056789953 3745390885 54 D-14    /
      DATA ASNHCS( 18) / -.8746250959 9624678045 6665935201 62 D-15    /
      DATA ASNHCS( 19) / +.1381524884 4526692155 8688022981 29 D-15    /
      DATA ASNHCS( 20) / -.2191668828 2900363984 9551422641 49 D-16    /
      DATA ASNHCS( 21) / +.3490465852 4827565638 3139237068 80 D-17    /
      DATA ASNHCS( 22) / -.5578578840 0895742439 6301570321 06 D-18    /
      DATA ASNHCS( 23) / +.8944514661 7134012551 0508827989 33 D-19    /
      DATA ASNHCS( 24) / -.1438342634 6571317305 5518452394 66 D-19    /
      DATA ASNHCS( 25) / +.2319181187 2169963036 3261446826 66 D-20    /
      DATA ASNHCS( 26) / -.3748700795 3314343674 5706045439 99 D-21    /
      DATA ASNHCS( 27) / +.6073210982 2064279404 5492428800 00 D-22    /
      DATA ASNHCS( 28) / -.9859940276 4633583177 3701734400 00 D-23    /
      DATA ASNHCS( 29) / +.1603921745 2788496315 2326382933 33 D-23    /
      DATA ASNHCS( 30) / -.2613884735 0287686596 7161343999 99 D-24    /
      DATA ASNHCS( 31) / +.4267084960 6857390833 3581653333 33 D-25    /
      DATA ASNHCS( 32) / -.6977021703 9185243299 7307733333 33 D-26    /
      DATA ASNHCS( 33) / +.1142508833 6806858659 8126933333 33 D-26    /
      DATA ASNHCS( 34) / -.1873529207 8860968933 0210133333 33 D-27    /
      DATA ASNHCS( 35) / +.3076358441 4464922794 0659200000 00 D-28    /
      DATA ASNHCS( 36) / -.5057736403 1639824787 0463999999 99 D-29    /
      DATA ASNHCS( 37) / +.8325075471 2689142224 2133333333 33 D-30    /
      DATA ASNHCS( 38) / -.1371845728 2501044163 9253333333 33 D-30    /
      DATA ASNHCS( 39) / +.2262986842 6552784104 1066666666 66 D-31    /
      DATA ALN2 / 0.6931471805 5994530941 7232121458 18D0 /
      DATA FIRST /.TRUE./
C***FIRST EXECUTABLE STATEMENT  DASINH
      IF (FIRST) THEN
         NTERMS = INITDS (ASNHCS, 39, 0.1*REAL(D1MACH(3)) )
         SQEPS = SQRT(D1MACH(3))
         XMAX = 1.0D0/SQEPS
      ENDIF
      FIRST = .FALSE.
C
      Y = ABS(X)
      IF (Y.GT.1.0D0) GO TO 20
C
      DASINH = X
      IF (Y.GT.SQEPS) DASINH = X*(1.0D0 + DCSEVL (2.D0*X*X-1.D0,
     1  ASNHCS, NTERMS) )
      RETURN
 20   IF (Y.LT.XMAX) DASINH = LOG (Y+SQRT(Y*Y+1.D0))
      IF (Y.GE.XMAX) DASINH = ALN2 + LOG(Y)
      DASINH = SIGN (DASINH, X)
      RETURN
C
      END
