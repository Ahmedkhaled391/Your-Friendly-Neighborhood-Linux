#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <gtk/gtk.h>

typedef struct
{
    GtkStack *stack;
    GtkListBox *listbox;
    const char *pages[5];
} NavData;

GtkWidget *create_sidebar(NavData *nd);
void on_nav_row_selected(GtkListBox *box, GtkListBoxRow *row, gpointer user_data);

#endif // SIDEBAR_H