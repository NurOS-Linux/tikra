using Gtk;
using Adw;

namespace Tikra {
    public class ClockPage : Box {
        private Label time_label;
        private Label date_label;
        private uint timeout_id;

        public ClockPage () {
            Object (orientation: Orientation.VERTICAL, spacing: 24);
            setup_ui ();
            start_clock ();
        }

        private void setup_ui () {
            halign = Align.FILL;
            valign = Align.FILL;
            hexpand = true;
            vexpand = true;

            // Create a centered container with elegant styling
            var main_card = new Adw.Clamp ();
            main_card.maximum_size = 600;
            main_card.margin_top = 48;
            main_card.margin_bottom = 48;
            main_card.margin_start = 24;
            main_card.margin_end = 24;
            main_card.halign = Align.CENTER;
            main_card.valign = Align.CENTER;

            var card_box = new Box (Orientation.VERTICAL, 32);
            card_box.halign = Align.CENTER;
            card_box.valign = Align.CENTER;

            // Time display with elegant background
            var time_card = new Adw.Bin ();
            time_card.add_css_class ("card");
            time_card.add_css_class ("time-display");
            
            var time_box = new Box (Orientation.VERTICAL, 8);
            time_box.margin_top = 32;
            time_box.margin_bottom = 32;
            time_box.margin_start = 48;
            time_box.margin_end = 48;
            time_box.halign = Align.CENTER;

            time_label = new Label ("");
            time_label.add_css_class ("title-1");
            time_label.add_css_class ("numeric");
            time_label.add_css_class ("monospace");
            time_label.add_css_class ("time-large");
            
            time_box.append (time_label);
            time_card.child = time_box;

            // Date information with subtle styling
            var date_box = new Box (Orientation.VERTICAL, 8);
            date_box.halign = Align.CENTER;
            
            date_label = new Label ("");
            date_label.add_css_class ("title-2");
            date_label.add_css_class ("dim-label");
            
            var day_label = new Label ("");
            day_label.add_css_class ("title-3");
            day_label.add_css_class ("accent");
            
            date_box.append (date_label);
            date_box.append (day_label);

            card_box.append (time_card);
            card_box.append (date_box);
            
            main_card.child = card_box;
            append (main_card);
            
            update_time ();
            update_date (date_label, day_label);
        }

        private void start_clock () {
            timeout_id = Timeout.add_seconds (1, () => {
                update_time ();
                return Source.CONTINUE;
            });
            
            // Update date every minute
            Timeout.add_seconds (60, () => {
                var child = get_first_child ();
                if (child is Adw.Clamp) {
                    var clamp = child as Adw.Clamp;
                    var card_box = clamp.child as Box;
                    if (card_box != null) {
                        var date_box = card_box.get_last_child () as Box;
                        if (date_box != null) {
                            var date_label_child = date_box.get_first_child () as Label;
                            var day_label_child = date_box.get_last_child () as Label;
                            if (date_label_child != null) {
                                update_date (date_label_child, day_label_child);
                            }
                        }
                    }
                }
                return Source.CONTINUE;
            });
        }

        private void update_time () {
            var date_time = new DateTime.now_local ();
            time_label.label = date_time.format ("%H:%M:%S");
        }

        private void update_date (Label date_label, Label? day_label) {
            var date_time = new DateTime.now_local ();
            date_label.label = date_time.format ("%B %d, %Y");
            if (day_label != null) {
                day_label.label = date_time.format ("%A");
            }
        }

        public override void dispose () {
            if (timeout_id != 0) {
                Source.remove (timeout_id);
                timeout_id = 0;
            }
            base.dispose ();
        }
    }
}