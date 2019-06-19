//===================================================== file = generl.c =====
//=  Program to generate Erlang distributed random variables                =
//===========================================================================
//=  Notes: 1) Writes to a user specified output file                       =
//=         2) Generates user specified number of values                    =
//=-------------------------------------------------------------------------=
//= Example user input:                                                     =
//=                                                                         =
//=   ----------------------------------------- generl.c -----              =
//=   -  Program to generate Erlang random variables         -              =
//=   --------------------------------------------------------              =
//=   Output file name ===================================> output.dat      =
//=   Random number seed =================================> 1               =
//=   Number of stages ===================================> 2               =
//=   Rate in customers per second for stage =============> 1.0             =
//=   Number of values to generate =======================> 1000000         =
//=   --------------------------------------------------------              =
//=   -  Generating samples to file                          -              =
//=   --------------------------------------------------------              =
//=   --------------------------------------------------------              =
//=   -  Done!                                                              =
//=   --------------------------------------------------------              =
//=-------------------------------------------------------------------------=
//= Example output file ("output.dat" for above):                           =
//=                                                                         =
//=   1.378647                                                              =
//=   0.105970                                                              =
//=   0.214854                                                              =
//=   0.344399                                                              =
//=   0.045423                                                              =
//=-------------------------------------------------------------------------=
//=  Build: bcc32 generl.c                                                  =
//=-------------------------------------------------------------------------=
//=  Execute: generl                                                        =
//=-------------------------------------------------------------------------=
//=  Author: Kenneth J. Christensen                                         =
//=          University of South Florida                                    =
//=          WWW: http://www.csee.usf.edu/~christen                         =
//=          Email: christen@csee.usf.edu                                   =
//=-------------------------------------------------------------------------=
//=  History: KJC (07/07/03) - Genesis (from genexp.c)                      =
//===========================================================================

//----- Include files -------------------------------------------------------
#include <stdio.h>              // Needed for printf()
#include <stdlib.h>             // Needed for exit() and ato*()
#include <math.h>               // Needed for log()

//----- Constants -----------------------------------------------------------
#define MAX_STAGES  500         // Maximum number of stages

//----- Function prototypes -------------------------------------------------
double expon(double x);         // Returns an exponential random variable
double rand_val(int seed);      // Jain's RNG

//===== Main program ========================================================
void main(void)
{
  FILE     *fp;                 // File pointer to output file
  char     file_name[256];      // Output file name string
  char     temp_string[256];    // Temporary string variable
  double   lambda[MAX_STAGES];  // Mean of rates for stages
  double   exp_rv;              // Exponential random variable
  double   erl_rv;              // Erlang random variable
  int      num_values;          // Number of values
  int      num_stages;          // Number of stages
  int      i,j;                 // Loop counters

  // Output banner
  printf("----------------------------------------- generl.c ----- \n");
  printf("-  Program to generate Erlang random variables         - \n");
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
  printf("Random number seed (greater than 0) ================> ");
  scanf("%s", temp_string);
  rand_val((int) atoi(temp_string));

  // Prompt for number of stages
  printf("Number of stages ===================================> ");
  scanf("%s", temp_string);
  num_stages = atoi(temp_string);
  if (num_stages > MAX_STAGES)
  {
    printf("ERROR - too many stages (max stages is %d) \n", MAX_STAGES);
    exit(1);
  }

  // Prompt for stage rate and assign the rates (lambda[])
  printf("Rate in customers per second for stage =============> ");
  scanf("%s", temp_string);
  lambda[0] = atof(temp_string);
  for (i=1; i<num_stages; i++)
    lambda[i] = lambda[0];

  // Prompt for number of values to generate
  printf("Number of values to generate =======================> ");
  scanf("%s", temp_string);
  num_values = atoi(temp_string);

  //Output message and generate interarrival times
  printf("-------------------------------------------------------- \n");
  printf("-  Generating samples to file                          - \n");
  printf("-------------------------------------------------------- \n");

  // Generate and output Erlang random variables
  //  - Erlang random variable is a sum of exponential random variables
  for (i=0; i<num_values; i++)
  {
    erl_rv = 0.0;
    for (j=0; j<num_stages; j++)
    {
      exp_rv = expon(1.0 / lambda[j]);
      erl_rv = erl_rv + exp_rv;
    }
    fprintf(fp, "%f \n", erl_rv);
  }

  //Output message and close the output file
  printf("-------------------------------------------------------- \n");
  printf("-  Done! \n");
  printf("-------------------------------------------------------- \n");
  fclose(fp);
}

//===========================================================================
//=  Function to generate exponentially distributed random variables        =
//=    - Input:  Mean value of distribution                                 =
//=    - Output: Returns with exponentially distributed random variable     =
//===========================================================================
double expon(double x)
{
  double z;                     // Uniform random number (0 < z < 1)
  double exp_value;             // Computed exponential value to be returned

  // Pull a uniform random number (0 < z < 1)
  do
  {
    z = rand_val(0);
  }
  while ((z == 0) || (z == 1));

  // Compute exponential random variable using inversion method
  exp_value = -x * log(z);

  return(exp_value);
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
