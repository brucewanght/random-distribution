//===================================================== file = genemp.c =====
//=  Program to generate empirically distributed random variables           =
//===========================================================================
//=  Notes: 1) Writes to a user specified output file                       =
//=         2) Generates user specified number of samples                   =
//=         3) The empirical distribution is assume to exist in a file      =
//=            named "dist.dat".  The format of dist.dat is set of doubles  =
//=            in column order.  Each double is the probability of          =
//=            occurence and the value of the RV.  The probability          =
//=            values must sum to 1.0.  See the example below.              =
//=-------------------------------------------------------------------------=
//= Example user input:                                                     =
//=                                                                         =
//=   ----------------------------------------- genemp.c -----              =
//=   -  Program to generate empirically distributed random  -              =
//=   -  variables (empirical distribution in file dist.dat) -              =
//=   --------------------------------------------------------              =
//=   Output file name ===================================> output.dat      =
//=   Random number seed =================================> 1               =
//=   Number of samples to generate ======================> 5               =
//=   --------------------------------------------------------              =
//=   -  Building the empirical CDF                          -              =
//=   --------------------------------------------------------              =
//=   --------------------------------------------------------              =
//=   -  Generating samples to file                          -              =
//=   --------------------------------------------------------              =
//=   --------------------------------------------------------              =
//=   -  Done!                                                              =
//=   --------------------------------------------------------              =
//=-------------------------------------------------------------------------=
//= Example input file (this is the required file "dist.dat"                =
//=                                                                         =
//=   0.25  5.00                                                            =
//=   0.45  2.00                                                            =
//=   0.15  1.00                                                            =
//=   0.10  0.50                                                            =
//=   0.05  0.25                                                            =
//=                                                                         =
//= Example output (from above input file and user input):                  =
//=                                                                         =
//=   5.000000                                                              =
//=   5.000000                                                              =
//=   1.000000                                                              =
//=   2.000000                                                              =
//=   2.000000                                                              =
//=-------------------------------------------------------------------------=
//=  Build: gcc genemp.c, bcc32 genemp.c, cl genemp.c                       =
//=-------------------------------------------------------------------------=
//=  Execute: genexp (must have a file "dist.dat" in same directory)        =
//=-------------------------------------------------------------------------=
//=  Author: Kenneth J. Christensen                                         =
//=          University of South Florida                                    =
//=          WWW: http://www.csee.usf.edu/~christen                         =
//=          Email: christen@csee.usf.edu                                   =
//=-------------------------------------------------------------------------=
//=  History: KJC (12/30/00) - Genesis (from genexp.c)                      =
//=           KJC (01/29/01) - Changed Num_entries to be an int variable    =
//=           KJC (03/12/03) - Added Jain's RNG for finer granularity       =
//===========================================================================

//----- Include files -------------------------------------------------------
#include <stdio.h>              // Needed for printf()
#include <stdlib.h>             // Needed for exit() and ato*()

//----- Defines -------------------------------------------------------------
#define DIST_FILE  "dist.dat"   // Distribution file
#define MAX_ENTRY       1000    // Maximum number of lines in DIST_FILE

//----- Globals -------------------------------------------------------------
struct rv                       // Structure for an empirical RV
{
  double cdf_val;               // *** Probability value
  double rv_val;                // *** RV value
};
struct rv CDF[MAX_ENTRY];       // The CDF built from DIST_FILE
int       Num_entries;          // The number of entries in the CDF

//----- Function prototypes -------------------------------------------------
double emp(void);               // Returns an empirical random variable
double rand_val(int seed);      // Jain's RNG

//===== Main program ========================================================
void main(void)
{
  FILE     *fp_dist;              // File pointer to distribution file
  FILE     *fp_out;               // File pointer to output file
  char     instring1[80];         // Input string #1
  char     instring2[80];         // Input string #2
  double   num_samples;           // Number of samples to generate
  double   emp_rv;                // Exponential random variable
  int      i;                     // Loop counter

  // Output banner
  printf("----------------------------------------- genemp.c ----- \n");
  printf("-  Program to generate empirically distributed random  - \n");
  printf("-  variables (empirical distribution in file dist.dat) - \n");
  printf("-------------------------------------------------------- \n");

  // Open the distribution file
  fp_dist = fopen(DIST_FILE, "r");
  if (fp_dist == NULL)
  {
    printf("ERROR in opening the distributin file (%s) \n", DIST_FILE);
    exit(1);
  }

  // Build the CDF from the distribution file
  Num_entries = 0;
  fscanf(fp_dist, "%s %s \n", instring1, instring2);
  CDF[Num_entries].cdf_val = atof(instring1);
  CDF[Num_entries].rv_val = atof(instring2);
  while (!feof(fp_dist))
  {
    Num_entries++;
    fscanf(fp_dist, "%s %s \n", instring1, instring2);
    CDF[Num_entries].cdf_val = atof(instring1) + CDF[Num_entries-1].cdf_val;
    CDF[Num_entries].rv_val = atof(instring2);
  }

  // Verify that last cdf_val in the CDF is 1.0 (sum of probabilities = 1.0)
  if (CDF[Num_entries].cdf_val != 1.0)
  {
    printf("ERROR - sum of probabilities in %s is %f (must be 1.0) \n",
      DIST_FILE, CDF[Num_entries].cdf_val );
    exit(1);
  }

  // Prompt for output filename and then create/open the file
  printf("Output file name ===================================> ");
  scanf("%s", instring1);
  fp_out = fopen(instring1, "w");
  if (fp_out == NULL)
  {
    printf("ERROR in creating output file (%s) \n", instring1);
    exit(1);
  }

  // Prompt for random number seed and then use it
  printf("Random number seed =================================> ");
  scanf("%s", instring1);
  rand_val((int) atoi(instring1));

  // Prompt for number of samples to generate
  printf("Number of samples to generate ======================> ");
  scanf("%s", instring1);
  num_samples = atoi(instring1);

  // Output message and build the empriical CDF
  printf("-------------------------------------------------------- \n");
  printf("-  Building the empirical CDF                          - \n");
  printf("-------------------------------------------------------- \n");

  // Output message and generate interarrival times
  printf("-------------------------------------------------------- \n");
  printf("-  Generating samples to file                          - \n");
  printf("-------------------------------------------------------- \n");
  for (i=0; i<num_samples; i++)
  {
    emp_rv = emp();
    fprintf(fp_out, "%f \n", emp_rv);
  }

  // Output message and close the distribution and output files
  printf("-------------------------------------------------------- \n");
  printf("-  Done! \n");
  printf("-------------------------------------------------------- \n");
  fclose(fp_dist);
  fclose(fp_out);
}

//===========================================================================
//=  Function to generate empirically distributed random variables          =
//=    - Input: None                                                        =
//=    - Output: Returns with empirically distributed random variable       =
//===========================================================================
double emp(void)
{
  double z;                     // Uniform random number (0 < z < 1)
  double emp_value;             // Computed exponential value to be returned
  int    i;                     // Loop counter

  // Pull a uniform random number (0 < z < 1)
  z = rand_val(0);

  // Map z to empirical distribution and get empirical RV value
  for (i=0; i<=Num_entries; i++)
    if (z <= CDF[i].cdf_val)
    {
      emp_value = CDF[i].rv_val;
      break;
    }

  // Return the empirical RV value
  return(emp_value);
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
