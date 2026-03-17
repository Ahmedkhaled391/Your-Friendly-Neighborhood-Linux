#include "page_about.h"
#include "pages.h"

GtkWidget *create_page_about(void)
{
    GtkWidget *page = make_page();
    {
        GtkWidget *vbox = GTK_WIDGET(g_object_get_data(G_OBJECT(page), "vbox"));
        gtk_widget_set_valign(vbox, GTK_ALIGN_CENTER);
        gtk_widget_set_halign(vbox, GTK_ALIGN_CENTER);
        gtk_widget_set_vexpand(vbox, TRUE);

        GtkWidget *icon = gtk_image_new_from_icon_name("preferences-system-symbolic");
        gtk_image_set_pixel_size(GTK_IMAGE(icon), 64);
        gtk_widget_set_margin_bottom(icon, 16);

        GtkWidget *a_title = gtk_label_new("Your Friendly Neighborhood Linux");
        gtk_widget_add_css_class(a_title, "about-title");
        gtk_widget_set_margin_bottom(a_title, 6);

        GtkWidget *a_ver = gtk_label_new("Version 1.0");
        gtk_widget_add_css_class(a_ver, "about-version");
        gtk_widget_set_margin_bottom(a_ver, 16);

        GtkWidget *a_desc = gtk_label_new(
            "Making Linux accessible to everyone\nwithout ever touching a terminal.");
        gtk_widget_add_css_class(a_desc, "about-desc");
        gtk_label_set_justify(GTK_LABEL(a_desc), GTK_JUSTIFY_CENTER);

        gtk_box_append(GTK_BOX(vbox), icon);
        gtk_box_append(GTK_BOX(vbox), a_title);
        gtk_box_append(GTK_BOX(vbox), a_ver);
        gtk_box_append(GTK_BOX(vbox), a_desc);
    }
    return page;
}