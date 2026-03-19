#include "page_generator.h"
#include "pages.h"
#include "../widgets/card.h"
#include "../../logic/passwordGenerator.h"
#include <string.h>
#include <stdlib.h>

typedef struct {
    GtkWidget *spin;
    GtkWidget *entry;
} GeneratorData;

static void on_generator_data_free(gpointer data, GClosure *closure)
{
    g_free(data);
}

static void on_generate_password(GtkWidget *button, gpointer user_data)
{
    GeneratorData *data = (GeneratorData *)user_data;
    int length = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(data->spin));
    char *password = randomPasswordGeneration(length);

    if (password)
    {
        gtk_editable_set_text(GTK_EDITABLE(data->entry), password);
        free(password);
    }
}

static void on_copy_password(GtkWidget *button, gpointer user_data)
{
    GtkEntry *entry = GTK_ENTRY(user_data);
    const char *text = gtk_editable_get_text(GTK_EDITABLE(entry));

    if (text && strlen(text) > 0)
    {
        GdkDisplay *display = gdk_display_get_default();
        GdkClipboard *clipboard = gdk_display_get_clipboard(display);
        gdk_clipboard_set_text(clipboard, text);
    }
}

GtkWidget *create_page_generator(GtkWindow *parent)
{
    GtkWidget *page = make_page();
    page_add_title(page, "Password Generator");

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_start(box, 15);
    gtk_widget_set_margin_end(box, 15);
    gtk_widget_set_margin_top(box, 15);
    gtk_widget_set_margin_bottom(box, 15);

    GtkWidget *length_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget *length_label = gtk_label_new("Password Length:");
    GtkWidget *length_spin = gtk_spin_button_new_with_range(8, 64, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(length_spin), 16);
    gtk_box_append(GTK_BOX(length_box), length_label);
    gtk_box_append(GTK_BOX(length_box), length_spin);
    gtk_box_append(GTK_BOX(box), length_box);

    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget *generate_btn = gtk_button_new_with_label("Generate");
    GtkWidget *copy_btn = gtk_button_new_with_label("Copy");

    GtkWidget *password_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(password_entry), "Generated password: ");
    gtk_editable_set_editable(GTK_EDITABLE(password_entry), FALSE);

    GeneratorData *data = g_new(GeneratorData, 1);
    data->spin  = length_spin;
    data->entry = password_entry;

    g_signal_connect_data(generate_btn, "clicked", G_CALLBACK(on_generate_password),
                          data, on_generator_data_free, 0);
    g_signal_connect(copy_btn, "clicked", G_CALLBACK(on_copy_password), password_entry);

    gtk_box_append(GTK_BOX(button_box), generate_btn);
    gtk_box_append(GTK_BOX(button_box), copy_btn);
    gtk_box_append(GTK_BOX(box), password_entry);
    gtk_box_append(GTK_BOX(box), button_box);

    page_add(page, box);
    return page;
}