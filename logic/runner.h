#ifndef RUNNER_H
#define RUNNER_H

#include <gtk/gtk.h>

typedef struct
{
    const char *action_id;
    GtkWidget *spinner;
    GtkWidget *run_btn;
    GtkWindow *parent_window;
} ActionContext;

void on_run_clicked(GtkButton *btn, gpointer user_data);

#endif 