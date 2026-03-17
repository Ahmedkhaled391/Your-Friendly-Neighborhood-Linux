#ifndef PAGES_H
#define PAGES_H

#include <gtk/gtk.h>

GtkWidget *make_page(void);
void page_add(GtkWidget *page, GtkWidget *widget);
void page_add_title(GtkWidget *page, const char *text);

#endif // PAGES_H