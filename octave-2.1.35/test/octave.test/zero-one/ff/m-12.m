prefer_column_vectors = 0;
do_fortran_indexing = 0;
a = [9,8;7,6];
all (a(2:-1:1,logical ([0,1])) == [6;8])