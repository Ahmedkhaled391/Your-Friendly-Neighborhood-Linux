#include "window.h"
#include "sidebar.h"
#include "pages/page_setup.h"
#include "pages/page_perf.h"
#include "pages/page_custom.h"
#include "pages/page_tools.h"
#include "pages/page_generator.h"
#include "pages/page_about.h"
#include "pages/page_firewall.h"

static gboolean theme_name_is_dark(const char *theme_name)
{
    if (!theme_name)
        return FALSE;

    char *lower_name = g_ascii_strdown(theme_name, -1);
    gboolean is_dark = strstr(lower_name, "dark") != NULL;
    g_free(lower_name);

    return is_dark;
}

static void update_window_theme_class(GtkWidget *window)
{
    GtkSettings *settings = gtk_settings_get_default();
    char *theme_name = NULL;

    if (!settings)
        return;

    g_object_get(settings, "gtk-theme-name", &theme_name, NULL);

    if (theme_name_is_dark(theme_name))
        gtk_widget_remove_css_class(window, "light-mode");
    else
        gtk_widget_add_css_class(window, "light-mode");

    g_free(theme_name);
}

static void on_theme_name_changed(GObject *object, GParamSpec *pspec, gpointer user_data)
{
    (void)object;
    (void)pspec;
    update_window_theme_class(GTK_WIDGET(user_data));
}

static const char *APP_CSS =
    "window {"
    "    background-color: #1c1c1e;"
    "}"

    "headerbar {"
    "    background-color: #232325;"
    "    border-bottom: 1px solid #3a3a3c;"
    "    min-height: 56px;"
    "}"
    "headerbar .title {"
    "    font-weight: 700;"
    "    font-size: 0.95em;"
    "    color: #ffffff;"
    "}"
    ".header-subtitle {"
    "    font-size: 0.78em;"
    "    color: #8e8e93;"
    "}"

    ".sidebar {"
    "    background-color: #232325;"
    "    border-right: 1px solid #3a3a3c;"
    "}"
    ".sidebar row {"
    "    padding: 2px 8px;"
    "    border-radius: 0px;"
    "}"
    ".sidebar row:hover {"
    "    background-color: #2c2c2e;"
    "}"
    ".sidebar row:selected {"
    "    background-color: transparent;"
    "}"
    ".sidebar row:selected .nav-row-box {"
    "    background-color: #0a84ff;"
    "    border-radius: 10px;"
    "}"
    ".sidebar row:selected .nav-label {"
    "    color: #ffffff;"
    "    font-weight: 600;"
    "}"
    ".sidebar row:selected .nav-icon {"
    "    color: #ffffff;"
    "}"
    ".nav-row-box {"
    "    padding: 8px 12px;"
    "    border-radius: 10px;"
    "    transition: background-color 150ms ease;"
    "}"
    ".nav-label {"
    "    font-size: 0.92em;"
    "    color: #ebebf5;"
    "}"
    ".nav-icon {"
    "    color: #8e8e93;"
    "}"
    ".active-badge {"
    "    font-size: 0.65em;"
    "    color: #ffffff;"
    "    opacity: 0.7;"
    "}"

    ".page-title {"
    "    font-size: 1.4em;"
    "    font-weight: 700;"
    "    color: #ffffff;"
    "    padding-left: 4px;"
    "}"

    ".action-card {"
    "    background-color: #2c2c2e;"
    "    border-radius: 12px;"
    "    border: 1px solid #3a3a3c;"
    "}"
    ".action-card:hover {"
    "    background-color: #323234;"
    "    border-color: #48484a;"
    "}"
    ".card-title {"
    "    font-size: 0.95em;"
    "    font-weight: 600;"
    "    color: #ffffff;"
    "}"
    ".card-desc {"
    "    font-size: 0.82em;"
    "    color: #8e8e93;"
    "}"

    ".run-btn {"
    "    background-color: #0a84ff;"
    "    color: #ffffff;"
    "    border-radius: 20px;"
    "    border: none;"
    "    padding: 7px 18px;"
    "    font-weight: 700;"
    "    font-size: 0.88em;"
    "    box-shadow: 0 8px 18px rgba(0, 0, 0, 0.25);"
    "    transition: transform 140ms ease, background-color 140ms ease, box-shadow 140ms ease;"
    "}"
    ".run-btn-label {"
    "    color: #ffffff;"
    "    font-weight: 700;"
    "}"
    ".run-btn-icon {"
    "    color: #ffffff;"
    "}"
    ".run-btn:hover {"
    "    background-color: #409cff;"
    "    transform: translateY(-1px);"
    "    box-shadow: 0 10px 22px rgba(0, 0, 0, 0.32);"
    "}"
    ".run-btn:active {"
    "    background-color: #0071e3;"
    "    transform: translateY(0);"
    "    box-shadow: 0 6px 14px rgba(0, 0, 0, 0.25);"
    "}"
    ".run-btn:disabled {"
    "    background-color: rgba(255, 255, 255, 0.12);"
    "    color: rgba(255, 255, 255, 0.55);"
    "    box-shadow: none;"
    "}"
    ".reset-btn {"
    "    background-color: rgba(255, 255, 255, 0.08);"
    "    color: #ffffff;"
    "    border-radius: 20px;"
    "    border: 1px solid #3a3a3c;"
    "    padding: 7px 18px;"
    "    font-weight: 700;"
    "    font-size: 0.88em;"
    "}"
    ".reset-btn-label {"
    "    color: #ffffff;"
    "    font-weight: 700;"
    "}"
    ".reset-btn-icon {"
    "    color: #ffffff;"
    "}"
    ".reset-btn:hover {"
    "    background-color: rgba(255, 255, 255, 0.14);"
    "    border-color: #48484a;"
    "}"
    ".reset-btn:disabled {"
    "    background-color: rgba(255, 255, 255, 0.06);"
    "    color: rgba(255, 255, 255, 0.44);"
    "    border-color: rgba(255, 255, 255, 0.08);"
    "}"

    ".about-title {"
    "    font-size: 1.5em;"
    "    font-weight: 700;"
    "    color: #ffffff;"
    "}"
    ".about-version {"
    "    font-size: 0.85em;"
    "    color: #8e8e93;"
    "}"
    ".about-desc {"
    "    font-size: 0.92em;"
    "    color: #ebebf5;"
    "}"

    "switch.firewall-switch {"
    "    min-width: 52px;"
    "    min-height: 30px;"
    "    padding: 3px;"
    "    border-radius: 15px;"
    "    background-color: #24262d;"
    "    border: 1px solid #3a3f49;"
    "    box-shadow: inset 0 1px 2px rgba(0, 0, 0, 0.35);"
    "}"
    "switch.firewall-switch:checked {"
    "    background-color: #1f84ff;"
    "    border-color: #2f8fff;"
    "}"
    "switch.firewall-switch slider {"
    "    min-width: 24px;"
    "    min-height: 24px;"
    "    border-radius: 12px;"
    "    background-color: #2b3038;"
    "    border: 1px solid #0f1218;"
    "    box-shadow: 0 1px 2px rgba(0, 0, 0, 0.45);"
    "}"

    "scrolledwindow {"
    "    background-color: transparent;"
    "}"
    "scrolledwindow > viewport {"
    "    background-color: transparent;"
    "}"

    ".content-area {"
    "    background-color: #1c1c1e;"
    "}"
    "window.light-mode {"
    "    background-color: #f5f6f8;"
    "}"
    "window.light-mode headerbar {"
    "    background-color: #f4f5f7;"
    "    border-bottom: 1px solid #d6dae1;"
    "}"
    "window.light-mode headerbar .title {"
    "    color: #16181d;"
    "}"
    "window.light-mode .header-subtitle {"
    "    color: #5f6775;"
    "}"
    "window.light-mode .sidebar {"
    "    background-color: #eef1f5;"
    "    border-right: 1px solid #d6dae1;"
    "}"
    "window.light-mode .sidebar row:hover {"
    "    background-color: #e7ebf1;"
    "}"
    "window.light-mode .sidebar row:selected .nav-row-box {"
    "    background-color: #dbe7ff;"
    "}"
    "window.light-mode .sidebar row:selected .nav-label {"
    "    color: #16181d;"
    "}"
    "window.light-mode .sidebar row:selected .nav-icon {"
    "    color: #16181d;"
    "}"
    "window.light-mode .nav-label {"
    "    color: #16181d;"
    "}"
    "window.light-mode .nav-icon {"
    "    color: #4b5563;"
    "}"
    "window.light-mode .active-badge {"
    "    color: #16181d;"
    "}"
    "window.light-mode .page-title {"
    "    color: #16181d;"
    "}"
    "window.light-mode .action-card {"
    "    background-color: #ffffff;"
    "    border: 1px solid #d6dae1;"
    "    box-shadow: 0 8px 18px rgba(15, 23, 42, 0.08);"
    "}"
    "window.light-mode .action-card:hover {"
    "    background-color: #f8fafc;"
    "    border-color: #c8d0dc;"
    "}"
    "window.light-mode .card-title {"
    "    color: #16181d;"
    "}"
    "window.light-mode .card-desc {"
    "    color: #4b5563;"
    "}"
    "window.light-mode .run-btn {"
    "    background-color: #dbe7ff;"
    "    color: #16181d;"
    "    border: 1px solid #bfd1ff;"
    "    box-shadow: 0 6px 14px rgba(37, 99, 235, 0.10);"
    "}"
    "window.light-mode .run-btn-label {"
    "    color: #16181d;"
    "}"
    "window.light-mode .run-btn-icon {"
    "    color: #16181d;"
    "}"
    "window.light-mode .run-btn:hover {"
    "    background-color: #cfdfff;"
    "}"
    "window.light-mode .run-btn:disabled {"
    "    background-color: #eef2f7;"
    "    color: #6b7280;"
    "    border-color: #d6dae1;"
    "}"
    "window.light-mode .reset-btn {"
    "    background-color: #eef2f7;"
    "    color: #16181d;"
    "    border-color: #c8d0dc;"
    "}"
    "window.light-mode .reset-btn-label {"
    "    color: #16181d;"
    "}"
    "window.light-mode .reset-btn-icon {"
    "    color: #16181d;"
    "}"
    "window.light-mode .reset-btn:hover {"
    "    background-color: #e5ebf3;"
    "    border-color: #b6c1cf;"
    "}"
    "window.light-mode .about-title {"
    "    color: #16181d;"
    "}"
    "window.light-mode .about-version {"
    "    color: #5f6775;"
    "}"
    "window.light-mode .about-desc {"
    "    color: #2f3743;"
    "}"
    "window.light-mode .content-area {"
    "    background-color: #f5f6f8;"
    "}";

void on_activate(GtkApplication *app, gpointer user_data)
{

    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Your Friendly Neighborhood Linux");
    gtk_window_set_default_size(GTK_WINDOW(window), 860, 540);
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(provider, APP_CSS);
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);

    update_window_theme_class(window);

    GtkSettings *settings = gtk_settings_get_default();
    if (settings)
    {
        g_signal_connect(settings, "notify::gtk-theme-name",
                         G_CALLBACK(on_theme_name_changed), window);
    }

    GtkWidget *headerbar = gtk_header_bar_new();
    gtk_window_set_titlebar(GTK_WINDOW(window), headerbar);

    GtkWidget *title_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    gtk_widget_set_valign(title_box, GTK_ALIGN_CENTER);

    GtkWidget *h_title = gtk_label_new("Your Friendly Neighborhood Linux");
    gtk_widget_add_css_class(h_title, "title");

    GtkWidget *h_sub = gtk_label_new("A friendly assistant for configuring your Linux system");
    gtk_widget_add_css_class(h_sub, "header-subtitle");

    gtk_box_append(GTK_BOX(title_box), h_title);
    gtk_box_append(GTK_BOX(title_box), h_sub);
    gtk_header_bar_set_title_widget(GTK_HEADER_BAR(headerbar), title_box);

    GtkWidget *root = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_window_set_child(GTK_WINDOW(window), root);

    GtkWidget *stack = gtk_stack_new();
    gtk_stack_set_transition_type(GTK_STACK(stack), GTK_STACK_TRANSITION_TYPE_CROSSFADE);
    gtk_stack_set_transition_duration(GTK_STACK(stack), 150);
    gtk_widget_set_hexpand(stack, TRUE);
    gtk_widget_set_vexpand(stack, TRUE);
    gtk_widget_add_css_class(stack, "content-area");

    NavData *nd = g_new(NavData, 1);
    nd->stack = GTK_STACK(stack);
    nd->pages[0] = "setup";
    nd->pages[1] = "perf";
    nd->pages[2] = "custom";
    nd->pages[3] = "tools";
    nd->pages[4] = "generator";
    nd->pages[5] = "firewall";
    nd->pages[6] = "about";

    GtkWidget *sidebar = create_sidebar(nd);
    gtk_box_append(GTK_BOX(root), sidebar);

    GtkWidget *sep = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
    gtk_box_append(GTK_BOX(root), sep);

    gtk_box_append(GTK_BOX(root), stack);

    GtkWindow *win = GTK_WINDOW(window);

    gtk_stack_add_named(GTK_STACK(stack), create_page_setup(win), "setup");
    gtk_stack_add_named(GTK_STACK(stack), create_page_perf(win), "perf");
    gtk_stack_add_named(GTK_STACK(stack), create_page_custom(win), "custom");
    gtk_stack_add_named(GTK_STACK(stack), create_page_generator(win), "generator");
    gtk_stack_add_named(GTK_STACK(stack), create_page_tools(win), "tools");
    gtk_stack_add_named(GTK_STACK(stack), create_page_firewall(win), "firewall");
    gtk_stack_add_named(GTK_STACK(stack), create_page_about(), "about");

    // Free NavData when window is destroyed
    g_object_set_data_full(G_OBJECT(window), "nav-data", nd, g_free);

    // Select first row by default
    gtk_list_box_select_row(
        nd->listbox,
        gtk_list_box_get_row_at_index(nd->listbox, 0));

    gtk_window_present(GTK_WINDOW(window));
}