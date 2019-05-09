.PHONY: all
all: kara tc3

.PHONY: kara
kara:
	g++ -o kara kara.cpp

.PHONY: tc3
tc3:
	g++  -o tc3 tc3.cpp


.PHONY: clean
clean:
	rm -f *.o kara tc3 vgcore.* a.out
