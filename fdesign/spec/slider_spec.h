/** Header file generated by fdesign on Wed May  7 22:23:18 2008.**/

#ifndef FD_sliderattrib_h_
#define FD_sliderattrib_h_

/** Callbacks, globals and object handlers **/

extern void adjust_precision( FL_OBJECT *, long );
extern void minmax_change( FL_OBJECT *, long );
extern void returnsetting_change( FL_OBJECT *, long );
extern void initialvalue_change( FL_OBJECT *, long );
extern void slsize_change( FL_OBJECT *, long );
extern void step_change( FL_OBJECT *, long );
extern void increment_change( FL_OBJECT *, long );


/**** Forms and Objects ****/

typedef struct {
    FL_FORM   * sliderattrib;
    void      * vdata;
    char      * cdata;
    long        ldata;
    FL_OBJECT * background;
    FL_OBJECT * prec;
    FL_OBJECT * minval;
    FL_OBJECT * maxval;
    FL_OBJECT * returnsetting;
    FL_OBJECT * initial_val;
    FL_OBJECT * slsize;
    FL_OBJECT * step;
    FL_OBJECT * ldelta;
    FL_OBJECT * rdelta;
} FD_sliderattrib;

extern FD_sliderattrib * create_form_sliderattrib( void );

#endif /* FD_sliderattrib_h_ */
