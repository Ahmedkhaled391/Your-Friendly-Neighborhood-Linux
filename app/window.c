#include "window.h"
#include "sidebar.h"
#include "pages/page_setup.h"
#include "pages/page_perf.h"
#include "pages/page_custom.h"
#include "pages/page_tools.h"
#include "pages/page_generator.h"
#include "pages/page_about.h"


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
    ".run-btn label {"
    "    color: #ffffff;"
    "    font-weight: 700;"
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

    "scrolledwindow {"
    "    background-color: transparent;"
    "}"
    "scrolledwindow > viewport {"
    "    background-color: transparent;"
    "}"

    ".content-area {"
    "    background-color: #1c1c1e;"
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
    nd->pages[5] = "about";

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
    gtk_stack_add_named(GTK_STACK(stack), create_page_about(), "about");

    // Free NavData when window is destroyed
    g_object_set_data_full(G_OBJECT(window), "nav-data", nd, g_free);

    // Select first row by default
    gtk_list_box_select_row(
        nd->listbox,
        gtk_list_box_get_row_at_index(nd->listbox, 0));

    gtk_window_present(GTK_WINDOW(window));
}