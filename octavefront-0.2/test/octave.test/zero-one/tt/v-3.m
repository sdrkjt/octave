prefer_column_vectors = 1;
do_fortran_indexing = 1;
a = [9,8,7,6];
all (a(logical ([0,1,1,0])) == [8,7])
