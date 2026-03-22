#ifndef RUNNER_H
#define RUNNER_H

#include <gtk/gtk.h>

typedef struct
{
    const char *action_id;
    GtkWidget *spinner;
    GtkWidget *run_btn;
    GtkWindow *parent_window;
    void (*done_cb)(const char *action_id,
                    gboolean success,
                    int exit_code,
                    const char *out,
                    const char *errbuf,
                    gpointer user_data);
    gpointer done_user_data;
    GDestroyNotify done_user_data_destroy;
} ActionContext;

void on_run_clicked(GtkButton *btn, gpointer user_data);
void run_action_async(const char *action_id, GtkWindow *parent_window);
void run_action_async_with_callback(
    const char *action_id,
    GtkWindow *parent_window,
    void (*done_cb)(const char *action_id,
                    gboolean success,
                    int exit_code,
                    const char *out,
                    const char *errbuf,
                    gpointer user_data),
    gpointer user_data,
    GDestroyNotify user_data_destroy);

#endif