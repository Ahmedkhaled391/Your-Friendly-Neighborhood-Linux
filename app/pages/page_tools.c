#include "page_tools.h"
#include "pages.h"
#include "../widgets/card.h"
#include "../../logic/runner.h"

static void on_backup_status_loaded(GObject *src, GAsyncResult *res, gpointer user_data)
{
    GSubprocess *proc = G_SUBPROCESS(src);
    GtkSwitch *sw = GTK_SWITCH(user_data);
    GError *err = NULL;
    char *out = NULL;
    char *errbuf = NULL;

    gboolean ok = g_subprocess_communicate_utf8_finish(proc, res, &out, &errbuf, &err);

    if (ok && !err && out)
    {
        gboolean enabled = g_strstr_len(out, -1, "enabled") != NULL;
        g_object_set_data(G_OBJECT(sw), "syncing-backup-state", GINT_TO_POINTER(1));
        gtk_switch_set_active(sw, enabled);
        g_object_set_data(G_OBJECT(sw), "syncing-backup-state", NULL);
    }

    gtk_widget_set_sensitive(GTK_WIDGET(sw), TRUE);

    if (err)
        g_error_free(err);
    g_free(out);
    g_free(errbuf);
    g_object_unref(proc);
    g_object_unref(sw);
}

static void load_backup_status(GtkSwitch *sw)
{
    const char *argv[] = {
        "/bin/sh", "-c",
        "crontab -l 2>/dev/null | grep -qF 'YFNL_SIMPLE_BACKUP=1' && echo 'enabled' || echo 'disabled'",
        NULL};

    gtk_widget_set_sensitive(GTK_WIDGET(sw), FALSE);

    GError *err = NULL;
    GSubprocess *proc = g_subprocess_newv(
        argv,
        G_SUBPROCESS_FLAGS_STDOUT_PIPE | G_SUBPROCESS_FLAGS_STDERR_PIPE,
        &err);

    if (!proc)
    {
        gtk_widget_set_sensitive(GTK_WIDGET(sw), TRUE);
        if (err)
            g_error_free(err);
        return;
    }

    g_subprocess_communicate_utf8_async(
        proc, NULL, NULL,
        on_backup_status_loaded,
        g_object_ref(sw));
}

static gboolean on_backup_switch_state_set(GtkSwitch *sw, gboolean state, gpointer user_data)
{
    if (g_object_get_data(G_OBJECT(sw), "syncing-backup-state") != NULL)
        return FALSE;

    GtkWindow *parent = GTK_WINDOW(user_data);
    run_action_async(state ? "backup_install_auto" : "backup_remove_auto", parent);
    return FALSE;
}

static GtkWidget *create_backup_toggle_card(GtkWindow *parent)
{
    GtkWidget *frame = gtk_frame_new(NULL);
    gtk_widget_add_css_class(frame, "action-card");
    gtk_widget_set_margin_start(frame, 16);
    gtk_widget_set_margin_end(frame, 16);
    gtk_widget_set_margin_top(frame, 6);
    gtk_widget_set_margin_bottom(frame, 6);

    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 16);
    gtk_widget_set_margin_start(hbox, 18);
    gtk_widget_set_margin_end(hbox, 18);
    gtk_widget_set_margin_top(hbox, 16);
    gtk_widget_set_margin_bottom(hbox, 16);
    gtk_frame_set_child(GTK_FRAME(frame), hbox);

    GtkWidget *text_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_hexpand(text_box, TRUE);
    gtk_widget_set_valign(text_box, GTK_ALIGN_CENTER);

    GtkWidget *title = gtk_label_new("Monthly Auto Backup");
    gtk_widget_set_halign(title, GTK_ALIGN_START);
    gtk_widget_add_css_class(title, "card-title");

    GtkWidget *desc = gtk_label_new("Automatically backs up Documents, Desktop, Pictures and Downloads every month.");
    gtk_widget_set_halign(desc, GTK_ALIGN_START);
    gtk_label_set_wrap(GTK_LABEL(desc), TRUE);
    gtk_label_set_max_width_chars(GTK_LABEL(desc), 55);
    gtk_widget_add_css_class(desc, "card-desc");

    gtk_box_append(GTK_BOX(text_box), title);
    gtk_box_append(GTK_BOX(text_box), desc);

    GtkWidget *sw = gtk_switch_new();
    gtk_widget_add_css_class(sw, "firewall-switch");
    gtk_widget_set_valign(sw, GTK_ALIGN_CENTER);

    g_signal_connect(sw, "state-set", G_CALLBACK(on_backup_switch_state_set), parent);
    load_backup_status(GTK_SWITCH(sw));

    gtk_box_append(GTK_BOX(hbox), text_box);
    gtk_box_append(GTK_BOX(hbox), sw);

    return frame;
}

GtkWidget *create_page_tools(GtkWindow *parent)
{
    GtkWidget *page = make_page();
    page_add_title(page, "System Tools");
    page_add(page, create_card(
                       "Check System Information",
                       "Displays OS version, kernel info, CPU details, memory and disk usage.",
                       "check_sysinfo", parent));
    page_add(page, create_card(
                       "Run Backup Now",
                       "Backs up your Documents, Desktop, Pictures and Downloads into a compressed archive.",
                       "backup_run", parent));
    page_add(page, create_backup_toggle_card(parent));
    return page;
}