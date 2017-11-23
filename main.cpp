#include <math.h>
#include <pango/pangocairo.h>

static void
draw_text (cairo_t *cr)
{
#define RADIUS 150
#define N_WORDS 1
#define FONT "Sans 8"

  PangoLayout *layout;
  PangoFontDescription *desc;
  int i;

  /* Center coordinates on the middle of the region we are drawing
   */
  /* Create a PangoLayout, set the font and text */
  layout = pango_cairo_create_layout (cr);



  pango_layout_set_text (layout, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nunc sollicitudin ullamcorper ipsum vitae interdum. Duis ac odio placerat, vestibulum nunc a, auctor arcu. Ut augue metus, fermentum in dictum sollicitudin, feugiat ut neque. Fusce mollis et felis vel pellentesque. Maecenas finibus diam lacus, non mattis nunc aliquam nec. Suspendisse ut molestie lorem. Aenean ac aliquam arcu, vitae rhoncus ipsum. Suspendisse congue lobortis eros, et consectetur justo venenatis et. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Mauris rutrum pulvinar sapien, eleifend sollicitudin est interdum eu. Nullam at pulvinar leo. Aliquam sed tempus lectus, nec condimentum arcu. Vestibulum ac commodo erat, eu porta mi. Ut id tortor sed nisi congue pharetra quis non libero. Suspendisse vulputate vestibulum sem, tincidunt posuere ipsum molestie eget.", -1);
  desc = pango_font_description_from_string (FONT);
  pango_layout_set_font_description (layout, desc);
  pango_font_description_free (desc);

  pango_layout_set_width(layout, 200 * PANGO_SCALE);
  pango_layout_set_wrap(layout, PANGO_WRAP_WORD);

  /* Draw the layout N_WORDS times in a circle */
      int width, height;
      double angle = 0;
      double red;

      cairo_save (cr);

      /* Gradient from red at angle == 60 to blue at angle == 240 */
      cairo_set_source_rgb (cr, 0, 0, 0);

      /* Inform Pango to re-layout the text with the new transformation */

      cairo_move_to (cr, 0, 0);
      pango_cairo_show_layout (cr, layout);

      cairo_restore (cr);

  /* free the layout object */
  g_object_unref (layout);
}

int main (int argc, char **argv)
{
  cairo_t *cr;
  char *filename;
  cairo_status_t status;
  cairo_surface_t *surface;

  if (argc != 2)
    {
      g_printerr ("Usage: cairosimple OUTPUT_FILENAME\n");
      return 1;
    }
     filename = argv[1];

  surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32,
                                        2 * RADIUS, 2 * RADIUS);
  cr = cairo_create (surface);

  cairo_set_source_rgb (cr, 1.0, 1.0, 1.0);
  cairo_paint (cr);
  draw_text (cr);
  cairo_destroy (cr);

  status = cairo_surface_write_to_png (surface, filename);
  cairo_surface_destroy (surface);

  if (status != CAIRO_STATUS_SUCCESS)
    {
      g_printerr ("Could not save png to '%s'\n", filename);
      return 1;
    }

  return 0;
}
