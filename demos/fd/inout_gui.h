/* Header file generated by fdesign on Thu Jul 20 12:31:34 2017 */

#ifndef FD_f_h_
#define FD_f_h_

#include <forms.h>

#if defined __cplusplus
extern "C"
{
#endif

/* Callbacks, globals and object handlers */

void cb_write_to_plc( FL_OBJECT *, long );
void cb_term3_onoff( FL_OBJECT *, long );


/* Forms and Objects */

typedef struct {
    FL_FORM   * f;
    void      * vdata;
    char      * cdata;
    long        ldata;
    FL_OBJECT * plc_pv1;
    FL_OBJECT * plc_pv2;
    FL_OBJECT * term1_pv1;
    FL_OBJECT * term1_pv2;
    FL_OBJECT * term2_pv1;
    FL_OBJECT * term2_pv2;
    FL_OBJECT * term3_pv1;
    FL_OBJECT * term3_pv2;
    FL_OBJECT * term3_onoff;
} FD_f;

FD_f * create_form_f( void );

#if defined __cplusplus
}
#endif

#endif /* FD_f_h_ */
