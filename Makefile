#############################
#	Makefile for sniffer	#
#############################

target = sniffer.o resolve_parameter.o thread_cheat.o netlib.o


sniffer: $(target)
	gcc -o $@ $(target) -lpthread

sniffer.o: sniffer.c
	gcc -g -c -o $@ $<

resolve_parameter.o: resolve_parameter.c
	gcc -g -c -o $@ $<

thread_cheat.o: thread_cheat.c
	gcc -g -c -o $@ $< 

netlib.o: netlib.c
	gcc -g -c -o $@ $<

.PHONY:	clean
clean:
	-rm -f sniffer $(target)

