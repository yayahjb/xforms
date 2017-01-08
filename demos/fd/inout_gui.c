/* Form definition file generated by fdesign on Sat Jan  7 23:14:17 2017 */

#include <stdlib.h>
#include "inout_gui.h"


/***************************************
 ***************************************/

FD_f *
create_form_f( void )
{
    FL_OBJECT *obj;
    FD_f *fdui = ( FD_f * ) fl_malloc( sizeof *fdui );

    fdui->vdata = fdui->cdata = NULL;
    fdui->ldata = 0;

    fdui->f = fl_bgn_form( FL_NO_BOX, 637, 548 );

    obj = fl_add_box( FL_FLAT_BOX, 0, 0, 637, 548, "" );

    obj = fl_add_labelframe( FL_ENGRAVED_FRAME, 140, 230, 320, 100, "PLC managing the process and holding the values" );

    obj = fl_add_text( FL_NORMAL_TEXT, 160, 250, 100, 30, "process value 1" );

    fdui->plc_pv1 = obj = fl_add_text( FL_NORMAL_TEXT, 290, 250, 150, 30, "0" );
    fl_set_object_boxtype( obj, FL_DOWN_BOX );
    fl_set_object_color( obj, FL_MCOL, FL_MCOL );
    fl_set_object_lsize( obj, 16 );
    fl_set_object_lalign( obj, FL_ALIGN_CENTER );

    obj = fl_add_text( FL_NORMAL_TEXT, 160, 290, 100, 30, "process value 2" );

    fdui->plc_pv2 = obj = fl_add_text( FL_NORMAL_TEXT, 290, 285, 150, 30, "0" );
    fl_set_object_boxtype( obj, FL_DOWN_BOX );
    fl_set_object_color( obj, FL_MCOL, FL_MCOL );
    fl_set_object_lsize( obj, 16 );
    fl_set_object_lalign( obj, FL_ALIGN_CENTER );

    obj = fl_add_labelframe( FL_ENGRAVED_FRAME, 35, 40, 220, 145, "terminal 1 (never stops)" );

    fdui->term1_pv1 = obj = fl_add_input( FL_INT_INPUT, 150, 70, 90, 35, "process value 1" );
    fl_set_object_color( obj, FL_YELLOW, FL_WHEAT );
    fl_set_object_callback( obj, cb_write_to_plc, 1 );

    fdui->term1_pv2 = obj = fl_add_input( FL_INT_INPUT, 150, 110, 90, 35, "process value 2" );
    fl_set_object_color( obj, FL_YELLOW, FL_WHEAT );
    fl_set_object_callback( obj, cb_write_to_plc, 2 );

    obj = fl_add_labelframe( FL_ENGRAVED_FRAME, 335, 40, 220, 145, "terminal 2 (stops with work around)" );

    fdui->term2_pv1 = obj = fl_add_input( FL_INT_INPUT, 450, 70, 90, 35, "process value 1" );
    fl_set_object_color( obj, FL_YELLOW, FL_WHEAT );
    fl_set_object_callback( obj, cb_write_to_plc, 1 );

    fdui->term2_pv2 = obj = fl_add_input( FL_INT_INPUT, 450, 110, 90, 35, "process value 2" );
    fl_set_object_color( obj, FL_YELLOW, FL_WHEAT );
    fl_set_object_callback( obj, cb_write_to_plc, 2 );

    obj = fl_add_labelframe( FL_ENGRAVED_FRAME, 45, 375, 415, 145, "terminal 3 using InOut" );

    fdui->term3_pv1 = obj = fl_add_input( FL_INT_INPUT, 157, 405, 88, 35, "process value 1" );
    fl_set_object_color( obj, FL_YELLOW, FL_WHEAT );
    fl_set_object_callback( obj, cb_write_to_plc, 1 );

    fdui->term3_pv2 = obj = fl_add_input( FL_INT_INPUT, 157, 445, 88, 35, "process value 2" );
    fl_set_object_color( obj, FL_YELLOW, FL_WHEAT );
    fl_set_object_callback( obj, cb_write_to_plc, 2 );

    obj = fl_add_button( FL_NORMAL_BUTTON, 545, 495, 80, 40, "End" );

    fdui->term3_onoff = obj = fl_add_choice( FL_NORMAL_CHOICE2, 315, 430, 105, 25, "InOut mode" );
    fl_set_object_lalign( obj, FL_ALIGN_TOP );
    fl_set_object_callback( obj, cb_term3_onoff, 0 );
    fl_addto_choice( obj, "ON" );
    fl_addto_choice( obj, "OFF" );
    fl_set_choice( obj, 1 );

    obj = fl_add_text( FL_NORMAL_TEXT, 35, 195, 550, 20, "change of values of one central and active controller from multiple terminals" );

    fl_end_form( );

    fdui->f->fdui = fdui;

    return fdui;
}
