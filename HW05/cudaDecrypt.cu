nclude <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "cuda.h"
#include "functions.c"

__device__ unsigned int modprodC(unsigned int a, unsigned int b, unsigned int p) {
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
__device__ unsigned int modExpC(unsigned int a, unsigned int b, unsigned int p) {
  unsigned int z = a;
  unsigned int aExpb = 1;

  while (b > 0) {
    if (b%2 == 1) aExpb = modprodC(aExpb, z, p);
    z = modprodC(z, z, p);
    b /= 2;
  }
  return aExpb;
}


__global__ void findX(unsigned int p, unsigned int g, unsigned int h, unsigned int *x)
{
//unsigned int block = blockIdx.x;
//unsigned int blocksize = blockDim.x;
//unsigned int thread = threadIdx.x;
//unsigned int id=thread + block*blocksize;
if (*x==0 || modExpC(g,*x,p)!=h) {
    printf("Finding the secret key...\n");
    double startTime = clock();
    for (unsigned int i=0;i<p-1;i++) {
      if (modExpC(g,i+1,p)==h) {
        printf("Secret key found! x = %u \n", i+1);
        *x=i+1;
      } 
    }
    double endTime = clock();

    double totalTime = (endTime-startTime)/CLOCKS_PER_SEC;
    double work = (double) p;
    double throughput = work/totalTime;

    printf("Searching all keys took %g seconds, throughput was %g values tested per second.\n", totalTime, throughput);
  }


}

int main (int argc, char **argv) {

  /* Part 2. Start this program by first copying the contents of the main function from 
     your completed decrypt.c main function. */

  /* Q4 Make the search for the secret key parallel on the GPU using CUDA. */




  //declare storage for an ElGamal cryptosytem
  unsigned int n, p, g, h, x;
  unsigned int Nints;

  //get the secret key from the user
  printf("Enter the secret key (0 if unknown): "); fflush(stdout);
  char stat = scanf("%u",&x);

  printf("Reading file.\n");

  /* Q3 Complete this function. Read in the public key data from public_key.txt
    and the cyphertexts from messages.txt. */
FILE *pub_key = fopen("public_key.txt","r");
FILE *cyperT = fopen("message.txt","r");
fscanf(pub_key,"%u\n%u\n%u\n%u",&n,&p,&g,&h);
fclose(pub_key);
fscanf(cyperT,"%u\n",&Nints);
unsigned int *a=(unsigned int *) malloc(Nints*sizeof(unsigned int));
unsigned int *Zmessage = (unsigned int *) malloc(Nints*sizeof(unsigned int));
for(unsigned int i=0;i<Nints;i++)
{ 
 fscanf(cyperT,"%u %u\n",&Zmessage[i],&a[i]);
 
}
fclose(cyperT); 
  // find the secret key
unsigned int Nthreads = Nints;
unsigned int Nblocks = (n+Nthreads-1)/Nthreads;
cudaMalloc((void**)&x,1*sizeof(unsigned int));
printf("%u\n",x);
findX<<< Nthreads,Nblocks >>>(p,g,h,&x);
//cudaDeviceSynchronize();
printf("x:%u\n",x);
unsigned int foundx;
  /* Q3 After finding the secret key, decrypt the message */
cudaMemcpy(&x,&foundx,1*sizeof(unsigned int),cudaMemcpyHostToDevice);
printf("secret key:%u\n",foundx);
ElGamalDecrypt(Zmessage,a,Nints,p,foundx);
unsigned char *message = (unsigned char *) malloc(Nints*sizeof(unsigned char));
unsigned int charsPerInt = (n-1)/8;
unsigned int Nchars = Nints*charsPerInt;
convertZToString(Zmessage,Nints,message,Nchars);
printf("Decrypted Message = \"%s\"\n",message);


cudaFree(&x);
  return 0;

 
}
