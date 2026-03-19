#include "page_custom.h"
#include "pages.h"
#include "../widgets/card.h"

GtkWidget *create_page_custom(GtkWindow *parent)
{
    GtkWidget *page = make_page();
    page_add_title(page, "Customization");
    page_add(page, create_card(
                       "Apply Adwaita-dark Theme",
                       "Switches to Adwaita-dark for special icons and dark mode .",
                       "change_appearance", parent));
    return page;
}