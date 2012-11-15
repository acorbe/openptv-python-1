/* Implementation for parameters handling routines. */

#include "parameters.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* read_sequence_par() reads sequence parameters from a config file with the
   following format: each line is a value, first 4 values are image names,
   5th is the first number in the sequence, 6th line is the last value in the
   sequence.
   
   Arguments:
   char *filename - path to the text file containing the parameters.
   
   Returns:
   Pointer to a newly-allocated sequence_par structure. Don't forget to free
   the new memory that is allocated for the image names. If reading failed for 
   any reason, returns NULL.
*/
sequence_par* read_sequence_par(char *filename) {
    char line[SEQ_FNAME_MAX_LEN];
    FILE* par_file;
    int cam, read_ok;
    sequence_par *ret;
    
    par_file = fopen(filename, "r");
    if (par_file == NULL) {
        return NULL;
    }
    
    ret = (sequence_par *) malloc(sizeof(sequence_par));
    ret->img_base_name = (char **) calloc(4, sizeof(char *));
    
    /* Note the assumption of 4 cameras. Fixing this requires changing the
       file format. */
    for (cam = 0; cam < 4; cam++) {
        read_ok = fscanf(par_file, "%s\n", line);
        if (read_ok == 0) goto handle_error;
        
        ret->img_base_name[cam] = (char *) malloc(SEQ_FNAME_MAX_LEN);
        strncpy(ret->img_base_name[cam], line, SEQ_FNAME_MAX_LEN);
    }
    
    if( (read_ok = fscanf(par_file, "%d\n", &(ret->first))) == 0)
        goto handle_error;
    if( (read_ok = fscanf(par_file, "%d\n", &(ret->last))) == 0)    
        goto handle_error;

    fclose(par_file);
    return ret;
    
handle_error:
    printf("Error reading sequence parameters from %s\n", filename);
    free(ret);
    fclose(par_file);
    return NULL;
}

/* read_track_par() reads tracking parameters from a config file with the
   following format: each line is a value, in this order:
   1. dvxmin
   2. dvxmax
   3. dvymin
   4. dvymax
   5. dvzmin
   6. dvzmax
   7. dangle
   8. dacc
   9. add
   
   Arguments:
   char *filename - path to the text file containing the parameters.
   
   Returns:
   Pointer to a newly-allocated track_par structure. If reading failed for 
   any reason, returns NULL.
*/
track_par* read_track_par(char *filename) {
    FILE* fpp;
    track_par *ret = (track_par *) malloc(sizeof(track_par));
    
    fpp = fopen(filename, "r");
    if(fscanf(fpp, "%lf\n", &(ret->dvxmin)) == 0) goto handle_error;
    if(fscanf(fpp, "%lf\n", &(ret->dvxmax)) == 0) goto handle_error;
    if(fscanf(fpp, "%lf\n", &(ret->dvymin)) == 0) goto handle_error;
    if(fscanf(fpp, "%lf\n", &(ret->dvymax)) == 0) goto handle_error;
    if(fscanf(fpp, "%lf\n", &(ret->dvzmin)) == 0) goto handle_error;
    if(fscanf(fpp, "%lf\n", &(ret->dvzmax)) == 0) goto handle_error;
    if(fscanf(fpp, "%lf\n", &(ret->dangle)) == 0) goto handle_error;
    if(fscanf(fpp, "%lf\n", &(ret->dacc)) == 0) goto handle_error;
    if(fscanf(fpp, "%d\n", &(ret->add)) == 0) goto handle_error;
    fclose (fpp);
    
    ret->dsumg = ret->dn = ret->dnx = ret->dny = 0;
    return ret;

handle_error:
    free(ret);
    fclose (fpp);
    return NULL;
}

/* compare_track_par() checks that all fields of two track_par objects are
   equal.
   
   Arguments:
   track_par *t1, track_par *t2 - addresses of the objects for comparison.
   
   Returns:
   True if equal, false otherwise.
*/
int compare_track_par(track_par *t1, track_par *t2) {
    return ((t1->dvxmin == t2->dvxmin) && (t1->dvxmax == t2->dvxmax) && \
        (t1->dvymin == t2->dvymin) && (t1->dvymax == t2->dvymax) && \
        (t1->dvzmin == t2->dvzmin) && (t1->dvzmax == t2->dvzmax) && \
        (t1->dacc == t2->dacc) && (t1->dangle == t2->dangle) && \
        (t1->dsumg == t2->dsumg) && (t1->dn == t2->dn) && \
        (t1->dnx == t2->dnx) && (t1->dny == t2->dny) && (t1->add == t2->add));
}
