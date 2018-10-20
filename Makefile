build: quadtree
	gcc -Wall quadtree.c -o quadtree -g
run: quadtree
	./quadtree
clean: quadtree
	rm -rf quadtree
