prefer_column_vectors = 0;
do_fortran_indexing = 1;
a = [9,8;7,6];
all (a(logical ([1,1]),logical (0:1)) == [8;6])
