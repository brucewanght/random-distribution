//==================================================== file = genzipf.c =====
//=  Program to generate Zipf (power law) distributed random variables      =
//===========================================================================
//=  Notes: 1) Writes to a user specified output file                       =
//=         2) Generates user specified number of values                    =
//=         3) Run times is same as an empirical distribution generator     =
//=         4) Implements p(i) = C/i^alpha for i = 1 to N where C is the    =
//=            normalization constant (i.e., sum of p(i) = 1).              =
//=-------------------------------------------------------------------------=
//= Example user input:                                                     =
//=                                                                         =
//=   ---------------------------------------- genzipf.c -----              =
//=   -     Program to generate Zipf random variables        -              =
//=   --------------------------------------------------------              =
//=   Output file name ===================================> output.dat      =
//=   Random number seed =================================> 1               =
//=   Alpha vlaue ========================================> 1.0             =
//=   N value ============================================> 1000            =
//=   Number of values to generate =======================> 5               =
//=   --------------------------------------------------------              =
//=   -  Generating samples to file                          -              =
//=   --------------------------------------------------------              =
//=   --------------------------------------------------------              =
//=   -  Done!                                                              =
//=   --------------------------------------------------------              =
//=-------------------------------------------------------------------------=
//= Example output file ("output.dat" for above):                           =
//=                                                                         =
//=   1                                                                     =
//=   1                                                                     =
//=   161                                                                   =
//=   17                                                                    =
//=   30                                                                    =
//=-------------------------------------------------------------------------=
//=  Build: bcc32 genzipf.c                                                 =
//=-------------------------------------------------------------------------=
//=  Execute: genzipf                                                       =
//=-------------------------------------------------------------------------=
//=  Author: Kenneth J. Christensen                                         =
//=          University of South Florida                                    =
//=          WWW: http://www.csee.usf.edu/~christen                         =
//=          Email: christen@csee.usf.edu                                   =
//=-------------------------------------------------------------------------=
//=  History: KJC (11/16/03) - Genesis (from genexp.c)                      =
//===========================================================================
//----- Include files -------------------------------------------------------
#include <assert.h>             // Needed for assert() macro
#include <stdio.h>              // Needed for printf()
#include <stdlib.h>             // Needed for exit() and ato*()
#include <math.h>               // Needed for pow()
#include <stdint.h>
#include <time.h>

//----- Constants -----------------------------------------------------------
#define  FALSE          0       // Boolean false
#define  TRUE           1       // Boolean true

//----- Function prototypes -------------------------------------------------
uint64_t zipf(double alpha, uint64_t n);  // Returns a Zipf random variable
double   rand_val(uint64_t seed);         // Jain's RNG

//===== Main program ========================================================
int main(void)
{
    FILE   *fp;                   // File pouint64_ter to output file
    char   file_name[256];        // Output file name string
    char   temp_string[256];      // Temporary string variable
    double alpha;                 // Alpha parameter
    double n;                     // N parameter
    uint64_t    num_values;            // Number of values
    uint64_t    zipf_rv;               // Zipf random variable
    uint64_t    i;                     // Loop counter

    // Output banner
    printf("---------------------------------------- genzipf.c ----- \n");
    printf("-     Program to generate Zipf random variables        - \n");
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
    rand_val((uint64_t) atoi(temp_string));

    // Prompt for alpha value
    printf("Alpha value ========================================> ");
    scanf("%s", temp_string);
    alpha = atof(temp_string);

    // Prompt for N value
    printf("N value (biggest value in this test) ===============> ");
    scanf("%s", temp_string);
    n = atoll(temp_string);

    // Prompt for number of values to generate
    printf("Number of values to generate =======================> ");
    scanf("%s", temp_string);
    num_values = atoi(temp_string);

    // Output "generating" message
    printf("-------------------------------------------------------- \n");
    printf("-  Generating samples to file                          - \n");
    printf("-------------------------------------------------------- \n");

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    // Generate and output zipf random variables
    for (i = 0; i < num_values; i++)
    {
        zipf_rv = zipf(alpha, n);
        fprintf(fp, "%lu \n", zipf_rv);
        printf("vlaue = %lu, count = %lu\n", zipf_rv, i);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    // Output "done" message and close the output file
    printf("-------------------------------------------------------- \n");
    printf("-  Done! \n");
    long long elapsed_time = (end.tv_sec - start.tv_sec) * 1000000000
                             + (end.tv_nsec - start.tv_nsec);
    elapsed_time /= 1000;
    printf("elapsed_time: %lld, Avg: %f\n", elapsed_time,
           (double)elapsed_time / num_values);
    printf("-------------------------------------------------------- \n");
    fclose(fp);
}

//===========================================================================
//=  Function to generate Zipf (power law) distributed random variables     =
//=    - Input: alpha and N                                                 =
//=    - Output: Returns with Zipf distributed random variable              =
//===========================================================================
uint64_t zipf(double alpha, uint64_t n)
{
    static uint64_t first = TRUE; // Static first time flag
    static double c = 0;          // Normalization constant
    static double *sum_probs;     // Pre-calculated sum of probabilities
    double z;                     // Uniform random number (0 < z < 1)
    uint64_t zipf_value;          // Computed exponential value to be returned
    uint64_t i;                   // Loop counter
    uint64_t low, high, mid;      // Binary-search bounds

    // Compute normalization constant on first call only
    if (first == TRUE)
    {
        for (i = 1; i <= n; i++)
            c = c + (1.0 / pow((double) i, alpha));
        c = 1.0 / c;

        sum_probs = (double *)malloc((n + 1) * sizeof(*sum_probs));
        sum_probs[0] = 0;
        for (i = 1; i <= n; i++)
        {
            sum_probs[i] = sum_probs[i - 1] + c / pow((double) i, alpha);
        }
        first = FALSE;
    }

    // Pull a uniform random number (0 < z < 1)
    do
    {
        z = rand_val(0);
    }
    while ((z == 0) || (z == 1));

    // Map z to the value
    low = 1, high = n, mid;
    do
    {
        mid = floor((low + high) / 2);
        if (sum_probs[mid] >= z && sum_probs[mid - 1] < z)
        {
            zipf_value = mid;
            break;
        }
        else if (sum_probs[mid] >= z)
        {
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }
    while (low <= high);

    // Assert that zipf_value is between 1 and N
    assert((zipf_value >= 1) && (zipf_value <= n));

    return(zipf_value);
}
//=========================================================================
//= Multiplicative LCG for generating uniform(0.0, 1.0) random numbers    =
//=   - x_n = 7^5*x_(n-1)mod(2^31 - 1)                                    =
//=   - With x seeded to 1 the 10000th x value should be 1043618065       =
//=   - From R. Jain, "The Art of Computer Systems Performance Analysis," =
//=     John Wiley & Sons, 1991. (Page 443, Figure 26.2)                  =
//=========================================================================
double rand_val(uint64_t seed)
{
    const long  a =      16807;  // Multiplier
    const long  m = 2147483647;  // Modulus
    const long  q =     127773;  // m div a
    const long  r =       2836;  // m mod a
    static long x;               // Random uint64_t value
    long        x_div_q;         // x divided by q
    long        x_mod_q;         // x modulo q
    long        x_new;           // New x value

    // Set the seed if argument is non-zero and then return zero
    if (seed > 0)
    {
        x = seed;
        return(0.0);
    }

    // RNG using uint64_t arithmetic
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
