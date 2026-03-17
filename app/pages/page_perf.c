#include "page_perf.h"
#include "pages.h"
#include "../widgets/card.h"

GtkWidget *create_page_perf(GtkWindow *parent)
{
    GtkWidget *page = make_page();
    page_add_title(page, "Performance Tweaks");
    page_add(page, create_card(
                       "Enable Useful System Tweaks",
                       "Optimizes swappiness, improves disk performance for standard use.",
                       "enable_tweaks", parent));
    page_add(page, create_card(
                       "Clean System Cache",
                       "Frees up disk space by clearing apt cache and temp files.",
                       "clean_cache", parent));
    return page;
}