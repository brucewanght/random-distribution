//===================================================== file = gendet.c =====
//=  Program to generate deterministically distributed interarrival times   =
//=   - Generates delta time stamps                                         =
//===========================================================================
//=  Notes: 1) Writes to a user specified output file                       =
//=             * File format is <interarrival time delta>                  =
//=         2) Generates samples for user specified time period             =
//=-------------------------------------------------------------------------=
//= Example user input:                                                     =
//=                                                                         =
//=   --------------------------------------------------------              =
//=   -  Program to generate exponentially distributed                      =
//=   -  interarrival times.                                                =
//=   --------------------------------------------------------              =
//=   Output file name ===================================> output.dat      =
//=   Arrival rate in customers per second (lambda) ======> 1.0             =
//=   Time period to generate interarrival times =========> 15.0            =
//=   --------------------------------------------------------              =
//=   -  Generating samples to file                          -              =
//=   --------------------------------------------------------              =
//=   --------------------------------------------------------              =
//=   -  Done!                                                              =
//=   --------------------------------------------------------              =
//=-------------------------------------------------------------------------=
//= Example output file ("output.dat" for above):                           =
//=                                                                         =
//=   1.000000                                                              =
//=   1.000000                                                              =
//=   1.000000                                                              =
//=   1.000000                                                              =
//=   1.000000                                                              =
//=   1.000000                                                              =
//=   1.000000                                                              =
//=   1.000000                                                              =
//=   1.000000                                                              =
//=   1.000000                                                              =
//=   1.000000                                                              =
//=   1.000000                                                              =
//=   1.000000                                                              =
//=   1.000000                                                              =
//=   1.000000                                                              =
//=-------------------------------------------------------------------------=
//=  Build: gcc gendet.c -lm, bcc32 gendet.c, cl gendet.c                   =
//=-------------------------------------------------------------------------=
//=  Execute: gendet                                                        =
//=-------------------------------------------------------------------------=
//=  Author: Kenneth J. Christensen                                         =
//=          University of South Florida                                    =
//=          WWW: http://www.csee.usf.edu/~christen                         =
//=          Email: christen@csee.usf.edu                                   =
//=-------------------------------------------------------------------------=
//=  History: KJC (10/04/98) - Genesis                                      =
//===========================================================================

//----- Include files -------------------------------------------------------
#include <stdio.h>              // Needed for printf()
#include <stdlib.h>             // Needed for exit() and atof()

//===== Main program ========================================================
void main(void)
{
    FILE     *fp;                 // File pointer to output file
    char     file_name[256];      // Output file name string
    char     temp_string[256];    // Temporary string variable
    double   lambda;              // Mean of arrival rate
    double   det_rv;              // Deterministic random variable
    double   time_period;         // Time period to generate arrival samples
    double   sum_time;            // Sum of time up to now
    long int i;                   // Loop counter

    //Output banner
    printf("----------------------------------------- genexp.c ----- \n");
    printf("-  Program to generate deterministically distributed   - \n");
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

    // Prompt for mean arrival rate (lambda)
    printf("Arrival rate in customers per second (lambda) ======> ");
    scanf("%s", temp_string);
    lambda = atof(temp_string);

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
        det_rv = 1.0 / lambda;
        fprintf(fp, "%f \n", det_rv);
        sum_time = sum_time + det_rv;
        if (sum_time >= time_period) break;
    }

    //Output message and close the output file
    printf("-------------------------------------------------------- \n");
    printf("-  Done! \n");
    printf("-------------------------------------------------------- \n");
    fclose(fp);
}
