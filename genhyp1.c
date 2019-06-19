//==================================================== file = genhyp1.c =====
//=  Program to generate hyperexponentially distributed interarrival times  =
//=   - Generates delta time stamps                                         =
//===========================================================================
//=  Notes: 1) Writes to a user specified output file                       =
//=             * File format is <interarrival time delta>                  =
//=         2) Takes as input the Lambda1, Lambda2, P1, and P2              =
//=         3) Generates samples for user specified time period             =
//=-------------------------------------------------------------------------=
//= Example user input:                                                     =
//=                                                                         =
//=   ---------------------------------------- genhyp1.c -----              =
//=   -  Program to generate hyperexponentially distributed  -              =
//=   -  interarrival times.                                 -              =
//=   --------------------------------------------------------              =
//=   Output file name ===================================> output.dat      =
//=   Random number seed =================================> 1               =
//=   Arrival rate in customers per second (lambda1) =====> 10.0            =
//=   Arrival rate in customers per second (lambda2) =====> 1.0             =
//=   Probability for state 1 ============================> 0.25            =
//=   Time period to generate interarrival times =========> 10.0            =
//=   --------------------------------------------------------              =
//=   -  Generating samples to file                          -              =
//=   --------------------------------------------------------              =
//=   --------------------------------------------------------              =
//=   -  Done!                                                              =
//=   --------------------------------------------------------              =
//=-------------------------------------------------------------------------=
//= Example output file ("output.dat" for above):                           =
//=                                                                         =
//=   0.202846                                                              =
//=   0.779468                                                              =
//=   1.518870                                                              =
//=   0.038733                                                              =
//=   0.067537                                                              =
//=   0.655049                                                              =
//=   3.364708                                                              =
//=   0.063544                                                              =
//=   4.866771                                                              =
//=-------------------------------------------------------------------------=
//=  Build: bcc32 genhyp1.c                                                 =
//=-------------------------------------------------------------------------=
//=  Execute: genhyp1                                                       =
//=-------------------------------------------------------------------------=
//=  Author: Kenneth J. Christensen                                         =
//=          University of South Florida                                    =
//=          WWW: http://www.csee.usf.edu/~christen                         =
//=          Email: christen@csee.usf.edu                                   =
//=-------------------------------------------------------------------------=
//=  History: KJC (06/29/03) - Genesis (from genipp.c and genhyp2.c)        =
//===========================================================================

//----- Include files -------------------------------------------------------
#include <stdio.h>              // Needed for printf()
#include <stdlib.h>             // Needed for exit() and ato*()
#include <math.h>               // Needed for log()

//----- Function prototypes -------------------------------------------------
double expon(double x);         // Returns an exponential random variable
double rand_val(int seed);      // Jain's RNG

//===== Main program ========================================================
void main(void)
{
  FILE     *fp;                 // File pointer to output file
  char     file_name[256];      // Output file name string
  char     temp_string[256];    // Temporary string variable
  double   lambda1;             // Mean of arrival rate for state 1
  double   lambda2;             // Mean of arrival rate for state 2
  double   p1;                  // Probability goto state 1
  double   hyp_rv;              // Hyperxponential random variable
  double   time_period;         // Time period to generate arrival samples
  double   sum_time;            // Sum of time up to now
  int      i;                   // Loop counter

  // Output banner
  printf("---------------------------------------- genhyp1.c ----- \n");
  printf("-  Program to generate hyperexponentially distributed  - \n");
  printf("-  interarrival times.                                 - \n");
  printf("-------------------------------------------------------- \n");

  // Prompt for output filename and then create/open the file
  printf("Output file name ===================================> ");
  scanf("%s", file_name);
  fp = fopen(file_name, "w");
  if (fp == NULL)
  {
    printf("ERROR in creating output file (%s) \n", file_name);
    exit(1);
  }

  // Prompt for random number seed and then use it
  printf("Random number seed =================================> ");
  scanf("%s", temp_string);
  rand_val((int) atoi(temp_string));

  // Prompt for mean arrival rate for state 1 (lambda1)
  printf("Arrival rate in customers per second (lambda1) =====> ");
  scanf("%s", temp_string);
  lambda1 = atof(temp_string);

  // Prompt for mean arrival rate for state 2 (lambda2)
  printf("Arrival rate in customers per second (lambda2) =====> ");
  scanf("%s", temp_string);
  lambda2 = atof(temp_string);

  // Prompt for probability goto state 1
  printf("Probablity for state 1 =============================> ");
  scanf("%s", temp_string);
  p1 = atof(temp_string);

  // Prompt for time period (seconds) to generate samples
  printf("Time period to generate interarrival times =========> ");
  scanf("%s", temp_string);
  time_period = atof(temp_string);

  //Output message and generate interarrival times
  printf("-------------------------------------------------------- \n");
  printf("-  Generating samples to file                          - \n");
  printf("-------------------------------------------------------- \n");

  // Generate and output interarrival times
  sum_time = 0.0;
  while(1)
  {
    if (rand_val(0) <= p1)
      hyp_rv = expon(1.0 / lambda1);
    else
      hyp_rv = expon(1.0 / lambda2);
    fprintf(fp, "%f \n", hyp_rv);
    sum_time = sum_time + hyp_rv;
    if (sum_time >= time_period) break;
  }

  //Output message and close the output file
  printf("-------------------------------------------------------- \n");
  printf("-  Done! \n");
  printf("-------------------------------------------------------- \n");
  fclose(fp);
}

//=============================================================================
//=  Function to generate exponentially distributed RVs                       =
//=    - Input:  x (mean value of distribution)                               =
//=    - Output: Returns with exponential RV                                  =
//=============================================================================
double expon(double x)
{
  double z;     // Uniform random number from 0 to 1

  // Pull a uniform RV (0 < z < 1)
  do
  {
    z = rand_val(0);
  }
  while ((z == 0) || (z == 1));

  return(-x * log(z));
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
