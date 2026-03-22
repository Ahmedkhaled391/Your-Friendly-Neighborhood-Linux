#include "runner.h"
#include "actions.h"
#include <glib.h>

static gboolean should_suppress_success_dialog(const char *action_id)
{
    if (!action_id)
        return FALSE;

    return g_str_equal(action_id, "backup_install_auto") ||
           g_str_equal(action_id, "backup_remove_auto") ||
           g_str_equal(action_id, "change_appearance") ||
           g_str_equal(action_id, "change_appearance_light") ||
           g_str_equal(action_id, "change_appearance_blue") ||
           g_str_equal(action_id, "change_appearance_green") ||
           g_str_equal(action_id, "change_appearance_orange") ||
           g_str_equal(action_id, "change_appearance_purple") ||
           g_str_equal(action_id, "change_appearance_yellow") ||
           g_str_equal(action_id, "reset_appearance_default");
}

static void free_action_context(ActionContext *ctx)
{
    if (!ctx)
        return;

    if (ctx->done_user_data_destroy)
        ctx->done_user_data_destroy(ctx->done_user_data);

    if (ctx->spinner)
        g_object_unref(ctx->spinner);
    if (ctx->run_btn)
        g_object_unref(ctx->run_btn);
    if (ctx->parent_window)
        g_object_unref(ctx->parent_window);

    g_free(ctx);
}

static void show_alert_dialog(GtkWindow *parent_window, const char *message, const char *detail)
{
    GtkAlertDialog *dialog = gtk_alert_dialog_new("%s", message ? message : "");
    gtk_alert_dialog_set_modal(dialog, TRUE);

    if (detail && *detail)
        gtk_alert_dialog_set_detail(dialog, detail);

    gtk_alert_dialog_show(dialog, parent_window);
    g_object_unref(dialog);
}

static void on_subprocess_done(GObject *src, GAsyncResult *res, gpointer user_data)
{
    GSubprocess *proc = G_SUBPROCESS(src);
    ActionContext *ctx = (ActionContext *)user_data;
    GError *err = NULL;
    char *out = NULL;
    char *errbuf = NULL;

    gboolean ok = g_subprocess_communicate_utf8_finish(proc, res, &out, &errbuf, &err);
    gboolean success = FALSE;
    int code = -1;

    if (ctx->run_btn)
        gtk_widget_set_sensitive(ctx->run_btn, TRUE);
    if (ctx->spinner)
    {
        gtk_spinner_stop(GTK_SPINNER(ctx->spinner));
        gtk_widget_set_visible(ctx->spinner, FALSE);
    }

    if (!ok || err)
    {
        const char *detail = NULL;
        if (err)
        {
            detail = err->message;
            g_error_free(err);
        }

        show_alert_dialog(ctx->parent_window, "Failed to run action.", detail);
    }
    else
    {
        code = g_subprocess_get_exit_status(proc);
        success = (code == 0);
        if (code == 0)
        {
            if (!should_suppress_success_dialog(ctx->action_id))
            {
                const char *detail = (out && strlen(out) > 0) ? out : NULL;
                show_alert_dialog(ctx->parent_window, "Done", detail);
            }
        }
        else
        {
            char *message = g_strdup_printf("Action finished with errors (exit code %d).", code);
            const char *detail = (errbuf && strlen(errbuf) > 0) ? errbuf : NULL;
            show_alert_dialog(ctx->parent_window, message, detail);
            g_free(message);
        }
    }

    if (ctx->done_cb)
        ctx->done_cb(ctx->action_id, success, code, out, errbuf, ctx->done_user_data);

    g_free(out);
    g_free(errbuf);
    free_action_context(ctx);
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
        free_action_context(ctx);
        return;
    }

    GError *err = NULL;
    const char *argv[] = {"/bin/sh", "-c", cmd, NULL};

    const gchar *dbus = g_getenv("DBUS_SESSION_BUS_ADDRESS");
    GSubprocessLauncher *launcher = g_subprocess_launcher_new(
        G_SUBPROCESS_FLAGS_STDOUT_PIPE | G_SUBPROCESS_FLAGS_STDERR_PIPE);

    if (dbus)
        g_subprocess_launcher_setenv(launcher, "DBUS_SESSION_BUS_ADDRESS", dbus, TRUE);

    GSubprocess *proc = g_subprocess_launcher_spawnv(launcher, argv, &err);
    g_object_unref(launcher);

    if (!proc)
    {
        char *message = g_strdup_printf("Could not launch process: %s", err->message);
        show_alert_dialog(ctx->parent_window, message, NULL);
        g_free(message);
        g_error_free(err);
        if (ctx->run_btn)
            gtk_widget_set_sensitive(ctx->run_btn, TRUE);
        if (ctx->spinner)
        {
            gtk_spinner_stop(GTK_SPINNER(ctx->spinner));
            gtk_widget_set_visible(ctx->spinner, FALSE);
        }
        free_action_context(ctx);
        return;
    }

    g_subprocess_communicate_utf8_async(proc, NULL, NULL, on_subprocess_done, ctx);
}

void on_run_clicked(GtkButton *btn, gpointer user_data)
{
    (void)btn;
    ActionContext *template_ctx = (ActionContext *)user_data;

    gtk_widget_set_sensitive(template_ctx->run_btn, FALSE);
    gtk_widget_set_visible(template_ctx->spinner, TRUE);
    gtk_spinner_start(GTK_SPINNER(template_ctx->spinner));

    ActionContext *run_ctx = g_new(ActionContext, 1);
    *run_ctx = *template_ctx;

    if (run_ctx->spinner)
        g_object_ref(run_ctx->spinner);
    if (run_ctx->run_btn)
        g_object_ref(run_ctx->run_btn);
    if (run_ctx->parent_window)
        g_object_ref(run_ctx->parent_window);

    run_ctx->done_cb = NULL;
    run_ctx->done_user_data = NULL;
    run_ctx->done_user_data_destroy = NULL;

    launch_script(run_ctx);
}

void run_action_async(const char *action_id, GtkWindow *parent_window)
{
    if (!action_id)
        return;

    ActionContext *ctx = g_new0(ActionContext, 1);
    ctx->action_id = action_id;
    ctx->parent_window = parent_window ? GTK_WINDOW(g_object_ref(parent_window)) : NULL;
    ctx->done_cb = NULL;
    ctx->done_user_data = NULL;
    ctx->done_user_data_destroy = NULL;
    launch_script(ctx);
}

void run_action_async_with_callback(
    const char *action_id,
    GtkWindow *parent_window,
    void (*done_cb)(const char *action_id,
                    gboolean success,
                    int exit_code,
                    const char *out,
                    const char *errbuf,
                    gpointer user_data),
    gpointer user_data,
    GDestroyNotify user_data_destroy)
{
    if (!action_id)
    {
        if (user_data_destroy)
            user_data_destroy(user_data);
        return;
    }

    ActionContext *ctx = g_new0(ActionContext, 1);
    ctx->action_id = action_id;
    ctx->parent_window = parent_window ? GTK_WINDOW(g_object_ref(parent_window)) : NULL;
    ctx->done_cb = done_cb;
    ctx->done_user_data = user_data;
    ctx->done_user_data_destroy = user_data_destroy;
    launch_script(ctx);
}