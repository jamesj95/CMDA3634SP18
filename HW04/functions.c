#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "functions.h"
#include "omp.h"

//compute a*b mod p safely
unsigned int modprod(unsigned int a, unsigned int b, unsigned int p) {
  unsigned int za = a;
  unsigned int ab = 0;

  while (b > 0) {
    if (b%2 == 1) ab = (ab +  za) % p;
    za = (2 * za) % p;
    b /= 2;
  }
  return ab;
}

//compute a^b mod p safely
unsigned int modExp(unsigned int a, unsigned int b, unsigned int p) {
  unsigned int z = a;
  unsigned int aExpb = 1;

  while (b > 0) {
    if (b%2 == 1) aExpb = modprod(aExpb, z, p);
    z = modprod(z, z, p);
    b /= 2;
  }
  return aExpb;
}

//returns either 0 or 1 randomly
unsigned int randomBit() {
  return rand()%2;
}

//returns a random integer which is between 2^{n-1} and 2^{n}
unsigned int randXbitInt(unsigned int n) {
  unsigned int r = 1;
  for (unsigned int i=0; i<n-1; i++) {
    r = r*2 + randomBit();
  }
  return r;
}

//tests for primality and return 1 if N is probably prime and 0 if N is composite
unsigned int isProbablyPrime(unsigned int N) {

  if (N%2==0) return 0; //not interested in even numbers (including 2)

  unsigned int NsmallPrimes = 168;
  unsigned int smallPrimeList[168] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 
                                37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 
                                79, 83, 89, 97, 101, 103, 107, 109, 113, 
                                127, 131, 137, 139, 149, 151, 157, 163, 
                                167, 173, 179, 181, 191, 193, 197, 199, 
                                211, 223, 227, 229, 233, 239, 241, 251, 
                                257, 263, 269, 271, 277, 281, 283, 293, 
                                307, 311, 313, 317, 331, 337, 347, 349, 
                                353, 359, 367, 373, 379, 383, 389, 397, 
                                401, 409, 419, 421, 431, 433, 439, 443, 
                                449, 457, 461, 463, 467, 479, 487, 491, 
                                499, 503, 509, 521, 523, 541, 547, 557, 
                                563, 569, 571, 577, 587, 593, 599, 601, 
                                607, 613, 617, 619, 631, 641, 643, 647, 
                                653, 659, 661, 673, 677, 683, 691, 701, 
                                709, 719, 727, 733, 739, 743, 751, 757, 
                                761, 769, 773, 787, 797, 809, 811, 821, 
                                823, 827, 829, 839, 853, 857, 859, 863, 
                                877, 881, 883, 887, 907, 911, 919, 929, 
                                937, 941, 947, 953, 967, 971, 977, 983, 
                                991, 997};

  //before using a probablistic primality check, check directly using the small primes list
  for (unsigned int n=1;n<NsmallPrimes;n++) {
    if (N==smallPrimeList[n])   return 1; //true
    if (N%smallPrimeList[n]==0) return 0; //false
  }

  //if we're testing a large number switch to Miller-Rabin primality test
  unsigned int r = 0;
  unsigned int d = N-1;
  while (d%2 == 0) {
    d /= 2;
    r += 1;
  }

  for (unsigned int n=0;n<NsmallPrimes;n++) {
    unsigned int k = smallPrimeList[n];
    unsigned int x = modExp(k,d,N);

    if ((x==1) || (x==N-1)) continue;

    for (unsigned int i=1;i<r-1;i++) {
      x = modprod(x,x,N);
      if (x == 1) return 0; //false
      if (x == N-1) break;
    }
    // see whether we left the loop becasue x==N-1
    if (x == N-1) continue; 

    return 0; //false
  }
  return 1; //true
}

//Finds a generator of Z_p using the assumption that p=2*q+1
unsigned int findGenerator(unsigned int p) {
  unsigned int g;
  unsigned int q = (p-1)/2;

  do {
    //make a random number 1<= g < p
    g = randXbitInt(32)%p; //could also have passed n to findGenerator
  } while (g==0 || (modExp(g,q,p)==1) || (modExp(g,2,p)==1));
  
  return g;
}

void setupElGamal(unsigned int n, unsigned int *p, unsigned int *g, 
                                  unsigned int *h, unsigned int *x) {

  /* Use isProbablyPrime and randomXbitInt to find a new random n-bit prime number 
     which satisfies p=2*q+1 where q is also prime */
  unsigned int q;
  do {
    *p = randXbitInt(n);
    q = (*p-1)/2;
  } while (!isProbablyPrime(*p) || !isProbablyPrime(q));


  /* Use the fact that p=2*q+1 to quickly find a generator */
  *g = findGenerator(*p);
  
  //pick a secret key,  x
  *x = randXbitInt(n)%(*p);

  //compute h
  *h = modExp(*g,*x,*p);
  
  printf("ElGamal Setup successful.\n");
  printf("p = %u. \n", *p);  
  printf("g = %u is a generator of Z_%u \n", *g, *p);  
  printf("Secret key: x = %u \n", *x);
  printf("h = g^x = %u\n", *h);
  printf("\n");
}

void ElGamalEncrypt(unsigned int *m, unsigned int *a, unsigned int Nints, 
                    unsigned int p, unsigned int g, unsigned int h) {

  /* Q2.1 Parallelize this function with OpenMP   */
  #pragma omp parallel for 
  for (unsigned int i=0; i<Nints;i++) {
    //pick y in Z_p randomly
    unsigned int y;
    do {
      y = randXbitInt(32)%p;
    } while (y==0); //dont allow y=0

    //compute a = g^y
    a[i] = modExp(g,y,p);

    //compute s = h^y
    unsigned int s = modExp(h,y,p);

    //encrypt m by multiplying with s
    m[i] = modprod(m[i],s,p);  
  }
}

void ElGamalDecrypt(unsigned int *m, unsigned int *a, unsigned int Nints,
                    unsigned int p, unsigned int x) {

  /* Q2.1 Parallelize this function with OpenMP   */
  #pragma omp parallel for 
  for (unsigned int i=0; i<Nints;i++) {
    //compute s = a^x
    unsigned int s = modExp(a[i],x,p);

    //compute s^{-1} = s^{p-2}
    unsigned int invS = modExp(s,p-2,p);
    
    //decrypt message by multplying by invS
    m[i] = modprod(m[i],invS,p);
  }
}

//Pad the end of string so its length is divisible by Nchars
// Assume there is enough allocated storage for the padded string 
void padString(unsigned char* string, unsigned int charsPerInt) {

  /* Q1.2 Complete this function   */
  int numChars = strlen(string);
  numChars += (numChars % charsPerInt);
  int neededPad = numChars - strlen(string);
  realloc(string, (numChars + 1)*sizeof(char));
  //printf("%d and %d\n", numChars),strlen(string);
  for (int i = 0; i < neededPad; i++) {
	string[strlen(string) + i] = ' ';
	//printf("added pad for needed pad %d and %d and %d\n",neededPad,numChars, strlen(string));	
  }
  string[numChars] = '\0';
  //printf("%s the replacement\n", string);
}


void convertStringToZ(unsigned char *string, unsigned int Nchars,
                      unsigned int  *Z,      unsigned int Nints) {

  /* Q1.3 Complete this function   */
  /* Q2.2 Parallelize this function with OpenMP   */
  char* curr = malloc(Nchars / Nints); // num chars per int 
  int num = 0;
  #pragma omp parallel for shared(Z)
  for (int t = ((Nchars/Nints) - 1); t < Nchars; t = t + (Nchars/Nints)) {
     int num = 0;
     for (int i = (Nchars/Nints) - 1; i >= 0; i--) {
     	curr[i] = string[t - i];
     	num += curr[i] << ((((Nchars/Nints) - 1) - i) * 8);
     } 
	Z[t / (Nchars / Nints)] = num;
  }
  free(curr);
}


void convertZToString(unsigned int  *Z,      unsigned int Nints,
                      unsigned char *string, unsigned int Nchars) {

  /* Q1.4 Complete this function   */
  /* Q2.2 Parallelize this function with OpenMP   */
  //char * curr = malloc (Nchars / Nints); 
  int currInt; 
  int num;
  int globalCtr = 0;
  int ratio = Nchars / Nints;
  int t;
  #pragma omp parallel for shared(string)
  for (int i = 0; i < Nints; i++) {
  	currInt = Z[i];
 	for (t = ratio - 1; t >= 0; t--) {
//		printf("currInt is %c and we are at pos %d\n",(char)  currInt >> (t * 8), globalCtr);
		int val = t * 8;
		string[(ratio * i) + t] = (char) (currInt >> val); 
		currInt -= ((currInt >> val) << val);
	}
  }
  int g = Nchars-1;
  while (string[g] == ' ') {
 	g--; 
  }
  string[g+1] = '\0'; // g + 1 to get rid of pad 
}

/* convert cyphertext to a string */
char* cypherToString(unsigned int *m, unsigned int *a, int length) {
	// unsigned int is 32 bits 
	//unsigned int length = sizeof(m) / sizeof(unsigned int); // number of ints we have in m 
	// size of m should match size of a
	// we need 4*length(m) bytes for the string 
	// We could do this in less space if we used inputted bitsize
	unsigned int fullSize = length * 2;
	int ratio = 2;
	int currInt = m[0];
	char * finalString = malloc((length * 4 + 1) * sizeof(char)); 
	int counter = 0;
	for (int i = 0; i < length * 2; i+=2) {
		i = i / 2;
		//printf("i is %d and m[i] is %u and a[i] is %u\n", i, m[i], a[i]);
		currInt = m[i] & 0xFF;
		finalString[counter] =  (m[i] >> 8) + '0';
		finalString[counter+1] = currInt + '0';
		currInt = a[i] & 0xFF;
		finalString[counter+2] =( a[i] >> 8) + '0';
		finalString[counter+3] = currInt + '0';
		// Note: we can convert all of these characters back by adding '0'
		// (or casting), then if the resulting integer is negative, adding 256
		// to the value we retrieve. 
		currInt = m[i] & 0xFF;
	//	printf("%d and %c and %c and %d\n", currInt, currInt + '0', 250 + '0', (int) (finalString[counter+1] - '0'));	
		counter+=4;
		i = i * 2;
	}
	finalString[length * 4 + 1] = '\0';
	//printf("we came up with %s\n", finalString);
	return finalString;

}

void convertStringBack(char* final){
    int counter = 0;
    while (final[counter] != '\0') {
	int upper = (int) (final[counter] - '0');
	int lower = (int) (final[counter+1] - '0');
	if (upper < 0) {
		upper +=256;
	} 
	if (lower < 0) {
		lower +=256;
	}
	int number = (upper << 8) + lower;
	if (counter % 4 == 0) { //we're at an m
		printf("message (mhat,a) is (%d, ",number);
	} else {
		printf("%d) \n", number);
	}
	counter+=2;
    }
}

