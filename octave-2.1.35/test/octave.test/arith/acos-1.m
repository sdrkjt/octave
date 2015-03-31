rt2 = sqrt (2);
rt3 = sqrt (3);
v = [0, pi/6, pi/4, pi/3, pi/2, 2*pi/3, 3*pi/4, 5*pi/6, pi];
x = [1, rt3/2, rt2/2, 1/2, 0, -1/2, -rt2/2, -rt3/2, -1];
all (abs (acos (x) - v) < sqrt (eps))


