lab6: clean traverse_dir.c
# set -e = Exit immediately if a command exits with a non-zero status
# read -p = Read input from user
# > output.txt = Redirect output of main to a file
	@set -e;\
	gcc -o main traverse_dir.c;\
	read -p "Enter directory: " input;\
	./main "$$input";\
	rm -f dir0/hardlink.txt dir0/symlink.txt;\
	./main "$$input" > output.txt

.PHONY: clean

clean:
	rm -f main dir0/hardlink.txt dir0/symlink.txt