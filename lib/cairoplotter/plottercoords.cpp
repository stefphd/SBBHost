#include "plotter.hpp"

using namespace CarioGraphConstants;

double CGraph::CairoGraph::x_to_graph_coords(const double x) const
{
     return(offset_x + ((x - xmin) * graph_width) / (xmax - xmin));
}

double CGraph::CairoGraph::y_to_graph_coords(const double y) const
{
    return(offset_y + graph_height - ((y - ymin) * graph_height) / (ymax - ymin));
}
