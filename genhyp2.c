//==================================================== file = genhyp2.c =====
//=  Program to generate hyperexponentially distributed interarrival times  =
//=   - Generates delta time stamps (using Morse's method)                  =
//===========================================================================
//=  Notes: 1) Writes to a user specified output file                       =
//=             * File format is <interarrival time delta>                  =
//=         2) Takes as input the mean arrival rate and the CoV             =
//=         3) Generates samples for user specified time period             =
//=-------------------------------------------------------------------------=
//= Example user input:                                                     =
//=                                                                         =
//=   ---------------------------------------- genhyp2.c -----              =
//=   -  Program to generate hyperexponentially distributed  -              =
//=   -  interarrival times for a given CoV.                 -              =
//=   --------------------------------------------------------              =
//=   Output file name ===================================> output.dat      =
//=   Random number seed =================================> 1               =
//=   Arrival rate in customers per second (lambda) ======> 1.0             =
//=   Desired coefficent of variation (CoV > 1) ==========> 2.0             =
//=   Time period to generate interarrival times =========> 20.0            =
//=   --------------------------------------------------------              =
//=   -  Generating samples to file                          -              =
//=   --------------------------------------------------------              =
//=   --------------------------------------------------------              =
//=   -  Done!                                                              =
//=   --------------------------------------------------------              =
//=-------------------------------------------------------------------------=
//= Example output file ("output.dat" for above):                           =
//=                                                                         =
//=   8.999251                                                              =
//=   0.439236                                                              =
//=   0.855896                                                              =
//=   1.718401                                                              =
//=   0.038058                                                              =
//=   0.369126                                                              =
//=   1.896041                                                              =
//=   2.819142                                                              =
//=   2.742466                                                              =
//=   1.524527                                                              =
//=-------------------------------------------------------------------------=
//=  Build: bcc32 genhyp2.c                                                 =
//=-------------------------------------------------------------------------=
//=  Execute: genhyp2                                                       =
//=-------------------------------------------------------------------------=
//=  Author: Ken Christensen                                                =
//=          University of South Florida                                    =
//=          WWW: http://www.csee.usf.edu/~christen                         =
//=          Email: christen@csee.usf.edu                                   =
//=-------------------------------------------------------------------------=
//=  History: KJC (12/17/98) - Genesis                                      =
//=           KJC (05/20/03) - Added Jain's RNG for finer granularity       =
//=           KJC (06/29/03) - Renamed to genhyp2.c                         =
//===========================================================================

//----- Include files -------------------------------------------------------
#include <stdio.h>              // Needed for printf()
#include <stdlib.h>             // Needed for exit() and ato*()
#include <math.h>               // Needed for log()

//----- Function prototypes -------------------------------------------------
double hyper(double x, double cov);  // Returns a hyperexponential rv
double rand_val(int seed);           // Jain's RNG

//===== Main program ========================================================
void main(void)
{
  FILE     *fp;                 // File pointer to output file
  char     file_name[256];      // Output file name string
  char     temp_string[256];    // Temporary string variable
  double   lambda;              // Mean of arrival rate
  double   cov;                 // Coefficient of variation
  double   hyp_rv;              // Hyperxponential random variable
  double   time_period;         // Time period to generate arrival samples
  double   sum_time;            // Sum of time up to now
  int      i;                   // Loop counter

  // Output banner
  printf("---------------------------------------- genhyp2.c ----- \n");
  printf("-  Program to generate hyperexponentially distributed  - \n");
  printf("-  interarrival times for a given CoV.                 - \n");
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

  // Prompt for mean arrival rate (lambda)
  printf("Arrival rate in customers per second (lambda) ======> ");
  scanf("%s", temp_string);
  lambda = atof(temp_string);

  // Prompt for coefficent of variation (cov)
  printf("Desired coefficent of variation (CoV > 1) ==========> ");
  scanf("%s", temp_string);
  cov = atof(temp_string);

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
    hyp_rv = hyper((1.0 / lambda), cov);
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

//===========================================================================
//=  Function to generate hyperexponentially distributed random variables   =
//=  Uses Morse's method taken from SMPL from Simulating Computer Systems   =
//=  Systems, Techniques and Tools by M. H. MacDougall (1987)               =
//=    - Input:  Mean value of distribution and coefficient of variation    =
//=    - Output: Returns with hyperexponentially distributed rv             =
//===========================================================================
double hyper(double x, double cov)
{
  double p;                     // Probability value for Morse's method
  double z1, z2;                // Uniform random numbers from 0 to 1
  double hyp_value;             // Computed exponential value to be returned
  double temp;                  // Temporary double value

  // Pull a uniform random number (0 < z1 < 1)
  do
  {
    z1 = rand_val(0);
  }
  while ((z1 == 0) || (z1 == 1));

  // Pull another uniform random number (0 < z2 < 1)
  do
  {
    z2 = rand_val(0);
  }
  while ((z2 == 0) || (z2 == 1));

  // Compute hyperexponential random variable using Morse's method
  temp = cov*cov;
  p = 0.5 * (1.0 - sqrt((temp - 1.0) / (temp + 1.0)));
  if (z1 > p)
    temp = x / (1.0 - p);
  else
    temp = x / p;
  hyp_value = -0.5 * temp * log(z2);

  return(hyp_value);
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
