(all (cumprod ([2, 3; 4, 5], 1) == [2, 3; 8, 15])
 && all (cumprod ([2, 3; 4, 5], 2) == [2, 6; 4, 20]))
