#ifndef CARD_H
#define CARD_H

#include <gtk/gtk.h>

GtkWidget *create_card(
    const char *title,
    const char *desc,
    const char *action_id,
    GtkWindow *parent);

#endif // CARD_H