/*
 *
 *  This file is part of the XForms library package.
 *
 * XForms is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1, or
 * (at your option) any later version.
 *
 * XForms is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with XForms; see the file COPYING.  If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 *
 */

/**
 * \file pmenu.h
 */

#ifndef PMENU_H_
#define PMENU_H_

#define MAXITEMS  ( FL_MENU_MAXITEMS + 1 )	/* index 0 unused */

/* make sure that the first 5 elements are the same as FL_CHOICE.
 * fdesign assumes this
 */

typedef struct
{
    int             numitems;				/* number of items in menu */
    int             val;					/* last menu item selected */
    char          * items[ MAXITEMS ];	    /* individual menu items   */
    char          * shortcut[ MAXITEMS ];	/* shortcuts for items */
    unsigned char   mode[ MAXITEMS ];		/* menu item mode */
    int             align;				    /* onle here to mirror FL_CHOICE */
    int             extern_menu;			/* if external pop is used */
    short           showsymbol;			    /* whether symbol is to be shown */
    short           shown;				    /* if shown                     */
    char            mval[ MAXITEMS ];		/* entry value, position based */
    char            modechange[ MAXITEMS ];
    char            cur_val;				/* a counter for the value      */
	int             no_title;
} FL_MENU_SPEC;

#endif
