      SUBROUTINE EXCHQZ(NMAX, N, A, B, Z, L, LS1, LS2, EPS, FAIL)
      INTEGER NMAX, N, L, LS1, LS2
      DOUBLE PRECISION A(NMAX,N), B(NMAX,N), Z(NMAX,N), EPS
      LOGICAL FAIL
c modified july 9, 1998 a.s.hodel@eng.auburn.edu:
c     REAL changed to DOUBLE PRECISION
c     calls to AMAX1 changed to call MAX instead.
c     calls to SROT  changed to DROT  (both in BLAS)
c     calls to giv changed to dlartg (LAPACK); required new variable tempr
C*
C* GIVEN THE UPPER TRIANGULAR MATRIX B AND UPPER HESSENBERG MATRIX A
C* WITH CONSECUTIVE LS1XLS1 AND LS2XLS2 DIAGONAL BLOCKS (LS1,LS2.LE.2)
C* STARTING AT ROW/COLUMN L, EXCHQZ PRODUCES EQUIVALENCE TRANSFORMA-
C* TIONS QT AND ZT THAT EXCHANGE THE BLOCKS ALONG WITH THEIR GENERALIZED
C* EIGENVALUES. EXCHQZ REQUIRES THE SUBROUTINES DROT (BLAS) AND GIV.
C* THE PARAMETERS IN THE CALLING SEQUENCE ARE (STARRED PARAMETERS ARE
C* ALTERED BY THE SUBROUTINE):
C*
C*    NMAX     THE FIRST DIMENSION OF A, B AND Z
C*    N        THE ORDER OF A, B AND Z
C*   *A,*B     THE MATRIX PAIR WHOSE BLOCKS ARE TO BE INTERCHANGED
C*   *Z        UPON RETURN THIS ARRAY IS MULTIPLIED BY THE COLUMN
C*             TRANSFORMATION ZT.
C*    L        THE POSITION OF THE BLOCKS
C*    LS1      THE SIZE OF THE FIRST BLOCK
C*    LS2      THE SIZE OF THE SECOND BLOCK
C*    EPS      THE REQUIRED ABSOLUTE ACCURACY OF THE RESULT
C*   *FAIL     A LOGICAL VARIABLE WHICH IS FALSE ON A NORMAL RETURN,
C*             TRUE OTHERWISE.
C*
      INTEGER I, J, L1, L2, L3, LI, LJ, LL, IT1, IT2
      DOUBLE PRECISION U(3,3), D, E, F, G, SA, SB, A11B11, A21B11,
     * A12B22, B12B22,
     * A22B22, AMMBMM, ANMBMM, AMNBNN, BMNBNN, ANNBNN, TEMPR
      LOGICAL ALTB
      FAIL = .FALSE.
      L1 = L + 1
      LL = LS1 + LS2
      IF (LL.GT.2) GO TO 10
C*** INTERCHANGE 1X1 AND 1X1 BLOCKS VIA AN EQUIVALENCE
C*** TRANSFORMATION       A:=Q*A*Z , B:=Q*B*Z
C*** WHERE Q AND Z ARE GIVENS ROTATIONS
      F = MAX(ABS(A(L1,L1)),ABS(B(L1,L1)))
      ALTB = .TRUE.
      IF (ABS(A(L1,L1)).GE.F) ALTB = .FALSE.
      SA = A(L1,L1)/F
      SB = B(L1,L1)/F
      F = SA*B(L,L) - SB*A(L,L)
C* CONSTRUCT THE COLUMN TRANSFORMATION Z
      G = SA*B(L,L1) - SB*A(L,L1)
      CALL DLARTG(F, G, D, E,TEMPR)
      CALL DROT(L1, A(1,L), 1, A(1,L1), 1, E, -D)
      CALL DROT(L1, B(1,L), 1, B(1,L1), 1, E, -D)
      CALL DROT(N, Z(1,L), 1, Z(1,L1), 1, E, -D)
C* CONSTRUCT THE ROW TRANSFORMATION Q
      IF (ALTB) CALL DLARTG(B(L,L), B(L1,L), D, E,TEMPR)
      IF (.NOT.ALTB) CALL DLARTG(A(L,L), A(L1,L), D, E,TEMPR)
      CALL DROT(N-L+1, A(L,L), NMAX, A(L1,L), NMAX, D, E)
      CALL DROT(N-L+1, B(L,L), NMAX, B(L1,L), NMAX, D, E)
      A(L1,L) = 0.
      B(L1,L) = 0.
      RETURN
C*** INTERCHANGE 1X1 AND 2X2 BLOCKS VIA AN EQUIVALENCE
C*** TRANSFORMATION  A:=Q2*Q1*A*Z1*Z2 , B:=Q2*Q1*B*Z1*Z2
C*** WHERE EACH QI AND ZI IS A GIVENS ROTATION
   10 L2 = L + 2
      IF (LS1.EQ.2) GO TO 60
      G = MAX(ABS(A(L,L)),ABS(B(L,L)))
      ALTB = .TRUE.
      IF (ABS(A(L,L)).LT.G) GO TO 20
      ALTB = .FALSE.
      CALL DLARTG(A(L1,L1), A(L2,L1), D, E,TEMPR)
      CALL DROT(N-L, A(L1,L1), NMAX, A(L2,L1), NMAX, D, E)
      CALL DROT(N-L, B(L1,L1), NMAX, B(L2,L1), NMAX, D, E)
C**  EVALUATE THE PENCIL AT THE EIGENVALUE CORRESPONDING
C**  TO THE 1X1 BLOCK
   20 SA = A(L,L)/G
      SB = B(L,L)/G
      DO 40 J=1,2
        LJ = L + J
        DO 30 I=1,3
          LI = L + I - 1
          U(I,J) = SA*B(LI,LJ) - SB*A(LI,LJ)
   30   CONTINUE
   40 CONTINUE
      CALL DLARTG(U(3,1), U(3,2), D, E,TEMPR)
      CALL DROT(3, U(1,1), 1, U(1,2), 1, E, -D)
C* PERFORM THE ROW TRANSFORMATION Q1
      CALL DLARTG(U(1,1), U(2,1), D, E,TEMPR)
      U(2,2) = -U(1,2)*E + U(2,2)*D
      CALL DROT(N-L+1, A(L,L), NMAX, A(L1,L), NMAX, D, E)
      CALL DROT(N-L+1, B(L,L), NMAX, B(L1,L), NMAX, D, E)
C* PERFORM THE COLUMN TRANSFORMATION Z1
      IF (ALTB) CALL DLARTG(B(L1,L), B(L1,L1), D, E,TEMPR)
      IF (.NOT.ALTB) CALL DLARTG(A(L1,L), A(L1,L1), D, E,TEMPR)
      CALL DROT(L2, A(1,L), 1, A(1,L1), 1, E, -D)
      CALL DROT(L2, B(1,L), 1, B(1,L1), 1, E, -D)
      CALL DROT(N, Z(1,L), 1, Z(1,L1), 1, E, -D)
C* PERFORM THE ROW TRANSFORMATION Q2
      CALL DLARTG(U(2,2), U(3,2), D, E,TEMPR)
      CALL DROT(N-L+1, A(L1,L), NMAX, A(L2,L), NMAX, D, E)
      CALL DROT(N-L+1, B(L1,L), NMAX, B(L2,L), NMAX, D, E)
C* PERFORM THE COLUMN TRANSFORMATION Z2
      IF (ALTB) CALL DLARTG(B(L2,L1), B(L2,L2), D, E,TEMPR)
      IF (.NOT.ALTB) CALL DLARTG(A(L2,L1), A(L2,L2), D, E,TEMPR)
      CALL DROT(L2, A(1,L1), 1, A(1,L2), 1, E, -D)
      CALL DROT(L2, B(1,L1), 1, B(1,L2), 1, E, -D)
      CALL DROT(N, Z(1,L1), 1, Z(1,L2), 1, E, -D)
      IF (ALTB) GO TO 50
      CALL DLARTG(B(L,L), B(L1,L), D, E,TEMPR)
      CALL DROT(N-L+1, A(L,L), NMAX, A(L1,L), NMAX, D, E)
      CALL DROT(N-L+1, B(L,L), NMAX, B(L1,L), NMAX, D, E)
C*  PUT THE NEGLECTABLE ELEMENTS EQUAL TO ZERO
   50 A(L2,L) = 0.
      A(L2,L1) = 0.
      B(L1,L) = 0.
      B(L2,L) = 0.
      B(L2,L1) = 0.
      RETURN
C*** INTERCHANGE 2X2 AND 1X1 BLOCKS VIA AN EQUIVALENCE
C*** TRANSFORMATION  A:=Q2*Q1*A*Z1*Z2 , B:=Q2*Q1*B*Z1*Z2
C*** WHERE EACH QI AND ZI IS A GIVENS ROTATION
   60 IF (LS2.EQ.2) GO TO 110
      G = MAX(ABS(A(L2,L2)),ABS(B(L2,L2)))
      ALTB = .TRUE.
      IF (ABS(A(L2,L2)).LT.G) GO TO 70
      ALTB = .FALSE.
      CALL DLARTG(A(L,L), A(L1,L), D, E,TEMPR)
      CALL DROT(N-L+1, A(L,L), NMAX, A(L1,L), NMAX, D, E)
      CALL DROT(N-L+1, B(L,L), NMAX, B(L1,L), NMAX, D, E)
C**  EVALUATE THE PENCIL AT THE EIGENVALUE CORRESPONDING
C**  TO THE 1X1 BLOCK
   70 SA = A(L2,L2)/G
      SB = B(L2,L2)/G
      DO 90 I=1,2
        LI = L + I - 1
        DO 80 J=1,3
          LJ = L + J - 1
          U(I,J) = SA*B(LI,LJ) - SB*A(LI,LJ)
   80   CONTINUE
   90 CONTINUE
      CALL DLARTG(U(1,1), U(2,1), D, E,TEMPR)
      CALL DROT(3, U(1,1), 3, U(2,1), 3, D, E)
C* PERFORM THE COLUMN TRANSFORMATION Z1
      CALL DLARTG(U(2,2), U(2,3), D, E,TEMPR)
      U(1,2) = U(1,2)*E - U(1,3)*D
      CALL DROT(L2, A(1,L1), 1, A(1,L2), 1, E, -D)
      CALL DROT(L2, B(1,L1), 1, B(1,L2), 1, E, -D)
      CALL DROT(N, Z(1,L1), 1, Z(1,L2), 1, E, -D)
C* PERFORM THE ROW TRANSFORMATION Q1
      IF (ALTB) CALL DLARTG(B(L1,L1), B(L2,L1), D, E,TEMPR)
      IF (.NOT.ALTB) CALL DLARTG(A(L1,L1), A(L2,L1), D, E,TEMPR)
      CALL DROT(N-L+1, A(L1,L), NMAX, A(L2,L), NMAX, D, E)
      CALL DROT(N-L+1, B(L1,L), NMAX, B(L2,L), NMAX, D, E)
C* PERFORM THE COLUMN TRANSFORMATION Z2
      CALL DLARTG(U(1,1), U(1,2), D, E,TEMPR)
      CALL DROT(L2, A(1,L), 1, A(1,L1), 1, E, -D)
      CALL DROT(L2, B(1,L), 1, B(1,L1), 1, E, -D)
      CALL DROT(N, Z(1,L), 1, Z(1,L1), 1, E, -D)
C* PERFORM THE ROW TRANSFORMATION Q2
      IF (ALTB) CALL DLARTG(B(L,L), B(L1,L), D, E,TEMPR)
      IF (.NOT.ALTB) CALL DLARTG(A(L,L), A(L1,L), D, E,TEMPR)
      CALL DROT(N-L+1, A(L,L), NMAX, A(L1,L), NMAX, D, E)
      CALL DROT(N-L+1, B(L,L), NMAX, B(L1,L), NMAX, D, E)
      IF (ALTB) GO TO 100
      CALL DLARTG(B(L1,L1), B(L2,L1), D, E,TEMPR)
      CALL DROT(N-L, A(L1,L1), NMAX, A(L2,L1), NMAX, D, E)
      CALL DROT(N-L, B(L1,L1), NMAX, B(L2,L1), NMAX, D, E)
C*  PUT THE NEGLECTABLE ELEMENTS EQUAL TO ZERO
  100 A(L1,L) = 0.
      A(L2,L) = 0.
      B(L1,L) = 0.
      B(L2,1) = 0.
      B(L2,L1) = 0.
      RETURN
C*** INTERCHANGE 2X2 AND 2X2 BLOCKS VIA A SEQUENCE OF
C*** QZ-STEPS REALIZED BY THE EQUIVALENCE TRANSFORMATIONS
C***          A:=Q5*Q4*Q3*Q2*Q1*A*Z1*Z2*Z3*Z4*Z5
C***          B:=Q5*Q4*Q3*Q2*Q1*B*Z1*Z2*Z3*Z4*Z5
C*** WHERE EACH QI AND ZI IS A GIVENS ROTATION
  110 L3 = L + 3
C* COMPUTE IMPLICIT SHIFT
      AMMBMM = A(L,L)/B(L,L)
      ANMBMM = A(L1,L)/B(L,L)
      AMNBNN = A(L,L1)/B(L1,L1)
      ANNBNN = A(L1,L1)/B(L1,L1)
      BMNBNN = B(L,L1)/B(L1,L1)
      DO 130 IT1=1,3
        U(1,1) = 1.
        U(2,1) = 1.
        U(3,1) = 1.
        DO 120 IT2=1,10
C* PERFORM ROW TRANSFORMATIONS Q1 AND Q2
          CALL DLARTG(U(2,1), U(3,1), D, E,TEMPR)
          CALL DROT(N-L+1, A(L1,L), NMAX, A(L2,L), NMAX, D, E)
          CALL DROT(N-L, B(L1,L1), NMAX, B(L2,L1), NMAX, D, E)
          U(2,1) = D*U(2,1) + E*U(3,1)
          CALL DLARTG(U(1,1), U(2,1), D, E,TEMPR)
          CALL DROT(N-L+1, A(L,L), NMAX, A(L1,L), NMAX, D, E)
          CALL DROT(N-L+1, B(L,L), NMAX, B(L1,L), NMAX, D, E)
C* PERFORM COLUMN TRANSFORMATIONS Z1 AND Z2
          CALL DLARTG(B(L2,L1), B(L2,L2), D, E,TEMPR)
          CALL DROT(L3, A(1,L1), 1, A(1,L2), 1, E, -D)
          CALL DROT(L2, B(1,L1), 1, B(1,L2), 1, E, -D)
          CALL DROT(N, Z(1,L1), 1, Z(1,L2), 1, E, -D)
          CALL DLARTG(B(L1,L), B(L1,L1), D, E,TEMPR)
          CALL DROT(L3, A(1,L), 1, A(1,L1), 1, E, -D)
          CALL DROT(L1, B(1,L), 1, B(1,L1), 1, E, -D)
          CALL DROT(N, Z(1,L), 1, Z(1,L1), 1, E, -D)
C* PERFORM TRANSFORMATIONS Q3,Z3,Q4,Z4,Q5 AND Z5 IN
C* ORDER TO REDUCE THE PENCIL TO HESSENBERG FORM
          CALL DLARTG(A(L2,L), A(L3,L), D, E,TEMPR)
          CALL DROT(N-L+1, A(L2,L), NMAX, A(L3,L), NMAX, D, E)
          CALL DROT(N-L1, B(L2,L2), NMAX, B(L3,L2), NMAX, D, E)
          CALL DLARTG(B(L3,L2), B(L3,L3), D, E,TEMPR)
          CALL DROT(L3, A(1,L2), 1, A(1,L3), 1, E, -D)
          CALL DROT(L3, B(1,L2), 1, B(1,L3), 1, E, -D)
          CALL DROT(N, Z(1,L2), 1, Z(1,L3), 1, E, -D)
          CALL DLARTG(A(L1,L), A(L2,L), D, E,TEMPR)
          CALL DROT(N-L+1, A(L1,L), NMAX, A(L2,L), NMAX, D, E)
          CALL DROT(N-L, B(L1,L1), NMAX, B(L2,L1), NMAX, D, E)
          CALL DLARTG(B(L2,L1), B(L2,L2), D, E,TEMPR)
          CALL DROT(L3, A(1,L1), 1, A(1,L2), 1, E, -D)
          CALL DROT(L2, B(1,L1), 1, B(1,L2), 1, E, -D)
          CALL DROT(N, Z(1,L1), 1, Z(1,L2), 1, E, -D)
          CALL DLARTG(A(L2,L1), A(L3,L1), D, E,TEMPR)
          CALL DROT(N-L, A(L2,L1), NMAX, A(L3,L1), NMAX, D, E)
          CALL DROT(N-L1, B(L2,L2), NMAX, B(L3,L2), NMAX, D, E)
          CALL DLARTG(B(L3,L2), B(L3,L3), D, E,TEMPR)
          CALL DROT(L3, A(1,L2), 1, A(1,L3), 1, E, -D)
          CALL DROT(L3, B(1,L2), 1, B(1,L3), 1, E, -D)
          CALL DROT(N, Z(1,L2), 1, Z(1,L3), 1, E, -D)
C* TEST OF CONVERGENCE ON THE ELEMENT SEPARATING THE BLOCKS
          IF (ABS(A(L2,L1)).LE.EPS) GO TO 140
C* COMPUTE A NEW SHIFT IN CASE OF NO CONVERGENCE
          A11B11 = A(L,L)/B(L,L)
          A12B22 = A(L,L1)/B(L1,L1)
          A21B11 = A(L1,L)/B(L,L)
          A22B22 = A(L1,L1)/B(L1,L1)
          B12B22 = B(L,L1)/B(L1,L1)
          U(1,1) = ((AMMBMM-A11B11)*(ANNBNN-A11B11)-AMNBNN*
     *     ANMBMM+ANMBMM*BMNBNN*A11B11)/A21B11 + A12B22 - A11B11*B12B22
          U(2,1) = (A22B22-A11B11) - A21B11*B12B22 - (AMMBMM-A11B11) -
     *     (ANNBNN-A11B11) + ANMBMM*BMNBNN
          U(3,1) = A(L2,L1)/B(L1,L1)
  120   CONTINUE
  130 CONTINUE
      FAIL = .TRUE.
      RETURN
C*  PUT THE NEGLECTABLE ELEMENTS EQUAL TO ZERO IN
C*  CASE OF CONVERGENCE
  140 A(L2,L) = 0.
      A(L2,L1) = 0.
      A(L3,L) = 0.
      A(L3,L1) = 0.
      B(L1,L) = 0.
      B(L2,L) = 0.
      B(L2,L1) = 0.
      B(L3,L) = 0.
      B(L3,L1) = 0.
      B(L3,L2) = 0.
      RETURN
      END
