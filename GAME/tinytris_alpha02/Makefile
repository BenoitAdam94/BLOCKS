PROG = hello
CC = ./gbdk-n/bin/gbdk-n-compile.sh
LD = ./gbdk-n/bin/gbdk-n-link.sh
MR = ./gbdk-n/bin/gbdk-n-make-rom.sh
_OBJ = $(patsubst %.c,%.rel,$(wildcard *.c))

all: build

build: $(PROG).gb

$(PROG).gb: $(_OBJ)
	$(LD) -o $(PROG).ihx $^
	$(MR) $(PROG).ihx $(PROG).gb

%.rel: %.c
	$(CC) -o $@ -c $<

gbdk-n-lib:
	test -d gbdk-n || git clone https://github.com/flozz/gbdk-n.git
	cd ./gbdk-n/ && make

clean:
	rm -rf *.rel
	rm -rf *.lst
	rm -rf *.sym
	rm -rf *.asm
	rm -rf $(PROG).gb
	rm -rf $(PROG).ihx
	rm -rf $(PROG).map
	rm -rf $(PROG).sym
	rm -rf $(PROG).noi
	rm -rf $(PROG).lk
