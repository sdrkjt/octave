prefer_column_vectors = 1;
do_fortran_indexing = 1;
a = [9,8;7,6];
all (a(logical ([0,1]),[2,1]) == [6,7])
