# Image-Compression
PROGRAM DE COMPRIMARE, DECOMPRIMARE, OGLINDIRE SI SUPRAPUNERE DE IMAGINI;
Costin Eugen, Seria CC, Grupa 314;
Tema 2 STRUCTURI DE DATE, Aprilie 2018;
Responsabil tema: Mihai Nan;


Programul consta in 3 task-uri;
Task-ul 1: Compresia unei imagini;
Task-ul 2: Decompresia unei imagini;
Task-ul 3: Oglindirea unei imagini (vertical sau orizontal)
BONUS: Suprapunerea a doua imagini;

Structuri folosite:

Pixel:
	Memoreaza elementele unei matrice de pixeli citite din fisierul de intrare
QuadtreeNode:
	Memoreaza elementele unui arbore cuaternar obtinut din matricea de pixeli;
QuadtreeArray:
	Memoreaza un vector obtinut din arborele cuaternar;


Functii principale ale programului:
	initNode - initializeaza un nod al arborelui cu parametrii r,g,b si area;
	build - construieste un arbore dintr-o matrice
	buildv/h - construieste un arbore oglindit v/h dintr-o matrice
	buildArray - construieste un vector din arbore
	build2 - construieste un arbore din vector
	build3 - construieste o matrice dintr-un arbore
	overlay - suprapune 2 arbori
Functii secundare ale programului:
	mean - calculeaza pragul "mean" dupa formula din enunt
	mediumRed/Green/Blue - calculeaza componenta medie de culoare r/g/b dintr-o portiune de imagine
Functii auxiliare ale programului:
	freeMatrix/freeQuad - elibereaza memoria alocata dinamic pt arbore si matrice
	countNodes - numara nodurile arborelui 
	countColors - numara frunzele arborelui
	sqroot - calculeaza radicalul de ordin 2 al unui numar

In main se verifica in primul rand ce task trebuie abordat, in functie de parametrii primiti din linia de comanda;
In functie de task, se apeleaza functiile astfel:
Task 1:
	-citirea matricei din fisierul de intrare
	-crearea arborelui
	-crearea vectorului
	-afisarea vectorului in fisierul de iesire
Task 2:
	-citirea vectorului din fisierul de intrare
	-crearea arborelui din vector
	-crearea matricei din arbore
	-afisarea matricei in fisierul de iesire
Task 3:
	-citirea matricei din fisierul de intrare
	-crearea arborelui oglindit corespunzator argumentelor din linia de comanda
	-crearea matricei rezultante si afisarea ei in fisierul de iesire
BONUS:
	-citirea primei matrice din primul fisier de intrare
	-citirea celei de-a doua matrice din al doilea fisier de intrare;
	-crearea celor doi arbori corespunzatori celor 2 matrice
	-suprapunerea celor 2 arbori
	-crearea matricei corespunzatoare arborelui rezultant
	-afisarea acesteia in fisierul de iesire

Dupa fiecare task se realizeaza eliberarea memoriei alocata dinamic in cadrul acelui task;

Functiile ce includ arbori, vectori, matrici sunt implementate folosind algoritmi recursivi;

Mentiuni:
Consider ca aceasta tema a fost una dintre cele mai interesante pe care le-am avut, si apreciez aplicabilitatea;
Este mult mai interesanta o tema al carei rezultat este vizibil decat una la fel de grea dar a carei parte aplicabila este restrasa.
