/* Form definition file generated by fdesign on Thu Jul 20 12:31:34 2017 */

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

    fdui->f = fl_bgn_form( FL_NO_BOX, 737, 693 );

    obj = fl_add_box( FL_FLAT_BOX, 0, 0, 737, 693, "" );

    obj = fl_add_labelframe( FL_ENGRAVED_FRAME, 180, 405, 320, 100, "PLC managing the process and holding the values" );

    obj = fl_add_text( FL_NORMAL_TEXT, 200, 425, 100, 30, "process value 1" );

    fdui->plc_pv1 = obj = fl_add_text( FL_NORMAL_TEXT, 330, 425, 150, 30, "0" );
    fl_set_object_boxtype( obj, FL_DOWN_BOX );
    fl_set_object_color( obj, FL_MCOL, FL_MCOL );
    fl_set_object_lsize( obj, 16 );
    fl_set_object_lalign( obj, FL_ALIGN_CENTER );

    obj = fl_add_text( FL_NORMAL_TEXT, 200, 465, 100, 30, "process value 2" );

    fdui->plc_pv2 = obj = fl_add_text( FL_NORMAL_TEXT, 330, 460, 150, 30, "0" );
    fl_set_object_boxtype( obj, FL_DOWN_BOX );
    fl_set_object_color( obj, FL_MCOL, FL_MCOL );
    fl_set_object_lsize( obj, 16 );
    fl_set_object_lalign( obj, FL_ALIGN_CENTER );

    obj = fl_add_labelframe( FL_ENGRAVED_FRAME, 75, 265, 220, 95, "terminal 1 (never stops)" );

    fdui->term1_pv1 = obj = fl_add_input( FL_INT_INPUT, 190, 275, 90, 35, "process value 1" );
    fl_set_object_color( obj, FL_YELLOW, FL_WHEAT );
    fl_set_object_callback( obj, cb_write_to_plc, 1 );

    fdui->term1_pv2 = obj = fl_add_input( FL_INT_INPUT, 190, 315, 90, 35, "process value 2" );
    fl_set_object_color( obj, FL_YELLOW, FL_WHEAT );
    fl_set_object_callback( obj, cb_write_to_plc, 2 );

    obj = fl_add_labelframe( FL_ENGRAVED_FRAME, 375, 265, 220, 95, "terminal 2 (stops with work around)" );

    fdui->term2_pv1 = obj = fl_add_input( FL_INT_INPUT, 490, 275, 90, 35, "process value 1" );
    fl_set_object_color( obj, FL_YELLOW, FL_WHEAT );
    fl_set_object_callback( obj, cb_write_to_plc, 1 );

    fdui->term2_pv2 = obj = fl_add_input( FL_INT_INPUT, 490, 315, 90, 35, "process value 2" );
    fl_set_object_color( obj, FL_YELLOW, FL_WHEAT );
    fl_set_object_callback( obj, cb_write_to_plc, 2 );

    obj = fl_add_labelframe( FL_ENGRAVED_FRAME, 145, 525, 415, 100, "terminal 3 using InOut" );

    fdui->term3_pv1 = obj = fl_add_input( FL_INT_INPUT, 257, 520, 88, 35, "process value 1" );
    fl_set_object_color( obj, FL_YELLOW, FL_WHEAT );
    fl_set_object_callback( obj, cb_write_to_plc, 1 );

    fdui->term3_pv2 = obj = fl_add_input( FL_INT_INPUT, 257, 580, 88, 35, "process value 2" );
    fl_set_object_color( obj, FL_YELLOW, FL_WHEAT );
    fl_set_object_callback( obj, cb_write_to_plc, 2 );

    obj = fl_add_button( FL_NORMAL_BUTTON, 645, 620, 80, 40, "End" );

    fdui->term3_onoff = obj = fl_add_choice( FL_NORMAL_CHOICE2, 415, 565, 105, 25, "InOut mode" );
    fl_set_object_lalign( obj, FL_ALIGN_TOP );
    fl_set_object_callback( obj, cb_term3_onoff, 0 );
    fl_addto_choice( obj, "ON" );
    fl_addto_choice( obj, "OFF" );
    fl_set_choice( obj, 1 );

    obj = fl_add_text( FL_NORMAL_TEXT, 75, 370, 550, 20, "change of values of one central and active controller from multiple terminals" );

    obj = fl_add_text( FL_NORMAL_TEXT, 30, 10, 555, 35, "This program demonstrates why inout-mode is necessary." );
    fl_set_object_lsize( obj, FL_LARGE_SIZE );

    obj = fl_add_text( FL_NORMAL_TEXT, 20, 45, 695, 185, "All yellow fields are cyclically updated by an idle task, getting the values from the PLC.\nEdit the fields of 'terminal 1' is not possible since they are always overwritten by the idle task.\n\n'terminal 2': If you try to block the update by using 'obj->belowmouse' it will not been updated if the cursor\naccidently is over the field. Move your mouse over the field to verify this.\n\n'terminal 3' demonstrates 'inout-mode'." );
    fl_set_object_lsize( obj, FL_NORMAL_SIZE );

    fl_end_form( );

    fdui->f->fdui = fdui;

    return fdui;
}
