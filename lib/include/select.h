/*
 *
 *	This file is part of the XForms library package.
 *
 * XForms is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1, or
 * (at your option) any later version.
 *
 * XForms is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with XForms.  If not, see <http://www.gnu.org/licenses/>.
 */

/********************** crop here for forms.h **********************/

/**
 * \file select.h
 *
 */

#ifndef FL_SELECT_H
#define FL_SELECT_H


/* Select object types */

enum {
	FL_NORMAL_SELECT,
	FL_MENU_SELECT,
	FL_DROPLIST_SELECT
};

/* Color types */

enum {
	FL_SELECT_POPUP_BACKGROUND_COLOR       = FL_POPUP_BACKGROUND_COLOR,
	FL_SELECT_POPUP_HIGHLIGHT_COLOR        = FL_POPUP_HIGHLIGHT_COLOR,
	FL_SELECT_POPUP_TITLE_COLOR            = FL_POPUP_TITLE_COLOR,
	FL_SELECT_POPUP_TEXT_COLOR             = FL_POPUP_TEXT_COLOR,
	FL_SELECT_POPUP_HIGHLIGHT_TEXT_COLOR   = FL_POPUP_HIGHLIGHT_TEXT_COLOR,
	FL_SELECT_POPUP_DISABLED_TEXT_COLOR    = FL_POPUP_DISABLED_TEXT_COLOR,
	FL_SELECT_NORMAL_COLOR,
	FL_SELECT_HIGHLIGHT_COLOR,
	FL_SELECT_LABEL_COLOR,
	FL_SELECT_TEXT_COLOR
};

/* Text types */

enum {
	FL_SELECT_TEXT_FONT,
	FL_SELECT_POPUP_TEXT_FONT,
    FL_SELECT_ITEM_TEXT_FONT,
	FL_SELECT_LABEL_FONT
};

/* Defaults */

#define FL_SELECT_COL1		    FL_COL1
#define FL_SELECT_COL2		    FL_MCOL
#define FL_SELECT_LCOL		    FL_LCOL
#define FL_SELECT_ALIGN		    FL_ALIGN_LEFT


FL_EXPORT FL_OBJECT *fl_create_select(
		int,
		FL_Coord,
		FL_Coord,
		FL_Coord,
		FL_Coord,
		const char *
		);

FL_EXPORT FL_OBJECT *fl_add_select(
		int,
		FL_Coord,
		FL_Coord,
		FL_Coord,
		FL_Coord,
		const char *
		);

FL_EXPORT int fl_clear_select(
		FL_OBJECT *
		);

FL_EXPORT FL_POPUP_ENTRY *fl_add_select_items(
		FL_OBJECT  *,
		const char *,
		...
		);

FL_EXPORT FL_POPUP_ENTRY *fl_insert_select_items(
		FL_OBJECT *,
		FL_POPUP_ENTRY *,
		const char     *,
		...
		);

FL_EXPORT FL_POPUP_ENTRY *fl_replace_select_item(
		FL_OBJECT *,
		FL_POPUP_ENTRY *,
	    const char *,
		...
		);

FL_EXPORT int fl_delete_select_item(
		FL_OBJECT *,
		FL_POPUP_ENTRY *
		);

FL_EXPORT int fl_set_select_popup(
		FL_OBJECT *,
		FL_POPUP  *
		);

FL_EXPORT FL_POPUP_RETURN *fl_get_select_item(
		FL_OBJECT *
		);

FL_EXPORT FL_POPUP_RETURN *fl_set_select_item(
		FL_OBJECT *,
		FL_POPUP_ENTRY *
		);

FL_EXPORT FL_POPUP_ENTRY *fl_get_select_item_by_value(
		FL_OBJECT *,
		long int
		);

FL_EXPORT FL_POPUP_ENTRY *fl_get_select_item_by_label(
		FL_OBJECT *,
		const char *
		);

FL_EXPORT FL_POPUP_ENTRY *fl_get_select_item_by_text(
		FL_OBJECT *,
		const char *
		);

FL_EXPORT FL_OBJECT *fl_set_select_popup_title(
		FL_OBJECT  *,
		const char *
		);

FL_EXPORT int fl_get_select_font(
		FL_OBJECT *,
		int,
		int *,
		int *
		);

FL_EXPORT int fl_set_select_font(
		FL_OBJECT *,
		int,
		int,
		int 
		);

FL_EXPORT int fl_set_select_text_align(
		FL_OBJECT *,
		int
		);

FL_EXPORT int fl_set_select_popup_bw(
		FL_OBJECT *,
		int
		);

FL_EXPORT int fl_set_select_policy(
		FL_OBJECT *,
		int
		);

#endif /* ! defined FL_SELECT_H */
