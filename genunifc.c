//=================================================== file = genunifc.c =====
//=  Program to generate continuous uniformly distributed random variables  =
//===========================================================================
//=  Notes: 1) Writes to a user specified output file                       =
//=         2) Generates user specified number of values                    =
//=-------------------------------------------------------------------------=
//= Example user input:                                                     =
//=                                                                         =
//=  --------------------------------------- genunifc.c -----               =
//=  -  Program to generate continuous uniform RVs          -               =
//=  --------------------------------------------------------               =
//=  Output file name ===================================> x                =
//=  Random number seed =================================> 1                =
//=  Min value (continuous) =============================> 1.0              =
//=  Max value (continuous) =============================> 2.0              =
//=  Number of values to generate =======================> 5                =
//=  --------------------------------------------------------               =
//=  -  Generating samples to file                          -               =
//=  --------------------------------------------------------               =
//=  --------------------------------------------------------               =
//=  -  Done!                                                               =
//=  --------------------------------------------------------               =                                                             =
//=-------------------------------------------------------------------------=
//= Example output file ("output.dat" for above):                           =
//=                                                                         =
//=  1.000008                                                               =
//=  1.131538                                                               =
//=  1.755605                                                               =
//=  1.458650                                                               =
//=  1.532767                                                               =
//=-------------------------------------------------------------------------=
//=  Build: bcc32 genunifc.c                                                =
//=-------------------------------------------------------------------------=
//=  Execute: genunifc                                                      =
//=-------------------------------------------------------------------------=
//=  Author: Ken Christensen                                                =
//=          University of South Florida                                    =
//=          WWW: http://www.csee.usf.edu/~christen                         =
//=          Email: christen@csee.usf.edu                                   =
//=-------------------------------------------------------------------------=
//=  History: KJC (12/31/00) - Genesis (from genexp.c)                      =
//=           KJC (05/20/03) - Added Jain's RNG for finer granularity       =
//=           KJC (01/26/07) - Cleaned-up                                   =
//===========================================================================

//----- Include files -------------------------------------------------------
#include <stdio.h>              // Needed for printf()
#include <stdlib.h>             // Needed for exit() and ato*()

//----- Function prototypes -------------------------------------------------
double unifc(double min, double max);  // Returns a continuous uniform RV
double rand_valc(int seed);            // Jain's RNG to return 0 < z < 1

//===== Main program ========================================================
void main(void)
{
    FILE     *fp;                 // File pointer to output file
    char     file_name[256];      // Output file name string
    char     temp_string[256];    // Temporary string variable
    double   min;                 // Minimum value
    double   max;                 // Maximum value
    double   unif_rv;             // Uniformly random variable
    int      num_values;          // Number of values
    int      i;                   // Loop counter

    // Output banner
    printf("--------------------------------------- genunifc.c ----- \n");
    printf("-  Program to generate continuous uniform RVs          - \n");
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
    rand_valc((int) atoi(temp_string));

    // Prompt for min value
    printf("Min value (continuous) =============================> ");
    scanf("%s", temp_string);
    min = atof(temp_string);

    // Prompt for max value
    printf("Max value (continuous) =============================> ");
    scanf("%s", temp_string);
    max = atof(temp_string);

    // Prompt for number of values to generate
    printf("Number of values to generate =======================> ");
    scanf("%s", temp_string);
    num_values = atoi(temp_string);

    //Output message and generate interarrival times
    printf("-------------------------------------------------------- \n");
    printf("-  Generating samples to file                          - \n");
    printf("-------------------------------------------------------- \n");

    // Generate and output interarrival times
    for (i = 0; i < num_values; i++)
    {
        unif_rv = unifc(min, max);
        fprintf(fp, "%f \n", unif_rv);
    }

    //Output message and close the output file
    printf("-------------------------------------------------------- \n");
    printf("-  Done! \n");
    printf("-------------------------------------------------------- \n");
    fclose(fp);
}

//===========================================================================
//=  Function to generate uniformly distributed random variables            =
//=    - Input:  Min and max values                                         =
//=    - Output: Returns with uniformly distributed random variable         =
//===========================================================================
double unifc(double min, double max)
{
    double z;                     // Uniform random number (0 < z < 1)
    double unif_value;            // Computed uniform value to be returned

    // Pull a uniform random value (0 < z < 1)
    z = rand_valc(0);

    // Compute uniform continuous random variable using inversion method
    unif_value = z * (max - min) + min;

    return(unif_value);
}

//=========================================================================
//= Multiplicative LCG for generating uniform(0.0, 1.0) random numbers    =
//=   - From R. Jain, "The Art of Computer Systems Performance Analysis," =
//=     John Wiley & Sons, 1991. (Page 443, Figure 26.2)                  =
//=========================================================================
double rand_valc(int seed)
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
