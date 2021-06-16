target=gxm12864.hex
target_ihx=gxm12864.ihx
objs=gxm12864.rel com.rel main.rel
CC=sdcc
CFLAGS=--model-large

$(target) : $(target_ihx)
	packihx $^ > $@

$(target_ihx) : $(objs)
	$(CC) $(CFLAGS) -o $@ $^

gxm12864.rel: gxm12864.c
	$(CC) $(CFLAGS) -o $@ -c $<

com.rel: com.c
	$(CC) $(CFLAGS) -o $@ -c $<

main.rel: main.c
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY:clean
clean:
	-rm $(target) $(target_ihx) *.asm *.lst *.rel *.sym *.map *.lk *.hex *.mem *.rst

.PHONY:deploy
deploy:
	-rm /mnt/shared/$(target)
	-cp $(target) /mnt/shared/

