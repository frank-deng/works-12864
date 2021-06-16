target=pic12864.exe

objs=main.obj bmp.obj
libs=

CC=cl /W3 /c
LD=link /machine:i386 /out:

$(target) : $(objs)
	$(LD)$(target) $(objs) $(libs)

main.obj:main.c bmp.h help.h
	$(CC) main.c
bmp.obj:bmp.c bmp.h
	$(CC) bmp.c

.PHONY:clean
clean:
	-del $(objs) $(target)
