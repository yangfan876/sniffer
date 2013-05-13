#############################
#	Makefile for sniffer	#
#############################

target = sniffer.o resolve_parameter.o


sniffer: $(target)
	gcc -o $@ $(target)

sniffer.o: sniffer.c
	gcc -g -c -o $@ $<

resolve_parameter.o: resolve_parameter.c
	gcc -g -c -o $@ $<

.PHONY:	clean
clean:
	-rm sniffer $(target)

