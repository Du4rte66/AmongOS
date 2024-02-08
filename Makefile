CC=gcc
CFLAGS=-Wall -std=c99 -pedantic
EXEC=pc
SRC=$(wildcard Code/*.c)
OBJ=$(SRC:.c=.o)
DIRSRC=Code
DIROBJ=obj

all : $(EXEC)

pc :obj/sha256.o obj/sha256_utils.o obj/timestamp.o $(OBJ)
	$(CC) $^ -o $@
	mv $(DIRSRC)/*.o $(DIROBJ)

install : installeur/install.o obj/sha256.o obj/sha256_utils.o obj/timestamp.o Code/couche_4.o Code/couche_3.o Code/couche_2.o Code/couche_1.o
	$(CC) $^ -o installeur/$@
	mv $(DIRSRC)/*.o $(DIROBJ)
	mv installeur/install.o $(DIROBJ)

couche_1.o : Code/couche_1.h Utils/utils.h
couche_2.o : Code/couche_2.h Code/couche_1.h Utils/utils.h
couche_3.o : Code/couche_3.h Code/couche_1.h Utils/utils.h
couche_4.o : Code/couche_4.h Code/couche_3.h Code/couche_2.h Code/couche_1.h Utils/utils.h Utils/timestamp.h
couche_5.o : Code/couche_5.h Code/couche_4.h Code/couche_3.h Code/couche_2.h Code/couche_1.h ../Utils/utils.h
turnOn.o : Utils/utils.h Code/couche_1.h Code/couche_2.h Code/couche_3.h Code/couche_4.h Code/couche_5.h

%.o : $(DIROBJ)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

install.o : Utils/Sha256/sha256.h Utils/Sha256/sha256_utils.h Utils/timestamp.h Code/couche_4.h Code/couche_3.h Code/couche_2.h Code/couche_1.h
	$(CC) $(CFLAGS) -o $@ -c installeur/install.c


# %.o : Utils/Sha256/%.c
# 	$(CC) -o $@ -c $<
#
# timestamp.o : Utils/timestamp.c
# 	$(CC) -o $@ -c $<

clean:
	rm -rf *.o $(EXEC) *.txt $(DIROBJ)/*.o $(DIRSRC)/*.o
