prefer_column_vectors = 1;
do_fortran_indexing = 0;
a = [9,8;7,6];
all (a(logical (0:1),logical ([1,1])) == [7,6])
