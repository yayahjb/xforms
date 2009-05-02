/*
 *	This file is part of the XForms library package.
 *
 *  XForms is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation; either version 2.1, or
 *  (at your option) any later version.
 *
 *  XForms is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with XForms.  If not, see <http://www.gnu.org/licenses/>.
 */


/**
 * \file xdraw.c
 *.
 *	This file is part of the XForms library package.
 *	Copyright (c) 1996-2002	 T.C. Zhao and Mark Overmars
 *	All rights reserved.
 *.
 *
 *
 *	Basic low level drawing routines in Xlib.
 *
 *	BUGS: All form window share a common GC and Colormap.
 *
 */

#if defined F_ID || defined DEBUG
char *fl_id_drw = "$Id: xdraw.c,v 1.14 2009/05/02 20:11:14 jtt Exp $";
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "include/forms.h"
#include "flinternal.h"


static int fli_mono_dither( unsigned long );
static void fli_set_current_gc( GC );

static GC dithered_gc;


/*******************************************************************
 *
 * Rectangle routines
 *
 ****************************************************************{**/


/***************************************
 * General rectangle routine
 ***************************************/

void
fl_rectangle( int	   fill,
			  FL_Coord x,
			  FL_Coord y,
			  FL_Coord w,
			  FL_Coord h,
			  FL_COLOR col )
{
	int bw = fli_dithered( fl_vmode ) && fli_mono_dither( col );
	GC gc = flx->gc;
	int ( * draw_as )( Display *,
					   Drawable,
					   GC,
					   int,
					   int,
					   unsigned int,
					   unsigned int );

	if ( flx->win == None || w <= 0 || h <= 0 )
		return;

	fli_canonicalize_rect( &x, &y, &w, &h );

	draw_as = fill ? XFillRectangle : XDrawRectangle;

	if ( bw && fill )
	{
		fli_set_current_gc( fli_whitegc );
		draw_as( flx->display, flx->win, flx->gc, x, y, w, h );
		fli_set_current_gc( dithered_gc );
	}


	fl_color( bw ? FL_BLACK : col );
	draw_as( flx->display, flx->win, flx->gc, x, y, w, h );

	if ( bw )
		fli_set_current_gc( gc );
}


/****** End of rectangle routines ***********************}***/


/****************************************************************
 *
 * Polygon and polylines
 *
 ***********************************************************{****/


/***************************************
 * xp must have n + 1 elements !!!
 ***************************************/

void
fl_polygon( int		   fill,
			FL_POINT * xp,
			int		   n,
			FL_COLOR   col )
{
	int bw = fli_dithered( fl_vmode ) && fli_mono_dither( col );
	GC gc = flx->gc;

	if ( flx->win == None || n <= 0 )
		return;

	if ( bw )
	{
		flx->gc = dithered_gc;
		fl_color( FL_WHITE );
		if ( fill )
			XFillPolygon( flx->display, flx->win, flx->gc, xp, n,
						  Nonconvex, CoordModeOrigin );
		else
		{
			xp[ n ].x = xp[ 0 ].x;
			xp[ n ].y = xp[ 0 ].y;
			n++;
			XDrawLines( flx->display, flx->win, flx->gc, xp, n,
						CoordModeOrigin );
		}
	}

	fl_color( bw ? FL_BLACK : col );

	if ( fill )
		XFillPolygon( flx->display, flx->win, flx->gc, xp, n,
					  Nonconvex, CoordModeOrigin );
	else
	{
		xp[ n ].x = xp[ 0 ].x;
		xp[ n ].y = xp[ 0 ].y;
		n++;
		XDrawLines( flx->display, flx->win, flx->gc, xp, n, CoordModeOrigin );
	}

	if ( bw )
		flx->gc = gc;
}


/****************** End of polygons *******************}********/

/****************************************************************
 *
 * Ellipse
 *
 **********************************************************{******/

void
fl_oval( int	  fill,
		 FL_Coord x,
		 FL_Coord y,
		 FL_Coord w,
		 FL_Coord h,
		 FL_COLOR col )
{
	int bw = fli_dithered( fl_vmode ) && fli_mono_dither( col );
	GC gc = flx->gc;
	int ( * draw_as )( Display *,
					   Drawable,
					   GC,
					   int,
					   int,
					   unsigned int,
					   unsigned int,
					   int,
					   int );

	if ( flx->win == None || w <= 0 || h <= 0 )
		return;

	draw_as = fill ? XFillArc : XDrawArc;

	if ( bw )
	{
		fli_set_current_gc( fli_whitegc );
		draw_as( flx->display, flx->win, flx->gc, x, y, w, h, 0, 360 * 64 );
		fli_set_current_gc( dithered_gc );
	}

	fl_color( bw ? FL_BLACK : col );

	if ( w >= 0 && h >= 0 )
		draw_as( flx->display, flx->win, flx->gc, x, y, w, h, 0, 360 * 64 );

	if ( bw )
		fli_set_current_gc( gc );
}


/***************************************
 ***************************************/

void
fl_ovalbound( FL_Coord x,
			  FL_Coord y,
			  FL_Coord w,
			  FL_Coord h,
			  FL_COLOR col )
{
	if ( flx->win == None || w <= 0 || h <= 0 )
		return;

	fl_color( col );
	XFillArc( flx->display, flx->win, flx->gc, x, y, w, h, 0, 360 * 64 );
	fl_color( FL_BLACK );
	XDrawArc( flx->display, flx->win, flx->gc, x, y, w - 1, h - 1, 0,
			  360 * 64 );
}


/******* End of ellipse routines ****************}***********/

/****************************************************************
 *
 * Arcs
 ****************************************************************/

/***************************************
 ***************************************/

void
fl_ovalarc( int		 fill,
			FL_Coord x,
			FL_Coord y,
			FL_Coord w,
			FL_Coord h,
			int		 t0,
			int		 dt,
			FL_COLOR col )
{
	int mono = fli_dithered( fl_vmode ) && fli_mono_dither( col );
	int ( * draw_as )( Display *,
					   Drawable,
					   GC,
					   int,
					   int,
					   unsigned int,
					   unsigned int,
					   int,
					   int );

	if ( flx->win == None || w <= 0 || h <= 0 )
		return;

	draw_as = fill ? XFillArc : XDrawArc;

	if ( mono )
	{
		fli_set_current_gc( fli_whitegc );
		draw_as( flx->display, flx->win, flx->gc, x, y, w, h,
				 t0 * 6.4, dt * 6.4 );
		fli_set_current_gc( dithered_gc );
	}

	fl_color( mono ? FL_BLACK : col );

	if ( w >= 0 && h >= 0 )
		draw_as( flx->display, flx->win, flx->gc, x, y, w, h,
				 t0 * 6.4, dt * 6.4 );

	if ( mono )
		fli_set_current_gc( fl_state[ fl_vmode ].gc[ 0 ] );
}


/***************************************
 ***************************************/

void
fl_pieslice( int	  fill,
			 FL_Coord x,
			 FL_Coord y,
			 FL_Coord w,
			 FL_Coord h,
			 int	  a1,
			 int	  a2,
			 FL_COLOR col )
{
	int delta = a2 - a1,
		bw = fli_dithered( fl_vmode ) && fli_mono_dither( col );
	GC gc = flx->gc;
	int ( * draw_as )( Display *,
					   Drawable,
					   GC,
					   int,
					   int,
					   unsigned int,
					   unsigned int,
					   int,
					   int );

	if ( flx->win == None || w <= 0 || h <= 0)
		return;

	draw_as = fill ? XFillArc : XDrawArc;

	if ( bw )
	{
		fli_set_current_gc( fli_whitegc );
		draw_as( flx->display, flx->win, flx->gc, x, y, w, h,
				 a1 * 6.4, delta * 6.4 );
		fli_set_current_gc( dithered_gc );
	}

	fl_color( bw ? FL_BLACK : col );

	if ( w >= 0 && h >= 0 )
		draw_as( flx->display, flx->win, flx->gc, x, y, w, h,
				 a1 * 6.4, delta * 6.4 );
	if ( bw )
		fli_set_current_gc( gc );
}


/*********************************************************************
 * Line segments
 *****************************************************************{***/


/***************************************
 ***************************************/

void
fl_lines( FL_POINT * xp,
		  int		 n,
		  FL_COLOR	 col )
{
	if ( flx->win == None  || n <= 0 )
		return;

	fl_color( col );

	/* we may need to break up the request into smaller pieces */

	if ( fli_context->ext_request_size >= n )
		XDrawLines( flx->display, flx->win, flx->gc, xp, n, CoordModeOrigin );
	else
	{
		int req = fli_context->ext_request_size;
		int i,
			nchunks = ( n + ( n / req ) ) / req,
			left;
		FL_POINT *p = xp;

		for ( i = 0; i < nchunks; i++, p += req - 1 )
			XDrawLines( flx->display, flx->win, flx->gc, p, req,
						CoordModeOrigin );

		left = xp + n - p;

		if ( left )
		{
			if ( left == 1 )
			{
				p--;
				left++;
			}

			XDrawLines( flx->display, flx->win, flx->gc, p, left,
						CoordModeOrigin );
		}
	}
}


/***************************************
 * simple line (x1,y1) (x2,y2)
 ***************************************/

void
fl_line( FL_Coord xi,
		 FL_Coord yi,
		 FL_Coord xf,
		 FL_Coord yf,
		 FL_COLOR c )
{
	if ( flx->win == None )
		return;

	fl_color( c );
	XDrawLine( flx->display, flx->win, flx->gc, xi, yi, xf, yf );
}


/**************** End of line segments *******************}*********/


/* points */

/***************************************
 ***************************************/

void
fl_point( FL_Coord x,
		  FL_Coord y,
		  FL_COLOR c )
{
	if ( flx->win == None )
		return;

	fl_color( c );
	XDrawPoint( flx->display, flx->win, flx->gc, x, y );
}


/***************************************
 ***************************************/

void
fl_points( FL_POINT * p,
		   int		  np,
		   FL_COLOR	  c )
{
	if ( flx->win == None || np <= 0 )
		return;

	fl_color( c );
	XDrawPoints( flx->display, flx->win, flx->gc, p, np, CoordModeOrigin );
}


/********************************************************************
 * Basic drawing attributes
 ****************************************************************{*/

static int lw     = 0,
		   ls     = LineSolid,
		   drmode = GXcopy;


/***************************************
 ***************************************/

void
fl_linewidth( int n )
{
	XGCValues gcvalue;
	unsigned long gcmask;

	if ( lw == n )
		return;

	gcmask = GCLineWidth;
	gcvalue.line_width = lw = n;
	XChangeGC( flx->display, flx->gc, gcmask, &gcvalue );
}


/***************************************
 ***************************************/

int
fl_get_linewidth( void )
{
	return lw;
}


static void fli_xdashedlinestyle( Display *,
								  GC,
								  const char *,
								  int );

/***************************************
 ***************************************/

void
fli_xlinestyle( Display * d,
				GC		 gc,
				int		 n )
{
	static char dots[ ]    = { 2, 4 };
	static char dotdash[ ] = { 7, 3, 2, 3 };
	static char ldash[ ]   = { 10, 4 };
	XGCValues gcvalue;
	unsigned long gcmask;

	if ( ls == n )
		return;

	ls = n;

	gcmask = GCLineStyle;
	if ( n == FL_DOT )
		fli_xdashedlinestyle( d, gc, dots, 2 );
	else if ( n == FL_DOTDASH )
		fli_xdashedlinestyle( d, gc, dotdash, 4 );
	else if ( n == FL_LONGDASH )
		fli_xdashedlinestyle( d, gc, ldash, 2 );
	if ( n > LineDoubleDash )
		n = LineOnOffDash;

	gcvalue.line_style = n;
	XChangeGC( d, gc, gcmask, &gcvalue );
}


/***************************************
 ***************************************/

void
fl_linestyle( int n )
{
	fli_xlinestyle( flx->display, flx->gc, n );
}


/***************************************
 ***************************************/

int
fl_get_linestyle( void )
{
	return ls;
}


/***************************************
 ***************************************/

int
fl_get_drawmode( void )
{
	return drmode;
}


/***************************************
 ***************************************/

void
fl_drawmode( int request )
{
	if ( drmode != request )
		XSetFunction( flx->display, flx->gc, drmode = request );
}


/***************************************
 ***************************************/

static void
fli_xdashedlinestyle( Display	 * d,
					  GC		   gc,
					  const char * dash,
					  int		   ndash )
{
	static char default_dash[ ] = { 4, 4 };

	if ( dash == NULL )
	{
		dash = default_dash;
		ndash = 2;
	}

	XSetDashes( d, gc, 0, ( char * ) dash, ndash );
}


/***************************************
 ***************************************/

void
fl_dashedlinestyle( const char * dash,
					int			 ndash )
{
	static char default_dash[ ] = { 4, 4 };

	if ( dash == NULL )
	{
		dash = default_dash;
		ndash = 2;
	}

	XSetDashes( flx->display, flx->gc, 0, ( char * ) dash, ndash );
}


/************************************************************************
 * Clipping stuff
 ***********************************************************************/
/*
 *	Remember global clipping so unset_clipping will restore it. Most
 *	useful as part of event dispatching
 */

XRectangle fli_perm_xcr;
int fli_perm_clip;
static FL_RECT cur_clip;	/* not includng perm clip, probably should */


/***************************************
 ***************************************/

void
fli_set_perm_clipping( FL_Coord x,
					   FL_Coord y,
					   FL_Coord w,
					   FL_Coord h )
{
	fli_perm_clip       = 1;
	fli_perm_xcr.x      = x;
	fli_perm_xcr.y      = y;
	fli_perm_xcr.width  = w;
	fli_perm_xcr.height = h;
}


/***************************************
 ***************************************/

void
fli_unset_perm_clipping( void )
{
	fli_perm_clip = 0;
}


/***************************************
 ***************************************/

void
fl_set_clipping( FL_Coord x,
				 FL_Coord y,
				 FL_Coord w,
				 FL_Coord h )
{
	cur_clip.x      = x;
	cur_clip.y      = y;
	cur_clip.width  = w;
	cur_clip.height = h;

	if ( w > 0 && h > 0 )
	   XSetClipRectangles( flx->display, flx->gc, 0, 0, &cur_clip, 1,
						   Unsorted );
	else
		XSetClipMask( flx->display, flx->gc, None );
}


/***************************************
 ***************************************/

void
fl_set_text_clipping( FL_Coord x,
					  FL_Coord y,
					  FL_Coord w,
					  FL_Coord h )
{
	fl_set_gc_clipping( flx->textgc, x, y, w, h );
}


/***************************************
 ***************************************/

void
fl_unset_text_clipping( void )
{
	fl_unset_gc_clipping( flx->textgc );
}


/***************************************
 ***************************************/

void
fli_get_clipping( FL_Coord * x,
				  FL_Coord * y,
				  FL_Coord * w,
				  FL_Coord * h )
{
	*x = cur_clip.x;
	*y = cur_clip.y;
	*w = cur_clip.width;
	*h = cur_clip.height;
}


/***************************************
 ***************************************/

void
fli_set_additional_clipping( FL_Coord x,
							 FL_Coord y,
							 FL_Coord w,
							 FL_Coord h )
{
	FL_RECT rect[ 2 ],
			*r;

	rect[ 0 ]        = cur_clip;
	rect[ 1 ].x      = x;
	rect[ 1 ].y      = y;
	rect[ 1 ].width  = w;
	rect[ 1 ].height = h;

	r = fli_union_rect( rect, rect + 1 );

	if ( r != NULL )
	{
		XSetClipRectangles( flx->display, flx->gc, 0, 0, r, 1, Unsorted );
		fl_free( r );
	}
}


/***************************************
 ***************************************/

void
fl_set_gc_clipping( GC		 gc,
					FL_Coord x,
					FL_Coord y,
					FL_Coord w,
					FL_Coord h )
{
	XRectangle xrect;

	xrect.x      = x;
	xrect.y      = y;
	xrect.width  = w;
	xrect.height = h;
	XSetClipRectangles( flx->display, gc, 0, 0, &xrect, 1, Unsorted );
}


/***************************************
 ***************************************/

void
fl_set_clippings( FL_RECT * xrect,
				  int		n )
{
	XSetClipRectangles( flx->display, flx->gc, 0, 0, xrect, n, Unsorted );
}


/***************************************
 ***************************************/

void
fl_unset_clipping( void )
{
	if ( ! fli_perm_clip )
	{
		XSetClipMask( flx->display, flx->gc, None );
		cur_clip.x = cur_clip.y = cur_clip.width = cur_clip.height = 0;
	}
	else
	{
		XSetClipRectangles( flx->display, flx->gc, 0, 0, &fli_perm_xcr, 1,
							Unsorted );
		cur_clip = fli_perm_xcr;
	}
}


/***************************************
 ***************************************/

void
fl_unset_gc_clipping( GC gc )
{
	if ( ! fli_perm_clip )
		XSetClipMask( flx->display, gc, None );
	else
		XSetClipRectangles( flx->display, gc, 0, 0, &fli_perm_xcr, 1,
							Unsorted );
}


/***************************************
 ***************************************/

static void
fli_set_current_gc( GC gc )
{
	if ( flx->gc != gc )
	{
		flx->gc = gc;
		flx->color = FL_NoColor;
	}
}


/***************************************
 * manually dither non-gray scale colors by changing default GC. Grayscales
 * are typically used in buttons, boxes etc, better not to dither them
 ***************************************/

static int
fli_mono_dither( unsigned long col )
{
	int bwtrick = 0;

	switch ( col )
	{
		case FL_RED:
		case FL_MAGENTA:
		case FL_SLATEBLUE:
		case FL_PALEGREEN:
		case FL_DARKGOLD:
		case FL_INACTIVE_COL:
			dithered_gc = fli_bwgc[ 1 ];
			bwtrick     = 1;
			break;

		case FL_YELLOW:
		case FL_CYAN:
		case FL_INDIANRED:
		case FL_GREEN:
			dithered_gc = fli_bwgc[ 2 ];
			bwtrick     = 1;
			break;

		case FL_BLUE:
			dithered_gc = fli_bwgc[ 0 ];
			bwtrick     = 1;
			break;

		default:
			if ( col >= FL_FREE_COL1 )
			{
				int r,
					g,
					b;

				fl_get_icm_color( col, &r, &g, &b );
				if ( ( bwtrick = ( r > 70 && r <= 210 ) ) )
					dithered_gc = fli_bwgc[ r / 70 - 1 ];
			}
			break;
	}

	return bwtrick;
}
