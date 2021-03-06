/*
 *  This file is part of the XForms library package.
 *
 *  XForms is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation; either version 2.1, or
 *  (at your option) any later version.
 *
 *  XForms is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with XForms.  If not, see <http://www.gnu.org/licenses/>.
 */


/**
 * \file clock.c
 *
 *  This file is part of the XForms library package.
 *  Copyright (c) 1996-2002  T.C. Zhao and Mark Overmars
 *  All rights reserved.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "include/forms.h"
#include "flinternal.h"

#include <math.h>
#include <time.h>

#ifndef M_PI
#define M_PI   3.14159265359
#endif

typedef struct
{
    time_t ticks;
    long   offset;
    int    hours;
    int    minutes;
    int    seconds;
    int    nstep;
    int    am_pm;          /* 12 hr clock */
    int    hide_seconds;
    int    updating;
} SPEC;


static double hourhand[ 4 ][ 2 ] =
{
    { -0.6,  0.0 },
    {  0.0, -1.6 },
    {  0.6,  0.0 },
    {  0.0,  7.0 }
};

static double minhand[ 4 ][ 2 ] =
{
    { -0.6,  0.0 },
    {  0.0, -1.6 },
    {  0.6,  0.0 },
    {  0.0, 11.6 }
};

static double sechand[ 4 ][ 2 ] =
{
    { -0.3,  0.0 },
    {  0.0, -2.0 },
    {  0.3,  0.0 },
    {  0.0, 11.6 }
};


#define ROTxy( xx, yy, x, y, a )                                         \
    do                                                                   \
    {                                                                    \
        double s = sin( a );                                             \
        double c = cos( a );                                             \
        xx = FL_crnd( xc + ( ( x ) - xc ) * c + ( ( y ) - yc ) * s );    \
        yy = FL_crnd( yc - ( ( x ) - xc ) * s + ( ( y ) - yc ) * c );    \
    } while ( 0 )


/***************************************
 ***************************************/

static void
draw_hand( FL_Coord x,
           FL_Coord y,
           FL_Coord w,
           FL_Coord h,
           double   a[ ][ 2 ],
           double   ra,
           FL_COLOR fc,
           FL_COLOR bc )
{
    int i;
    double ccp[ 4 ][ 2 ];
    double xc = x + 0.5 * w,
           yc = y + 0.5 * h;
    FL_POINT xp[ 5 ];            /* Needs one extra point! */

    for ( i = 0; i < 4; i++ )
    {
        ccp[ i ][ 0 ] = xc + a[ i ][ 0 ] * w / 28.0;
        ccp[ i ][ 1 ] = yc + a[ i ][ 1 ] * h / 28.0;
        ROTxy( xp[ i ].x, xp[ i ].y, ccp[ i ][ 0 ], ccp[ i ][ 1 ], ra );
    }

    fl_polyf( xp, 4, fc );
    fl_polyl( xp, 4, bc );
}


/***************************************
 ***************************************/

static void
show_hands( FL_OBJECT * obj,
            FL_Coord x,
            FL_Coord y,
            FL_Coord w,
            FL_Coord h,
            FL_COLOR fcolor,
            FL_COLOR bcolor )
{
    SPEC * sp = obj->spec;
    double ra;
    double fact = - M_PI / 180.0;

    ra = fact * ( 180 + 30 * sp->hours + 0.5 * sp->minutes );
    draw_hand( x, y, w, h, hourhand, ra, fcolor, bcolor );

    ra = fact * ( 180 + 6 * sp->minutes + sp->seconds / 10 );
    draw_hand( x, y, w, h, minhand, ra, fcolor, bcolor );

    if ( ! sp->hide_seconds ) {
        ra = fact * ( 180 + 6 * sp->seconds );
        draw_hand( x, y, w, h, sechand, ra, fcolor, bcolor );
    }
}


/***************************************
 ***************************************/

static void
draw_clock( FL_OBJECT * obj )
{
    double xc = obj->x + 0.5 * obj->w,
           yc = obj->y + 0.5 * obj->h;
    FL_COORD w = obj->w - 4;
    FL_COORD h = obj->h - 4;
    int i;
    double ra;
    FL_POINT xp[ 5 ];             /* need one extra for closing of polygon! */
    double f1,
           f2 = f2 = 0.40 * h,
           f3 = 0.44 * h;

    /* Draw hour ticks */

    for ( ra = 0.0, i = 0; i < 12; i++, ra += M_PI / 6 )
    {
        f1 = ( ( i % 3 ) ? 0.01 : 0.02 ) * w;

        ROTxy( xp[ 0 ].x, xp[ 0 ].y, xc - f1, yc + f2, ra );
        ROTxy( xp[ 1 ].x, xp[ 1 ].y, xc + f1, yc + f2, ra );
        ROTxy( xp[ 2 ].x, xp[ 2 ].y, xc + f1, yc + f3, ra );
        ROTxy( xp[ 3 ].x, xp[ 3 ].y, xc - f1, yc + f3, ra );

        fl_polyf( xp, 4, FL_LEFT_BCOL );
    }

    show_hands( obj, obj->x + 2 + 0.02 * w, obj->y + 2 + 0.02 * h,
                w, h, FL_RIGHT_BCOL, FL_RIGHT_BCOL );
    show_hands( obj, obj->x, obj->y, w, h, obj->col2, FL_LEFT_BCOL );
}


/***************************************
 ***************************************/

static void
draw_digitalclock( FL_OBJECT * ob )
{
    char buf[ 12 ];
    SPEC *sp = ob->spec;

    if ( sp->am_pm )
    {
        if ( ! sp->hide_seconds )
            sprintf( buf, "%d:%02d:%02d %s",
                     sp->hours == 0 ? 12 : (sp->hours > 12 ?
                                            sp->hours - 12 : sp->hours),
                     sp-> minutes, sp->seconds, sp->hours >= 12 ? "pm" : "am" );
        else
            sprintf( buf, "%d:%02d %s",
                     sp->hours == 0 ? 12 : (sp->hours > 12 ?
                                            sp->hours - 12 : sp->hours),
                     sp->minutes, sp->hours >= 12 ? "pm" : "am" );
    }
    else
    {
        if ( ! sp->hide_seconds )
            sprintf( buf, "%d:%02d:%02d", sp->hours, sp->minutes, sp->seconds );
        else
            sprintf( buf, "%d:%02d", sp->hours, sp->minutes );
    }

    fl_draw_text( FL_ALIGN_CENTER, ob->x, ob->y, ob->w, ob->h, ob->col2,
                  ob->lstyle, ob->lsize, buf );
}


/***************************************
 ***************************************/

static int
handle_clock( FL_OBJECT * ob,
              int         event,
              FL_Coord    x   FL_UNUSED_ARG,
              FL_Coord    y   FL_UNUSED_ARG,
              int         k   FL_UNUSED_ARG,
              void *      ev  FL_UNUSED_ARG )
{
    time_t ticks;
    struct tm *timeofday;
    SPEC *sp = ob->spec;

    switch ( event )
    {
        case FL_ATTRIB :
            if ( ob->align & ~ FL_ALIGN_INSIDE )
                ob->align = fl_to_outside_lalign( ob->align );
            break;

        case FL_DRAW :
            fl_draw_box( ob->boxtype, ob->x, ob->y, ob->w, ob->h,
                         ob->col1, ob->bw );
            if ( ob->type == FL_DIGITAL_CLOCK )
                draw_digitalclock( ob );
            else
                draw_clock( ob );
            /* fall through */

        case FL_DRAWLABEL :
            if ( ! sp->updating )
                fl_draw_text_beside( ob->align & ~ FL_ALIGN_INSIDE,
                                     ob->x, ob->y, ob->w, ob->h,
                                     ob->lcol, ob->lstyle, ob->lsize,
                                     ob->label );
            sp->updating = 0;
            break;

        case FL_STEP:
            /* Clock has a resolution of about 1 sec. FL_STEP is sent about
               every 0.05 sec. Reduce number of tests to 10 per second. */

            if ( ++sp->nstep & 1 )
                break;

            sp->nstep = 0;
            ticks = time( 0 ) + sp->offset;

            if ( ticks != sp->ticks )
            {
                sp->ticks    = ticks;
                timeofday    = localtime( &ticks );
                sp->seconds  = timeofday->tm_sec;
                sp->hours    = timeofday->tm_hour;
                sp->minutes  = timeofday->tm_min;

                sp->updating = 1;
                fl_redraw_object( ob );
            }
            break;

        case FL_FREEMEM:
            fl_free( ob->spec );
            break;
    }

    return FL_RETURN_NONE;
}


/***************************************
 ***************************************/

FL_OBJECT *
fl_create_clock( int          type,
                 FL_Coord     x,
                 FL_Coord     y,
                 FL_Coord     w,
                 FL_Coord     h,
                 const char * s )
{
    FL_OBJECT *obj;
    SPEC *sp;

    obj = fl_make_object( FL_CLOCK, type, x, y, w, h, s, handle_clock );

    obj->boxtype   = FL_CLOCK_BOXTYPE;
    obj->col1      = FL_CLOCK_COL1;
    obj->col2      = FL_CLOCK_COL2;
    obj->lcol      = FL_CLOCK_LCOL;
    obj->align     = FL_CLOCK_ALIGN;
    obj->automatic = obj->active = 1;
    obj->spec      = fl_calloc( 1, sizeof *sp );

    return obj;
}


/***************************************
 ***************************************/

FL_OBJECT *
fl_add_clock( int          type,
              FL_Coord     x,
              FL_Coord     y,
              FL_Coord     w,
              FL_Coord     h,
              const char * s )

{
    FL_OBJECT *ob = fl_create_clock( type, x, y, w, h, s );

    fl_add_object( fl_current_form, ob );
    fl_set_object_dblbuffer( ob, 1 );
    return ob;
}


/***************************************
 ***************************************/

long
fl_set_clock_adjustment( FL_OBJECT * ob,
                         long        offset )
{
    SPEC *sp = ob->spec;
    long old = sp->offset;

    sp->offset = offset;

    return old;
}


/***************************************
 ***************************************/

void
fl_get_clock( FL_OBJECT * ob,
              int *       h,
              int *       m,
              int *       s )
{
    SPEC *sp = ob->spec;

    *h = sp->hours;
    *m = sp->minutes;
    *s = sp->seconds;
}


/***************************************
 ***************************************/

int
fl_set_clock_ampm( FL_OBJECT * ob,
                   int         am_pm )
{
    SPEC *sp = ob->spec;
    int old = sp->am_pm != am_pm;

    if ( sp->am_pm != !! am_pm )
        sp->am_pm = !! am_pm;

    return old;
}


/***************************************
 ***************************************/

int
fl_set_clock_hide_seconds( FL_OBJECT * obj,
                           int         hide )
{
    SPEC *sp = obj->spec;
    int old = sp->hide_seconds;

    if ( sp->hide_seconds != !! hide )
        sp->hide_seconds = !! hide;

    return old;
}


/*
 * Local variables:
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
