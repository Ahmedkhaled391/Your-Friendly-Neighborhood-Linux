#include "page_custom.h"
#include "pages.h"
#include "../widgets/card.h"
#include "../../logic/runner.h"
#include <string.h>

typedef struct
{
    const char *title;
    const char *desc;
    const char *theme_name;
    const char *action_id;
} AppearanceOption;

static const AppearanceOption APPEARANCE_OPTIONS[] = {
    {"Apply System Dark Theme", "Changes the GNOME system theme to Ubuntu Yaru dark with Adwaita icons.", "Yaru-dark", "change_appearance"},
    {"Apply System Light Theme", "Changes the GNOME system theme to Ubuntu Yaru light with Adwaita icons.", "Yaru", "change_appearance_light"},
    {"Apply Blue System Theme", "Changes the GNOME system theme to Ubuntu Yaru blue dark.", "Yaru-blue-dark", "change_appearance_blue"},
    {"Apply Green System Theme", "Changes the GNOME system theme to Ubuntu Yaru viridian dark.", "Yaru-viridian-dark", "change_appearance_green"},
    {"Apply Red System Theme", "Changes the GNOME system theme to Ubuntu Yaru red dark.", "Yaru-red-dark", "change_appearance_orange"},
    {"Apply Purple System Theme", "Changes the GNOME system theme to Ubuntu Yaru purple dark.", "Yaru-purple-dark", "change_appearance_purple"},
    {"Apply Yellow System Theme", "Changes the GNOME system theme to Ubuntu Yaru bark dark, the closest installed yellow variant.", "Yaru-bark-dark", "change_appearance_yellow"},
};

static const guint APPEARANCE_OPTIONS_COUNT = G_N_ELEMENTS(APPEARANCE_OPTIONS);

static const char *get_current_appearance_action_id(void)
{
    const char *action_id = NULL;
    GSettings *settings = g_settings_new("org.gnome.desktop.interface");
    if (!settings)
        return NULL;

    gchar *theme = g_settings_get_string(settings, "gtk-theme");
    if (theme)
    {
        for (guint i = 0; i < APPEARANCE_OPTIONS_COUNT; i++)
        {
            if (strcmp(theme, APPEARANCE_OPTIONS[i].theme_name) == 0)
            {
                action_id = APPEARANCE_OPTIONS[i].action_id;
                break;
            }
        }
    }

    g_free(theme);
    g_object_unref(settings);
    return action_id;
}

static GtkWidget *create_appearance_card(const AppearanceOption *option, const char *current_action_id, GtkWindow *parent)
{
    gboolean is_current = current_action_id && strcmp(current_action_id, option->action_id) == 0;
    return create_card_with_state(option->title, option->desc, option->action_id, parent, is_current);
}

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

static void rebuild_custom_page(GtkWidget *page)
{
    GtkWidget *vbox = GTK_WIDGET(g_object_get_data(G_OBJECT(page), "vbox"));
    GtkWindow *parent = GTK_WINDOW(g_object_get_data(G_OBJECT(page), "custom-parent"));
    const char *current_action_id;

    if (!vbox || !parent)
        return;

    while (gtk_widget_get_first_child(vbox))
        gtk_box_remove(GTK_BOX(vbox), gtk_widget_get_first_child(vbox));

    current_action_id = get_current_appearance_action_id();

    page_add_title(page, "System Appearance");
    for (guint i = 0; i < APPEARANCE_OPTIONS_COUNT; i++)
        page_add(page, create_appearance_card(&APPEARANCE_OPTIONS[i], current_action_id, parent));

    page_add(page, create_reset_button_row(parent));
}

static void on_custom_theme_changed(GSettings *settings, gchar *key, gpointer user_data)
{
    (void)settings;
    (void)key;
    rebuild_custom_page(GTK_WIDGET(user_data));
}

GtkWidget *create_page_custom(GtkWindow *parent)
{
    GtkWidget *page = make_page();

    g_object_set_data(G_OBJECT(page), "custom-parent", parent);
    rebuild_custom_page(page);

    GSettings *settings = g_settings_new("org.gnome.desktop.interface");
    if (settings)
    {
        g_signal_connect_object(settings, "changed::gtk-theme", G_CALLBACK(on_custom_theme_changed), page, 0);
        g_object_set_data_full(G_OBJECT(page), "custom-theme-settings", settings, g_object_unref);
    }

    return page;
}