//==================================================== file = genuniq. =====
//=  Program to generate unique random integer values                       =
//===========================================================================
//=  Notes: 1) Writes to a user specified output file                       =
//=         2) Generates user specified number of values (up to 2^32 - 1)   =
//=         3) Uses Roy Hann's Generate31()                                 =
//=-------------------------------------------------------------------------=
//= Example execution:                                                      =
//=                                                                         =
//=   ---------------------------------------- genuniq.c -----              =
//=   -  Program to generate unique random integers          -              =
//=   --------------------------------------------------------              =
//=   Output file name ===================================> out.dat         =
//=   Random number seed (greater than 0) ================> 1               =
//=   Number of unique values to generate ================> 1000000         =
//=   --------------------------------------------------------              =
//=   -  Generating samples to file                          -              =
//=   --------------------------------------------------------              =
//=   --------------------------------------------------------              =
//=   -  Done!                                                              =
//=   --------------------------------------------------------              =
//=-------------------------------------------------------------------------=
//=  Build: bcc32 genuniq.c                                                 =
//=-------------------------------------------------------------------------=
//=  Execute: genuniq                                                       =
//=-------------------------------------------------------------------------=
//=  Author: Ken Christensen                                                =
//=          University of South Florida                                    =
//=          WWW: http://www.csee.usf.edu/~christen                         =
//=          Email: christen@csee.usf.edu                                   =
//=-------------------------------------------------------------------------=
//=  History: KJC (05/14/08) - Genesis                                      =
//=           KJC (05/15/08) - Fixed probelm in shuffle                     =
//===========================================================================
//----- Include files -------------------------------------------------------
#include <stdio.h>             // Needed for printf()
#include <stdlib.h>            // Needed for malloc(), atoi(), and exit()

//----- Function prototypes -------------------------------------------------
int randInt(int seed);         // LCG RNG with x_n = 7^5*x_(n-1)mod(2^31 - 1)
int Generator31(void);         // Unique value RNG credited to Roy Hann

//===========================================================================
//=  Main program                                                           =
//===========================================================================
void main()
{
  FILE   *fp;                  // File pointer to output file
  char    file_name[256];      // Output file name string
  char    temp_string[256];    // Temporary string variable
  int     num;                 // Number of values to generate
  int    *z;                   // Array of shuffled unique integers
  int     temp;                // Temporary value
  int     i, j;                // Indexes

  // Output banner
  printf("---------------------------------------- genuniq.c ----- \n");
  printf("-  Program to generate unique random integers          - \n");
  printf("-------------------------------------------------------- \n");

  // Prompt for output filename and then create/open the file
  printf("Output file name ===================================> ");
  scanf("%s", file_name);
  fp = fopen(file_name, "w");
  if (fp == NULL)
  {
    printf("*** ERROR in creating output file (%s) \n", file_name);
    exit(1);
  }

  // Prompt for random number seed and then use it
  printf("Random number seed (greater than 0) ================> ");
  scanf("%s", temp_string);
  randInt((int) atoi(temp_string));

  // Prompt for number of values to generate
  printf("Number of unique values to generate ================> ");
  scanf("%s", temp_string);
  num = atoi(temp_string);

  // Malloc space for an array of num integers
  z = (int *) malloc(num * sizeof(int));
  if (z == NULL)
  {
    printf("*** ERROR - could not malloc space for array \n");
    exit(1);
  }

  // Create an array of num unique integers
  for (i=0; i<num; i++)
    z[i] = Generator31();

  // Shuffle the array of unique integers
  for (i=0; i<num; i++)
  {
    j = randInt(0) % num;
    temp = z[i];
    z[i] = z[j];
    z[j] = temp;
  }

  // Output message and generate values
  printf("-------------------------------------------------------- \n");
  printf("-  Generating samples to file                          - \n");
  printf("-------------------------------------------------------- \n");

  // Output the values to the file
  for (i=0; i<num; i++)
    fprintf(fp, "%d \n", z[i]);

  // Output message and close the output file
  printf("-------------------------------------------------------- \n");
  printf("-  Done!                                               - \n");
  printf("-------------------------------------------------------- \n");
  fclose(fp);
}

//=========================================================================
//= RNG of unique values from                                             =
//=   http://www.teradata.com/teradataforum/Topic10933-9-1.aspx#bm11020   =
//=========================================================================
int Generator31(void)
{
  static int n = 1;            // Start with n = 1

  // Generate the unique random value
  n = n >> 1 | ((n^n >> 3) & 1) << 30;

  // Return the unique random value
  return n;
}

//=========================================================================
//= Multiplicative LCG for generating uniform(0.0, 1.0) random numbers    =
//=   - x_n = 7^5*x_(n-1)mod(2^31 - 1)                                    =
//=   - With x seeded to 1 the 10000th x value should be 1043618065       =
//=   - From R. Jain, "The Art of Computer Systems Performance Analysis," =
//=     John Wiley & Sons, 1991. (Page 443, Figure 26.2)                  =
//=========================================================================
int randInt(int seed)
{
  const long  a =      16807;  // Multiplier
  const long  m = 2147483647;  // Modulus
  const long  q =     127773;  // m div a
  const long  r =       2836;  // m mod a
  static long x;               // Random int value (seed is set to 1)
  long        x_div_q;         // x divided by q
  long        x_mod_q;         // x modulo q
  long        x_new;           // New x value

  // Seed the RNG
  if (seed > 0) x = seed;

  // RNG using integer arithmetic
  x_div_q = x / q;
  x_mod_q = x % q;
  x_new = (a * x_mod_q) - (r * x_div_q);
  if (x_new > 0)
    x = x_new;
  else
    x = x_new + m;

  // Return a random integer value
  return(x);
}
