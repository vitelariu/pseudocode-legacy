# pseudocod

Compileaza cu g++ main.cpp -o main

# atentie
Acest interpret a fost scris cu o metoda mai neortodoxa. Manipularea erorilor este
minima, deci cand se scrie pseudocod mare atentie la sintaxa. !!! Sintaxa gresita
poate duce la segmentation fault / rezultate nedorite !!!
Acest proiect a fost facut de fun, nu pentru un motiv real!
Enjoy

# scrie
```scrie 1```
output: 1

```scrie "Salut\n"```
output: Salut

```scrie "String", " ", "altceva" + "2"```
output: String altceva2

```scrie !(1 = 10 - 9) sau (100 > 69 si 30 < 60)```
output: 1

# citeste
```
citeste x (numar natural)
scrie x
```
output: x (va fi un numar)

```
citeste y (text)
scrie y
```
output: y (va fi text)
```
citeste x, y, z (numere naturale)
scrie x + y + z
```
output: x + y + z

# asignare
```
x <- 0
y <- "Hello"
z <- "Hello " + "World"
```

# daca
```
daca 1 + 1 = 2 atunci
  scrie "Adevarat\n"
```
```
citeste a
daca a = "a" atunci
  scrie "Adevarat\n"
altfel
  scrie "Fals\n"
```

# cat timp
```
i <- 0
cat timp i <= 100 executa
  scrie i, " "
  i <- i + 1
```

# executa
```
i <- 0
executa
  scrie i, " "
  i <- i + 1
pana cand i <= 100
```
# pentru
```
pentru i <- 0, 10 executa
  scrie i, " "
```
```
pentru i <- 100, 0, -1 executa
  scrie i, " "
```
# comentarii
// Comentariile se pun pe linie separata cu incepand cu "//"
Corect:
```
// Program
scrie 1
```

Gresit:
```
scrie 1 // program
```

# arrays
// Initializeaza un array cu 10 elemente
// fara acel '\' din fata [100]
```
array[10] <- 0
scrie array[1] + 69

array[3] <- 0, 1, 2
scrie array[0]
```
```
text <- "Hello World"
scrie text[0]
```
// !!! atentie
```
text <- "\n"
scrie text[0]
// eroare
```
# conversie
```
citeste x (text)
// sa zicem ca citeste un caracter "a"
scrie numar: x
```
```
citeste y (numar natural)
scrie text: y
```

// Conversia se foloseste doar in statement-urile scrie si asigneaza. Nu se foloseste pe un numar in parte



# exemple de cod
## fibonacci
```
// Fibonacci

citeste n

a <- 0
b <- 1
c <- 0

cat timp c <= n executa
	scrie c, " "
	c <- a + b
	a <- b
	b <- c
```

## brainfuck interpreter
```
//citeste program (text)
// hello world hard coded
program <- "+++++++\[>++++++++++<-]>++.>++++++++++\[>++++++++++<-]>+.>++++++++++\[>+++++++++++<-]>--..>++++++++++\[>+++++++++++<-]>+.>++++++++++\[>+++<-]>++.>++++++++++\[>+++++++++<-]>---.>++++++++++\[>+++++++++++<-]>+.>++++++++++\[>+++++++++++<-]>++++.>++++++++++\[>+++++++++++<-]>--.>++++++++++\[>++++++++++<-]>."
program <- program + ";"

array[2000] <- 0
p <- 50
i <- 0


cat timp program[i] != ";" executa
	daca program[i] = "+" atunci
		array[p] <- array[p] + 1
	daca program[i] = "-" atunci
		array[p] <- array[p] - 1
	daca program[i] = ">" atunci
		p <- p + 1
	daca program[i] = "<" atunci
		p <- p - 1
	daca program[i] = "]" atunci
		daca array[p] != 0 atunci
			pair <- 1
			cat timp pair != 0 executa
				i <- i - 1
				daca program[i] = "[" atunci
					pair <- pair - 1
				daca program[i] = "]" atunci
					pair <- pair + 1


	daca program[i] = "," atunci
		citeste x
		array[p] <- x
	daca program[i] = "." atunci
		scrie text: array[p]


	i <- i + 1
```
