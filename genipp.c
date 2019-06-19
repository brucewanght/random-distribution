//===================================================== file = genipp.c =====
//=  Program to generate IPP interarrival times                             =
//===========================================================================
//=  Notes:  1) Write to a user specified output file                       =
//=             * File format is <interarrival time delta>                  =
//=          2) Generates samples for user specified time period            =
//=-------------------------------------------------------------------------=
//= Example user input:                                                     =
//=                                                                         =
//=   --------------------------------------------------------              =
//=   -  Program to generate IPP interarrival times                         =
//=   --------------------------------------------------------              =
//=   Enter output file name =========================> output.dat          =
//=   Random number seed =============================>  1                  =
//=   Packet generation rate when on (lambda) ========>  2.0                =
//=   On-to-off rate (alpha) =========================>  1.0                =
//=   Off-to-on rate (beta) ==========================>  1.0                =
//=   Time period to generate samples ================> 15.0                =
//=   --------------------------------------------------------              =
//=   -  Generating samples for 15.00000 seconds...                         =
//=   -    * lambda = 1.000000 customers per second                         =
//=   -    * alpha  = 1.000000 transitions per second                       =
//=   -    * beta   = 1.000000 transations per second                       =
//=   --------------------------------------------------------              =
//=   --------------------------------------------------------              =
//=   -  Done!                                                              =
//=   --------------------------------------------------------              =
//=-------------------------------------------------------------------------=
//= Example output file ("output.dat" for above):                           =
//=                                                                         =
//=   1.619595                                                              =
//=   0.996787                                                              =
//=   0.447229                                                              =
//=   2.783921                                                              =
//=   0.087708                                                              =
//=   0.237619                                                              =
//=   0.105208                                                              =
//=   5.431907                                                              =
//=   0.059742                                                              =
//=   0.458645                                                              =
//=   0.070519                                                              =
//=   0.249380                                                              =
//=   0.298588                                                              =
//=   0.357812                                                              =
//=   0.147305                                                              =
//=   0.872833                                                              =
//=   0.560737                                                              =
//=   0.263869                                                              =
//=-------------------------------------------------------------------------=
//=  Build: gcc genipp.c -lm, bcc32 genipp.c, cl genipp.c                   =
//=-------------------------------------------------------------------------=
//=  Execute: genipp                                                        =
//=-------------------------------------------------------------------------=
//=  Author: Kenneth J. Christensen                                         =
//=          University of South Florida                                    =
//=          WWW: http://www.csee.usf.edu/~christen                         =
//=          Email: christen@csee.usf.edu                                   =
//=-------------------------------------------------------------------------=
//=  History: KJC (10/04/98) - Genesis                                      =
//=           KJC (05/20/03) - Added Jain's RNG for finer granularity       =
//===========================================================================

//----- Include files ---------------------------------------------------------
#include <stdio.h>              // Needed for printf()
#include <stdlib.h>             // Needed for exit() and ato*()
#include <math.h>               // Needed for log()

//----- Function prototypes ---------------------------------------------------
double expon(double x);         // Returns an exponential random variable
double rand_val(int seed);      // Jain's RNG

//===== Main program ==========================================================
void main(void)
{
  char     in_string[32];       // Input string
  FILE     *fp;                 // File pointer to output file
  double   lambda;              // IPP packet generation rate
  double   alpha;               // IPP rate from on to off
  double   beta;                // IPP rate from off to on
  double   ipp_rv;              // IPP random variable
  double   temp, temp1;         // Variables needed for IPP to H2 conversion
  double   lambda1, lambda2;    // Variables needed for IPP to H2 conversion
  double   pi1;                 // Variable needed for IPP to H2 conversion
  double   time_period ;        // Time period to generate arrival samples
  double   sum_time;            // Sum of time upto now
  long int i;                   // Loop counter

  //Output banner
  printf("-------------------------------------------------------- \n");
  printf("-  Program to generate IPP interarrival times            \n");
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
  printf("Random number seed =============================> ");
  scanf("%s", in_string);
  rand_val((int) atoi(in_string));

  // Prompt for packet generation rate (lambda)
  printf("Packet generation rate when on (lambda) ========> ");
  scanf("%s", in_string);
  lambda = atof(in_string);

  // Prompt for on-to-off rate (alpha)
  printf("On-to-off rate (alpha) =========================> ");
  scanf("%s", in_string);
  alpha = atof(in_string);

  // Prompt for off-to-on rate (beta)
  printf("Off-to-on rate (beta) ==========================> ");
  scanf("%s", in_string);
  beta = atof(in_string);

  // Prompt for time period (seconds) to generate samples
  printf("Time period to generate samples ================> ");
  scanf("%s", in_string);
  time_period = atof(in_string);

  // Conversion from IPP to H2
  temp = (lambda + alpha + beta);
  temp1 = (4.0 * lambda * beta);
  lambda1 = 0.5*(temp + sqrt(temp*temp - temp1));
  lambda2 = 0.5*(temp - sqrt(temp*temp - temp1));
  pi1 = (lambda - lambda2)/(lambda1 - lambda2);

  //Output message and generate samples
  printf("-------------------------------------------------------- \n");
  printf("-  Generating samples for %f seconds...   \n", time_period);
  printf("-    * lambda = %f customers per second   \n", lambda);
  printf("-    * alpha  = %f transitions per second \n", alpha);
  printf("-    * beta   = %f transations per second \n", beta);
  printf("-------------------------------------------------------- \n");
  sum_time = 0.0;
  while(1)
  {
    if (rand_val(0) < pi1)
      ipp_rv = expon(1.0 / lambda1);
    else
      ipp_rv = expon(1.0 / lambda2);

    fprintf(fp, "%f \n", ipp_rv);
    sum_time = sum_time + ipp_rv;
    if (sum_time >= time_period) break;
  }

  //Output message and close the outout file
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
