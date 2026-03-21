#include "page_custom.h"
#include "pages.h"
#include "../widgets/card.h"
#include "../../logic/runner.h"

static GtkWidget *create_reset_button_row(GtkWindow *parent)
{
    GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_halign(row, GTK_ALIGN_END);
    gtk_widget_set_margin_start(row, 16);
    gtk_widget_set_margin_end(row, 16);
    gtk_widget_set_margin_top(row, 12);
    gtk_widget_set_margin_bottom(row, 6);

    GtkWidget *spinner = gtk_spinner_new();
    gtk_widget_set_visible(spinner, FALSE);
    gtk_widget_set_size_request(spinner, 20, 20);

    GtkWidget *btn_content = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    GtkWidget *btn_icon = gtk_image_new_from_icon_name("view-refresh-symbolic");
    GtkWidget *btn_label = gtk_label_new("Reset Default");
    gtk_widget_add_css_class(btn_icon, "reset-btn-icon");
    gtk_widget_add_css_class(btn_label, "reset-btn-label");
    gtk_box_append(GTK_BOX(btn_content), btn_icon);
    gtk_box_append(GTK_BOX(btn_content), btn_label);

    GtkWidget *reset_btn = gtk_button_new();
    gtk_button_set_child(GTK_BUTTON(reset_btn), btn_content);
    gtk_widget_add_css_class(reset_btn, "reset-btn");

    ActionContext *ctx = g_new(ActionContext, 1);
    ctx->action_id = "reset_appearance_default";
    ctx->spinner = spinner;
    ctx->run_btn = reset_btn;
    ctx->parent_window = parent;

    g_object_set_data_full(G_OBJECT(reset_btn), "ctx", ctx, g_free);
    g_signal_connect(reset_btn, "clicked", G_CALLBACK(on_run_clicked), ctx);

    gtk_box_append(GTK_BOX(row), spinner);
    gtk_box_append(GTK_BOX(row), reset_btn);

    return row;
}

GtkWidget *create_page_custom(GtkWindow *parent)
{
    GtkWidget *page = make_page();
    page_add_title(page, "System Appearance");
    page_add(page, create_card(
                       "Apply System Dark Theme",
                       "Changes the GNOME system theme to Ubuntu Yaru dark with Adwaita icons.",
                       "change_appearance", parent));
    page_add(page, create_card(
                       "Apply System Light Theme",
                       "Changes the GNOME system theme to Ubuntu Yaru light with Adwaita icons.",
                       "change_appearance_light", parent));
    page_add(page, create_card(
                       "Apply Blue System Theme",
                       "Changes the GNOME system theme to Ubuntu Yaru blue dark.",
                       "change_appearance_blue", parent));
    page_add(page, create_card(
                       "Apply Green System Theme",
                       "Changes the GNOME system theme to Ubuntu Yaru viridian dark.",
                       "change_appearance_green", parent));
    page_add(page, create_card(
                       "Apply Red System Theme",
                       "Changes the GNOME system theme to Ubuntu Yaru red dark.",
                       "change_appearance_orange", parent));
    page_add(page, create_card(
                       "Apply Purple System Theme",
                       "Changes the GNOME system theme to Ubuntu Yaru purple dark.",
                       "change_appearance_purple", parent));
    page_add(page, create_card(
                       "Apply Yellow System Theme",
                       "Changes the GNOME system theme to Ubuntu Yaru bark dark, the closest installed yellow variant.",
                       "change_appearance_yellow", parent));
    page_add(page, create_reset_button_row(parent));
    return page;
}