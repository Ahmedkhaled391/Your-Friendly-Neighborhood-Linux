#include "page_firewall.h"
#include "pages.h"
#include "../../logic/runner.h"

static gboolean on_firewall_switch_state_set(GtkSwitch *sw, gboolean state, gpointer user_data)
{
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
    gtk_widget_set_halign(vbox, GTK_ALIGN_CENTER);

    GtkWidget *switcher = gtk_switch_new();
    gtk_widget_set_tooltip_text(switcher, "Enable or disable firewall");
    gtk_switch_set_active(GTK_SWITCH(switcher), FALSE);
    gtk_widget_set_valign(switcher, GTK_ALIGN_CENTER);
    gtk_widget_add_css_class(switcher, "firewall-switch");

    g_signal_connect(switcher, "state-set", G_CALLBACK(on_firewall_switch_state_set), parent);

    gtk_box_append(GTK_BOX(vbox), switcher);

    return page;
}
