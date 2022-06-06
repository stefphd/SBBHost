#include "plotter.hpp"
#include <iomanip>
#include <iostream>
#include <gdk/gdkevents.h>

using namespace CarioGraphConstants;

void CGraph::CairoGraph::on_button_press_event(int button, double x, double y)
{
    Gtk::Allocation allocation = get_allocation();
    auto scaled_x =  x / allocation.get_width();
    auto scaled_y =  y / allocation.get_height();

    if(button == 1)
    {
        if (scaled_x > offset_x && scaled_x < offset_x + graph_width &&
            scaled_y > offset_y && scaled_y < offset_y + graph_height)
        {
            start_x = scaled_x;
            start_y = scaled_y;
            end_x = start_x; // prevents drawing rogue selection box on first click
            end_y = start_y;

            // selection start point in zoomed frame
            double sx = plot.xmin + (start_x - offset_x) * (plot.xmax - plot.xmin) / graph_width;
            double sy = plot.ymax - (start_y - offset_y) * (plot.ymax - plot.ymin) / graph_height;

            // selection start point in unzoomed frame, we always zoom wrt unzoomed frame coordinates.
            plot.zoom_start_x = offset_x + (sx - xmin)  / (xmax - xmin) * graph_width;
            plot.zoom_start_y = offset_y + (ymax - sy)  / (ymax - ymin) * graph_height;
            selection_mode = true;
        }
    }
    
    return;
}

void CGraph::CairoGraph::reset_event(int button_press, double, double)
{
    if (button_press == 1)  
    {
        plot.zoom_factor_x = 1.0;
        plot.zoom_factor_y = 1.0;
        plot.scroll_counter = 1;
        draw_zoom = false;
    }

    queue_draw();
}

bool CGraph::CairoGraph::on_scroll_event(double /* dx */, double dy) {
    
    if (dy < 0) { //zoom in
        plot.zoom_start_x = plot.scroll_x-0.25/plot.scroll_counter;
        plot.zoom_end_x = plot.scroll_x+0.25/plot.scroll_counter;
        plot.zoom_start_y = plot.scroll_y-0.25/plot.scroll_counter;
        plot.zoom_end_y = plot.scroll_y+0.25/plot.scroll_counter;
        
        plot.zoom_start_x = (plot.zoom_start_x>=0) ? plot.zoom_start_x : 0;
        plot.zoom_start_y = (plot.zoom_start_y>=0) ? plot.zoom_start_y : 0;
        plot.zoom_end_x = (plot.zoom_end_x<=1) ? plot.zoom_end_x : 1;
        plot.zoom_end_y = (plot.zoom_end_y<=1) ? plot.zoom_end_y : 1;

        plot.zoom_factor_x = fabs(plot.zoom_end_x - plot.zoom_start_x) / graph_width;
        plot.zoom_factor_y = fabs(plot.zoom_end_y - plot.zoom_start_y) / graph_height; 
         

        // save guard the user double click and too small a region selected
        if (plot.zoom_factor_x < 0.00001 || plot.zoom_factor_y < 0.00001) { return false; }

        plot.scroll_counter++;

    } else { //zoom out
        plot.scroll_counter = (plot.scroll_counter>1) ? plot.scroll_counter-- : plot.scroll_counter = 1;
        //nothing
    }

    draw_zoom = true;
    queue_draw();
    return true;
}

void CGraph::CairoGraph::on_button_release_event(int button_press, double, double)
{
    if (button_press == 1)  
    {
        draw_zoom = true;
        
        // selection end point in zoomed frame
        double ex = plot.xmin + (end_x - offset_x) * (plot.xmax - plot.xmin) /  graph_width;
        double ey = plot.ymax - (end_y - offset_y) * (plot.ymax - plot.ymin) /  graph_height;
        
         // selection start point in unzoomed frame, we always zoom wrt unzoomed frame coordinates.
        plot.zoom_end_x = offset_x + (ex - xmin)  / (xmax - xmin) * graph_width;
        plot.zoom_end_y = offset_y + (ymax - ey)  / (ymax - ymin) * graph_height;

        plot.zoom_factor_x = fabs(plot.zoom_end_x - plot.zoom_start_x) / graph_width;
        plot.zoom_factor_y = fabs(plot.zoom_end_y - plot.zoom_start_y) / graph_height; 
        selection_mode = false;     

        // save guard the user double click and too small a region selected
        if (plot.zoom_factor_x < 0.00001 || plot.zoom_factor_y < 0.00001) return;
    }

    queue_draw();
}

void CGraph::CairoGraph::on_motion_notify_event(double current_width, double current_height)
{
    Gtk::Allocation allocation = get_allocation();
    auto scaled_x = current_width / allocation.get_width();
    auto scaled_y = current_height / allocation.get_height();
    plot.scroll_x = scaled_x;
    plot.scroll_y = scaled_y;

    if (scaled_x < offset_x || scaled_x > offset_x + graph_width ||
        scaled_y < offset_y || scaled_y > offset_y + graph_height)
    {
        xvalue->set_text("");
        yvalue->set_text("");
        set_cursor("default");
    }
    else
    {
        // Evaluate values from coordinates

        double x = plot.xmin + (scaled_x - offset_x) * (plot.xmax - plot.xmin) / graph_width;
        double y = plot.ymin + (graph_height + offset_y - scaled_y) * (plot.ymax - plot.ymin) / graph_height;
        xvalue->set_text(Glib::ustring::format(std::fixed, std::setprecision(precisionx), x));
        yvalue->set_text(Glib::ustring::format(y));
        set_cursor(cross_hair_cursor);
    }

    if (scaled_x > offset_x && scaled_x < offset_x + graph_width &&
        scaled_y > offset_y && scaled_y < offset_y + graph_height && true == selection_mode)
    {
        end_x = scaled_x;
        end_y = scaled_y;
        queue_draw();
    }

    return;
}