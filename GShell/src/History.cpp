/*
 * History.cpp
 *
 *  Created on: Dec 11, 2018
 *      Author: root
 */

#include "History.h"
#include <menu.h>

#define HWIN_LEFT 0
#define HWIN_TOP 0
#define HWIN_HEIGHT 20
#define HWIN_WIDTH 80
#define CTRL_D 4

using namespace std;


template <class HistoryItem>
History<HistoryItem>::History(int size) {
	this->size = size;
	first = last = current = NULL;
	count = 0;
}

template <class HistoryItem>
History<HistoryItem>::~History() {
	struct element *p, *q;
	p = first;
	while (p) {
		q=p;
		p=p->next;
		delete q;
	}

}

template <class HistoryItem>
void History<HistoryItem>::append(HistoryItem *item){
	if(count == size) return ;
	struct element *p = new struct element;
	p->item = item;
	p->next = NULL;
	p->prev = last;
	if (last) {
		last->next = p;
	} else {
		first=p;
	}
	last = p;
	current = p;
	count++;
}

template <class HistoryItem>
void History<HistoryItem>::goPastEnd()
{
	current=NULL;
}


template <class HistoryItem>
HistoryItem *History<HistoryItem>::up()
{
	if (!first) return NULL;
	if (current == NULL) {
		current = last;
	} else if (current != first) current = current->prev;
	return current ? current->item : NULL;
}

template <class HistoryItem>
HistoryItem *History<HistoryItem>::down()
{
	if (!first) return NULL;
	if (current != NULL && current != last) current = current->next;
	return current ? current->item : NULL;
}

template <class HistoryItem>
void History<HistoryItem>::initMenu() {

	/* Create items */
	my_items = new ITEM * [count];
	struct element *p=first;
	int i=0;
	while(p) {
		my_items[i++] = new_item(p->item->getTheString().c_str(),NULL);
		p = p->next;
	}

	/* Create menu */
	my_menu = new_menu(my_items);

	/* Create the window to be associated with the menu */
	my_menu_win = newwin(HWIN_HEIGHT, HWIN_WIDTH, HWIN_TOP, HWIN_LEFT);
	keypad(my_menu_win, TRUE);

	/* Set main window and sub window */
	set_menu_win(my_menu, my_menu_win);
	set_menu_sub(my_menu, NULL);
	set_menu_format(my_menu, 5, 1);

	/* Set menu mark to the string " * " */
	set_menu_mark(my_menu, "");

	/* Print a border around the main window and print a title */
	//box(my_menu_win, 0, 0);
	//mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
	//mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
	//mvwaddch(my_menu_win, 2, 39, ACS_RTEE);

	/* Post the menu */
	post_menu(my_menu);
	wrefresh(my_menu_win);

	attron(COLOR_PAIR(2));
	mvwprintw(my_menu_win, HWIN_HEIGHT - 1, HWIN_WIDTH - 15, "(CTRL_D to Exit)");
	attroff(COLOR_PAIR(2));
	refresh();
}

template <class HistoryItem>
char * History<HistoryItem>::showHistory()
{
	initMenu();

	int c;
	while((c = wgetch(my_menu_win)) != CTRL_D) {
		switch(c) {
		case KEY_DOWN:
			menu_driver(my_menu, REQ_DOWN_ITEM);
			break;
		case KEY_UP:
			menu_driver(my_menu, REQ_UP_ITEM);
			break;
		case KEY_NPAGE:
			menu_driver(my_menu, REQ_SCR_DPAGE);
			break;
		case KEY_PPAGE:
			menu_driver(my_menu, REQ_SCR_UPAGE);
			break;
		case'\n':
			char *ret = new char[WIN_WIDTH+1];
			winstr(my_menu_win,ret);
			endMenu();
			return ret;
		}
		wrefresh(my_menu_win);
	}
	endMenu();
	return NULL;
}

template <class HistoryItem>
void History<HistoryItem>::endMenu() {
	int i;
	unpost_menu(my_menu);
	free_menu(my_menu);
	for(i = 0; i < count; i++)
		free_item(my_items[i]);
	delete my_items;
	delwin(my_menu_win);
	refresh();
}
