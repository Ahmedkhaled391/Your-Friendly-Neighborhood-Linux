#include "page_firewall.h"
#include "pages.h"
#include "../../logic/runner.h"

static gboolean firewall_output_is_active(const char *output)
{
    if (!output)
        return FALSE;

    return g_strstr_len(output, -1, "Status: active") != NULL;
}

static void on_firewall_status_loaded(GObject *src, GAsyncResult *res, gpointer user_data)
{
    GSubprocess *proc = G_SUBPROCESS(src);
    GtkSwitch *switcher = GTK_SWITCH(user_data);
    GError *err = NULL;
    char *out = NULL;
    char *errbuf = NULL;

    gboolean ok = g_subprocess_communicate_utf8_finish(proc, res, &out, &errbuf, &err);

    if (ok && !err)
    {
        g_object_set_data(G_OBJECT(switcher), "syncing-firewall-state", GINT_TO_POINTER(1));
        gtk_switch_set_active(switcher, firewall_output_is_active(out));
        g_object_set_data(G_OBJECT(switcher), "syncing-firewall-state", NULL);
    }

    gtk_widget_set_sensitive(GTK_WIDGET(switcher), TRUE);

    if (err)
        g_error_free(err);

    g_free(out);
    g_free(errbuf);
    g_object_unref(proc);
    g_object_unref(switcher);
}

static void load_firewall_status(GtkSwitch *switcher)
{
    GError *err = NULL;
    const char *argv[] = {
        "/bin/sh",
        "-c",
        "grep -qi '^ENABLED=yes' /etc/ufw/ufw.conf 2>/dev/null && echo 'Status: active' || echo 'Status: inactive'",
        NULL};

    gtk_widget_set_sensitive(GTK_WIDGET(switcher), FALSE);

    GSubprocess *proc = g_subprocess_newv(
        argv,
        G_SUBPROCESS_FLAGS_STDOUT_PIPE | G_SUBPROCESS_FLAGS_STDERR_PIPE,
        &err);

    if (!proc)
    {
        gtk_widget_set_sensitive(GTK_WIDGET(switcher), TRUE);
        if (err)
            g_error_free(err);
        return;
    }

    g_subprocess_communicate_utf8_async(
        proc,
        NULL,
        NULL,
        on_firewall_status_loaded,
        g_object_ref(switcher));
}

static gboolean on_firewall_switch_state_set(GtkSwitch *sw, gboolean state, gpointer user_data)
{
    if (g_object_get_data(G_OBJECT(sw), "syncing-firewall-state") != NULL)
        return FALSE;

    (void)sw;
    GtkWindow *parent = GTK_WINDOW(user_data);
    if (state)
    {
        run_action_async("enable_firewall", parent);
    }
    else
    {
        run_action_async("disable_firewall", parent);
    }
    return FALSE;
}

GtkWidget *create_page_firewall(GtkWindow *parent)
{
    GtkWidget *page = make_page();
    page_add_title(page, "Firewall");

    GtkWidget *vbox = GTK_WIDGET(g_object_get_data(G_OBJECT(page), "vbox"));
    gtk_widget_set_valign(vbox, GTK_ALIGN_START);
    gtk_widget_set_halign(vbox, GTK_ALIGN_FILL);

    GtkWidget *switcher = gtk_switch_new();
    gtk_widget_set_tooltip_text(switcher, "Enable or disable firewall");
    gtk_switch_set_active(GTK_SWITCH(switcher), FALSE);
    gtk_widget_set_valign(switcher, GTK_ALIGN_CENTER);
    gtk_widget_add_css_class(switcher, "firewall-switch");

    g_signal_connect(switcher, "state-set", G_CALLBACK(on_firewall_switch_state_set), parent);
    load_firewall_status(GTK_SWITCH(switcher));

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

    GtkWidget *title = gtk_label_new("Firewall Status");
    gtk_widget_set_halign(title, GTK_ALIGN_START);
    gtk_widget_add_css_class(title, "card-title");

    GtkWidget *desc = gtk_label_new("Turn UFW on or off. Current state is loaded from system configuration.");
    gtk_widget_set_halign(desc, GTK_ALIGN_START);
    gtk_label_set_wrap(GTK_LABEL(desc), TRUE);
    gtk_label_set_max_width_chars(GTK_LABEL(desc), 55);
    gtk_widget_add_css_class(desc, "card-desc");

    gtk_box_append(GTK_BOX(text_box), title);
    gtk_box_append(GTK_BOX(text_box), desc);

    gtk_box_append(GTK_BOX(hbox), text_box);
    gtk_box_append(GTK_BOX(hbox), switcher);

    gtk_box_append(GTK_BOX(vbox), frame);

    return page;
}
