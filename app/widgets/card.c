#include "card.h"
#include "../../logic/runner.h"

GtkWidget *create_card_with_state(
    const char *title,
    const char *desc,
    const char *action_id,
    GtkWindow *parent,
    gboolean is_current)
{

    GtkWidget *frame = gtk_frame_new(NULL);
    gtk_widget_add_css_class(frame, "action-card");
    if (is_current)
        gtk_widget_add_css_class(frame, "action-card-current");
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

    GtkWidget *lbl_title = gtk_label_new(title);
    gtk_widget_set_halign(lbl_title, GTK_ALIGN_START);
    gtk_widget_add_css_class(lbl_title, "card-title");

    GtkWidget *lbl_desc = gtk_label_new(desc);
    gtk_widget_set_halign(lbl_desc, GTK_ALIGN_START);
    gtk_label_set_wrap(GTK_LABEL(lbl_desc), TRUE);
    gtk_label_set_max_width_chars(GTK_LABEL(lbl_desc), 55);
    gtk_widget_add_css_class(lbl_desc, "card-desc");

    gtk_box_append(GTK_BOX(text_box), lbl_title);
    gtk_box_append(GTK_BOX(text_box), lbl_desc);

    GtkWidget *right = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_valign(right, GTK_ALIGN_CENTER);

    GtkWidget *spinner = gtk_spinner_new();
    gtk_widget_set_visible(spinner, FALSE);
    gtk_widget_set_size_request(spinner, 20, 20);

    GtkWidget *btn_content = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    GtkWidget *btn_icon = gtk_image_new_from_icon_name(
        is_current ? "object-select-symbolic" : "media-playback-start-symbolic");
    GtkWidget *btn_label = gtk_label_new(is_current ? "Current" : "Run");
    gtk_widget_add_css_class(btn_icon, "run-btn-icon");
    gtk_widget_add_css_class(btn_label, "run-btn-label");
    gtk_box_append(GTK_BOX(btn_content), btn_icon);
    gtk_box_append(GTK_BOX(btn_content), btn_label);
    gtk_widget_set_halign(btn_content, GTK_ALIGN_CENTER);

    GtkWidget *run_btn = gtk_button_new();
    gtk_button_set_child(GTK_BUTTON(run_btn), btn_content);
    gtk_widget_add_css_class(run_btn, "run-btn");
    if (is_current)
        gtk_widget_add_css_class(run_btn, "current-btn");
    gtk_widget_set_valign(run_btn, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(right), spinner);
    gtk_box_append(GTK_BOX(right), run_btn);

    if (is_current)
    {
        gtk_widget_set_sensitive(run_btn, FALSE);
    }
    else
    {

        ActionContext *ctx = g_new(ActionContext, 1);
        ctx->action_id = action_id;
        ctx->spinner = spinner;
        ctx->run_btn = run_btn;
        ctx->parent_window = parent;
        g_object_set_data_full(G_OBJECT(run_btn), "ctx", ctx, g_free);
        g_signal_connect(run_btn, "clicked", G_CALLBACK(on_run_clicked), ctx);
    }

    gtk_box_append(GTK_BOX(hbox), text_box);
    gtk_box_append(GTK_BOX(hbox), right);

    return frame;
}

GtkWidget *create_card(
    const char *title,
    const char *desc,
    const char *action_id,
    GtkWindow *parent)
{
    return create_card_with_state(title, desc, action_id, parent, FALSE);
}