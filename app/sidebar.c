#include "sidebar.h"

static GtkWidget *make_nav_row(const char *icon_name, const char *label_text, gboolean is_active)
{
    GtkWidget *row = gtk_list_box_row_new();

    GtkWidget *nav_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_add_css_class(nav_box, "nav-row-box");
    gtk_widget_set_margin_top(nav_box, 2);
    gtk_widget_set_margin_bottom(nav_box, 2);

    GtkWidget *icon = gtk_image_new_from_icon_name(icon_name);
    gtk_widget_add_css_class(icon, "nav-icon");
    gtk_image_set_pixel_size(GTK_IMAGE(icon), 16);

    GtkWidget *label = gtk_label_new(label_text);
    gtk_widget_add_css_class(label, "nav-label");
    gtk_widget_set_hexpand(label, TRUE);
    gtk_widget_set_halign(label, GTK_ALIGN_START);

    gtk_box_append(GTK_BOX(nav_box), icon);
    gtk_box_append(GTK_BOX(nav_box), label);

    if (is_active)
    {
        GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_box_append(GTK_BOX(vbox), nav_box);
        GtkWidget *badge = gtk_label_new("Active");
        gtk_widget_add_css_class(badge, "active-badge");
        gtk_widget_set_halign(badge, GTK_ALIGN_START);
        gtk_widget_set_margin_start(badge, 38);
        gtk_box_append(GTK_BOX(vbox), badge);
        gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row), vbox);
    }
    else
    {
        gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row), nav_box);
    }

    return row;
}

GtkWidget *create_sidebar(NavData *nd)
{
    GtkWidget *sidebar_scroll = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(
        GTK_SCROLLED_WINDOW(sidebar_scroll),
        GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(sidebar_scroll, 210, -1);
    gtk_widget_add_css_class(sidebar_scroll, "sidebar");

    GtkWidget *listbox = gtk_list_box_new();
    gtk_widget_add_css_class(listbox, "sidebar");
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(listbox), GTK_SELECTION_SINGLE);
    gtk_widget_set_margin_top(listbox, 8);
    gtk_widget_set_margin_bottom(listbox, 8);
    gtk_widget_set_margin_start(listbox, 8);
    gtk_widget_set_margin_end(listbox, 8);

    gtk_list_box_append(GTK_LIST_BOX(listbox),
                        make_nav_row("preferences-system-symbolic", "System Setup", FALSE));
    gtk_list_box_append(GTK_LIST_BOX(listbox),
                        make_nav_row("utilities-system-monitor-symbolic", "Performance Tweaks", FALSE));
    gtk_list_box_append(GTK_LIST_BOX(listbox),
                        make_nav_row("applications-graphics-symbolic", "Customization", FALSE));
    gtk_list_box_append(GTK_LIST_BOX(listbox),
                        make_nav_row("preferences-other-symbolic", "System Tools", FALSE));
    gtk_list_box_append(GTK_LIST_BOX(listbox),
                        make_nav_row("document-properties-symbolic", "Password Generator", FALSE));
    gtk_list_box_append(GTK_LIST_BOX(listbox),
                        make_nav_row("network-firewall-symbolic", "Manage Firewall", FALSE));
    // gtk_list_box_append(GTK_LIST_BOX(listbox),
    //                     make_nav_row("", "Manage Firewall", FALSE));
    gtk_list_box_append(GTK_LIST_BOX(listbox),
                        make_nav_row("help-about-symbolic", "About", FALSE));

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(sidebar_scroll), listbox);

    nd->listbox = GTK_LIST_BOX(listbox);

    g_signal_connect(listbox, "row-selected", G_CALLBACK(on_nav_row_selected), nd);

    return sidebar_scroll;
}

void on_nav_row_selected(GtkListBox *box, GtkListBoxRow *row, gpointer user_data)
{
    if (!row)
        return;
    NavData *nd = (NavData *)user_data;
    int idx = gtk_list_box_row_get_index(row);
    if (idx >= 0 && idx < 7)
        gtk_stack_set_visible_child_name(nd->stack, nd->pages[idx]);
}