d = [1; 2; 3];

d0 = [1, 0, 0;
      0, 2, 0;
      0, 0, 3];

d1 = [0, 1, 0, 0;
      0, 0, 2, 0;
      0, 0, 0, 3;
      0, 0, 0, 0];

d2 = [0, 0, 1, 0, 0;
      0, 0, 0, 2, 0;
      0, 0, 0, 0, 3;
      0, 0, 0, 0, 0;
      0, 0, 0, 0, 0];

dm1 = [0, 0, 0, 0;
       1, 0, 0, 0;
       0, 2, 0, 0;
       0, 0, 3, 0];

dm2 = [0, 0, 0, 0, 0;
       0, 0, 0, 0, 0;
       1, 0, 0, 0, 0;
       0, 2, 0, 0, 0;
       0, 0, 3, 0, 0];

(diag (d) == d0 && diag (d, 1) == d1 && diag (d, 2) == d2
 && diag (d, -1) == dm1 && diag (d, -2) == dm2
 && diag (d0) == d && diag (d1, 1) == d && diag (dm1, -1) == d)
