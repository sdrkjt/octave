x = zeros (3);
x(3,3) = 1;
all (any (x) == [0, 0, 1]) == 1 && all (any (ones (3)) == [1, 1, 1]) == 1
