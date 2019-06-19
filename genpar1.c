//==================================================== file = genpar1.c =====
//=  Program to generate Pareto interarrival times                          =
//=    - pdf is f(x) = (a*k^a)/x^(a+1)                                      =
//=    - k is lower bound and infinity is upper bound                       =
//=    - mean is (a*k)/(a-1)                                                =
//=    - variance is (a*k^2)/((a-1)^2*(a-2))                                =
//===========================================================================
//=  Notes: 1) Writes to a user specified output file                       =
//=             * File format is <interarrival time delta>                  =
//=         2) Generates samples for user specified time period             =
//=         3) See M. Garrett and W. Willinger, "Analysis, Modeling and     =
//=            Generation of Self-Similar VBR Video Traffic," ACM           =
//=            Computer Communications Review, Vol. 24, No. 4,              =
//=            pp. 269 - 280, September 1994.  See also, R. Jain            =
//=            "The Art of Computer Systems Performance Analysis,"          =
//=            John Wiley & Sons, 1991.  Page 495.                          =
//=-------------------------------------------------------------------------=
//= Example user input:                                                     =
//=                                                                         =
//=   ---------------------------------------- genpar1.c -----              =
//=   -  Program to generate Pareto random variables with    -              =
//=   -  lower bound value of k                              -              =
//=   --------------------------------------------------------              =
//=   Enter output file name =========================> output.dat          =
//=   Random number seed (greater than zero) =============> 1               =
//=   Pareto alpha value =============================> 1.5                 =
//=   Pareto k value =================================> 1.0                 =
//=   Number of values to generate ===================> 5                   =
//=   --------------------------------------------------------              =
//=   -  Generating samples to file                                         =
//=   -    * alpha = 1.500000                                               =
//=   -    * k     = 1.000000                                               =
//=   --------------------------------------------------------              =
//=   --------------------------------------------------------              =
//=   -  Done!                                                              =
//=   --------------------------------------------------------              =
//=-------------------------------------------------------------------------=
//= Example output file ("output.dat" for above):                           =
//=                                                                         =
//=   2536.840195                                                           =
//=   3.866336                                                              =
//=   1.205415                                                              =
//=   1.681431                                                              =
//=   1.521627                                                              =
//=-------------------------------------------------------------------------=
//=  Build: bcc32 genpar1.c                                                 =
//=-------------------------------------------------------------------------=
//=  Execute: genpar1                                                       =
//=-------------------------------------------------------------------------=
//=  Author: Kenneth J. Christensen                                         =
//=          University of South Florida                                    =
//=          WWW: http://www.csee.usf.edu/~christen                         =
//=          Email: christen@csee.usf.edu                                   =
//=-------------------------------------------------------------------------=
//=  History: KJC (01/06/99) - Genesis                                      =
//=           KJC (05/20/03) - Added Jain's RNG for finer granularity       =
//=           KJC (05/13/09) - Minor clean-up                               =
//===========================================================================
//----- Include files -------------------------------------------------------
#include <stdio.h>              // Needed for printf()
#include <stdlib.h>             // Needed for exit() and ato*()
#include <math.h>               // Needed for log() and pow()

//----- Function prototypes -------------------------------------------------
double pareto(double a, double k);    // Returns a Pareto rv
double rand_val(int seed);            // Jain's RNG

//===== Main program ========================================================
void main(void)
{
  char   in_string[256];      // Input string
  FILE   *fp;                 // File pointer to output file
  double a;                   // Pareto alpha value
  double k;                   // Pareto k value
  double pareto_rv;           // Pareto random variable
  int    num_values;          // Number of values
  int    i;                   // Loop counter

  //Output banner
  printf("---------------------------------------- genpar1.c ----- \n");
  printf("-  Program to generate Pareto random variables with    - \n");
  printf("-  lower bound value of k                              - \n");
  printf("-------------------------------------------------------- \n");

  // Prompt for output filename and then create/open the file
  printf("Enter output file name =========================> ");
  scanf("%s", in_string);
  fp = fopen(in_string, "w");
  if (fp == NULL)
  {
    printf("ERROR in creating output file (%s) \n", in_string);
    exit(1);
  }

  // Prompt for random number seed and then use it
  printf("Random number seed (greater than zero) =============> ");
  scanf("%s", in_string);
  rand_val((int) atoi(in_string));

  // Prompt for Pareto alpha value
  printf("Pareto alpha value =============================> ");
  scanf("%s", in_string);
  a = atof(in_string);

  // Prompt for Pareto k value
  printf("Pareto k value =================================> ");
  scanf("%s", in_string);
  k = atof(in_string);

  // Prompt for number of values to generate
  printf("Number of values to generate ===================> ");
  scanf("%s", in_string);
  num_values = atoi(in_string);

  //Output message and generate samples
  printf("-------------------------------------------------------- \n");
  printf("-  Generating samples to file                    \n");
  printf("-    * alpha = %f                                \n", a);
  printf("-    * k     = %f                                \n", k);
  printf("-------------------------------------------------------- \n");
  for (i=0; i<num_values; i++)
  {
    pareto_rv = pareto(a, k);
    fprintf(fp, "%f \n", pareto_rv);
  }

  //Output message and close the outout file
  printf("-------------------------------------------------------- \n");
  printf("-  Done! \n");
  printf("-------------------------------------------------------- \n");
  fclose(fp);
}

//===========================================================================
//=  Function to generate Pareto distributed RVs using                      =
//=    - Input:  a and k                                                    =
//=    - Output: Returns with Pareto RV                                     =
//===========================================================================
double pareto(double a, double k)
{
  double z;     // Uniform random number from 0 to 1
  double rv;    // RV to be returned

  // Pull a uniform RV (0 < z < 1)
  do
  {
    z = rand_val(0);
  }
  while ((z == 0) || (z == 1));

  // Generate Pareto rv using the inversion method
  rv = k / pow(z, (1.0 / a));

  return(rv);
}

//=========================================================================
//= Multiplicative LCG for generating uniform(0.0, 1.0) random numbers    =
//=   - x_n = 7^5*x_(n-1)mod(2^31 - 1)                                    =
//=   - With x seeded to 1 the 10000th x value should be 1043618065       =
//=   - From R. Jain, "The Art of Computer Systems Performance Analysis," =
//=     John Wiley & Sons, 1991. (Page 443, Figure 26.2)                  =
//=========================================================================
double rand_val(int seed)
{
  const long  a =      16807;  // Multiplier
  const long  m = 2147483647;  // Modulus
  const long  q =     127773;  // m div a
  const long  r =       2836;  // m mod a
  static long x;               // Random int value
  long        x_div_q;         // x divided by q
  long        x_mod_q;         // x modulo q
  long        x_new;           // New x value

  // Set the seed if argument is non-zero and then return zero
  if (seed > 0)
  {
    x = seed;
    return(0.0);
  }

  // RNG using integer arithmetic
  x_div_q = x / q;
  x_mod_q = x % q;
  x_new = (a * x_mod_q) - (r * x_div_q);
  if (x_new > 0)
    x = x_new;
  else
    x = x_new + m;

  // Return a random value between 0.0 and 1.0
  return((double) x / m);
}
