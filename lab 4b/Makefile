CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

# [CẬP NHẬT] Thêm compare vào all
all: kplc compare

kplc: main.o parser.o scanner.o reader.o charcode.o token.o error.o symtab.o semantics.o debug.o instructions.o codegen.o
	${CC} main.o parser.o scanner.o reader.o charcode.o token.o error.o symtab.o semantics.o debug.o instructions.o codegen.o -o kplc

# [MỚI] Thêm quy tắc biên dịch cho chương trình so sánh
compare: compare.c
	${CC} compare.c -o compare

main.o: main.c
	${CC} ${CFLAGS} main.c

scanner.o: scanner.c
	${CC} ${CFLAGS} scanner.c

parser.o: parser.c
	${CC} ${CFLAGS} parser.c

reader.o: reader.c
	${CC} ${CFLAGS} reader.c

charcode.o: charcode.c
	${CC} ${CFLAGS} charcode.c

token.o: token.c
	${CC} ${CFLAGS} token.c

error.o: error.c
	${CC} ${CFLAGS} error.c

symtab.o: symtab.c
	${CC} ${CFLAGS} symtab.c

semantics.o: semantics.c
	${CC} ${CFLAGS} semantics.c

debug.o: debug.c
	${CC} ${CFLAGS} debug.c

instructions.o: instructions.c
	${CC} ${CFLAGS} instructions.c

codegen.o: codegen.c
	${CC} ${CFLAGS} codegen.c

# [CẬP NHẬT] Xóa thêm kplc và compare khi clean
clean:
	rm -f *.o *~ kplc compare