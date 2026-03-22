#include "runner.h"
#include "actions.h"
#include <glib.h>

static void on_subprocess_done(GObject *src, GAsyncResult *res, gpointer user_data)
{
    GSubprocess *proc = G_SUBPROCESS(src);
    ActionContext *ctx = (ActionContext *)user_data;
    GError *err = NULL;
    char *out = NULL;
    char *errbuf = NULL;

    gboolean ok = g_subprocess_communicate_utf8_finish(proc, res, &out, &errbuf, &err);

    if (ctx->run_btn)
        gtk_widget_set_sensitive(ctx->run_btn, TRUE);
    if (ctx->spinner)
    {
        gtk_spinner_stop(GTK_SPINNER(ctx->spinner));
        gtk_widget_set_visible(ctx->spinner, FALSE);
    }

    GtkWidget *dialog;

    if (!ok || err)
    {
        dialog = gtk_message_dialog_new(
            ctx->parent_window,
            GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE,
            "Failed to run action.");
        if (err)
        {
            gtk_message_dialog_format_secondary_text(
                GTK_MESSAGE_DIALOG(dialog), "%s", err->message);
            g_error_free(err);
        }
    }
    else
    {
        int code = g_subprocess_get_exit_status(proc);
        if (code == 0)
        {
            dialog = gtk_message_dialog_new(
                ctx->parent_window,
                GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE,
                "Done");
            if (out && strlen(out) > 0)
                gtk_message_dialog_format_secondary_text(
                    GTK_MESSAGE_DIALOG(dialog), "%s", out);
        }
        else
        {
            dialog = gtk_message_dialog_new(
                ctx->parent_window,
                GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_WARNING, GTK_BUTTONS_CLOSE,
                "Action finished with errors (exit code %d).", code);
            if (errbuf && strlen(errbuf) > 0)
                gtk_message_dialog_format_secondary_text(
                    GTK_MESSAGE_DIALOG(dialog), "%s", errbuf);
        }
    }

    gtk_window_present(GTK_WINDOW(dialog));
    g_signal_connect(dialog, "response", G_CALLBACK(gtk_window_destroy), NULL);

    g_free(out);
    g_free(errbuf);
    g_free(ctx);
    g_object_unref(proc);
}

static void launch_script(ActionContext *ctx)
{
    const char *cmd = get_script(ctx->action_id);
    if (!cmd)
    {
        g_warning("No script mapped for: %s", ctx->action_id);
        if (ctx->run_btn)
            gtk_widget_set_sensitive(ctx->run_btn, TRUE);
        if (ctx->spinner)
        {
            gtk_spinner_stop(GTK_SPINNER(ctx->spinner));
            gtk_widget_set_visible(ctx->spinner, FALSE);
        }
        g_free(ctx);
        return;
    }

    GError *err = NULL;
    const char *argv[] = {"/bin/sh", "-c", cmd, NULL};

    // pass D-Bus session to the subprocess so gsettings works
    const gchar *dbus = g_getenv("DBUS_SESSION_BUS_ADDRESS");
    GSubprocessLauncher *launcher = g_subprocess_launcher_new(
        G_SUBPROCESS_FLAGS_STDOUT_PIPE | G_SUBPROCESS_FLAGS_STDERR_PIPE);

    if (dbus)
        g_subprocess_launcher_setenv(launcher, "DBUS_SESSION_BUS_ADDRESS", dbus, TRUE);

    GSubprocess *proc = g_subprocess_launcher_spawnv(launcher, argv, &err);
    g_object_unref(launcher);

    if (!proc)
    {
        GtkWidget *dialog = gtk_message_dialog_new(
            ctx->parent_window,
            GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE,
            "Could not launch process: %s", err->message);
        gtk_window_present(GTK_WINDOW(dialog));
        g_signal_connect(dialog, "response", G_CALLBACK(gtk_window_destroy), NULL);
        g_error_free(err);
        if (ctx->run_btn)
            gtk_widget_set_sensitive(ctx->run_btn, TRUE);
        if (ctx->spinner)
        {
            gtk_spinner_stop(GTK_SPINNER(ctx->spinner));
            gtk_widget_set_visible(ctx->spinner, FALSE);
        }
        g_free(ctx);
        return;
    }

    g_subprocess_communicate_utf8_async(proc, NULL, NULL, on_subprocess_done, ctx);
}

void on_run_clicked(GtkButton *btn, gpointer user_data)
{
    ActionContext *template_ctx = (ActionContext *)user_data;

    gtk_widget_set_sensitive(template_ctx->run_btn, FALSE);
    gtk_widget_set_visible(template_ctx->spinner, TRUE);
    gtk_spinner_start(GTK_SPINNER(template_ctx->spinner));

   
    ActionContext *run_ctx = g_new(ActionContext, 1);
    *run_ctx = *template_ctx;

    launch_script(run_ctx);
}

void run_action_async(const char *action_id, GtkWindow *parent_window)
{
    if (!action_id)
        return;

    ActionContext *ctx = g_new0(ActionContext, 1);
    ctx->action_id = action_id;
    ctx->parent_window = parent_window;
    launch_script(ctx);
}