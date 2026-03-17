#include "pages.h"

// ─────────────────────────────────────────────────────────────────────────────
// Scrollable page helper
// ─────────────────────────────────────────────────────────────────────────────

GtkWidget *make_page(void)
{
    GtkWidget *scroll = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(
        GTK_SCROLLED_WINDOW(scroll),
        GTK_POLICY_NEVER,
        GTK_POLICY_AUTOMATIC);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_margin_top(vbox, 16);
    gtk_widget_set_margin_bottom(vbox, 16);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), vbox);
    g_object_set_data(G_OBJECT(scroll), "vbox", vbox);

    return scroll;
}

void page_add(GtkWidget *page, GtkWidget *widget)
{
    GtkWidget *vbox = GTK_WIDGET(g_object_get_data(G_OBJECT(page), "vbox"));
    gtk_box_append(GTK_BOX(vbox), widget);
}

// Adds a bold page heading inside the scrollable content
void page_add_title(GtkWidget *page, const char *text)
{
    GtkWidget *lbl = gtk_label_new(text);
    gtk_widget_add_css_class(lbl, "page-title");
    gtk_widget_set_halign(lbl, GTK_ALIGN_START);
    gtk_widget_set_margin_start(lbl, 20);
    gtk_widget_set_margin_bottom(lbl, 10);
    page_add(page, lbl);
}