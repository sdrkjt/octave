rt2 = sqrt (2);
rt3 = sqrt (3);
x = [pi/6, pi/4, pi/3, pi/2, 2*pi/3, 3*pi/4, 5*pi/6];
v = [rt3, 1, rt3/3, 0, -rt3/3, -1, -rt3];
all (abs (cot (x) - v) < sqrt (eps))
