/**
 * @file svg.c
 * @brief Implementation of simple SVG drawing interface.
 *
 * Implements the basic functions for creating SVG documents.
 */
#include "svg.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>


/**
 * @brief Opaque SVG drawing context.
 *
 * Holds the necessary data to implement functions.
 */
struct SVG_CONTEXT {

    svg_write_fn write_fn;
    svg_cleanup_fn cleanup_fn;
    svg_user_context_ptr user;

};

/**
 * @brief Creates a new SVG drawing context.
 *
 * Initializes an SVG context with specified canvas dimensions and sets up
 * user-defined callbacks for writing and cleaning up output.
 *
 * @param write_fn   Callback function used to write SVG text
 * @param cleanup_fn Callback function used to clean up user resources
 * @param user       User-defined context passed to callbacks
 * @param width      Width of the SVG canvas in pixels
 * @param height     Height of the SVG canvas in pixels
 *
 * @return Pointer to a new SVG context, or NULL if creation fails
 */
svg_context_ptr svg_create(svg_write_fn write_fn, svg_cleanup_fn cleanup_fn, svg_user_context_ptr user, svg_px_t width, svg_px_t height) {

     if (!write_fn || !cleanup_fn || !user || width <= 0 || height <= 0) {
        return NULL;
    }
    svg_context_ptr context = (svg_context_ptr)malloc(sizeof(svg_context_t));
    if (!context) {
        return NULL;
    }
    context->write_fn = write_fn;
    context->cleanup_fn = cleanup_fn;
    context->user = user;
    char buffer[256];
    int n = snprintf(buffer, sizeof(buffer), "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" "<svg width=\"%d\" height=\"%d\" xmlns=\"http://www.w3.org/2000/svg\">\n", width, height);
    if (n < 0 || n >= (int)sizeof(buffer)) {
        free(context);
        return NULL;
    }
    if (write_fn(user, buffer) != SVG_OK) {
        free(context);
        return NULL;
    }
    return context; 

}

/**
 * @brief Destroys an SVG drawing context.
 *
 * Writes the closing </svg> tag, invokes the cleanup callback, and frees
 * the memory associated with the context.
 *
 * @param context Pointer to the SVG context to destroy
 *
 * @return SVG_OK on success, SVG_ERR_NULL if context is NULL,
 *         or SVG_ERR_IO if writing fails
 */
svg_return_t svg_destroy(svg_context_ptr context){
   if (!context) {
        return SVG_ERR_NULL;
    }
    const char* end_tag = "</svg>\n";
    svg_return_t ret = context->write_fn(context->user, end_tag);
    if (ret == SVG_OK) {
        ret = context->cleanup_fn(context->user);
    }
    free(context);
    return ret; 
}

/**
 * @brief Draws a circle element in the SVG.
 *
 * @param context Pointer to the SVG context
 * @param center  Center coordinates of the circle
 * @param radius  Radius of the circle
 * @param style   Optional CSS style string (can be NULL)
 *
 * @return SVG_OK on success, SVG_ERR_NULL if context or center is NULL,
 *         or SVG_ERR_IO if writing fails
 */
svg_return_t svg_circle(svg_context_ptr context, const svg_point_t *center, svg_real_t radius, const char *style){

    if (!context || !center) {
        return SVG_ERR_NULL;
    }
    char buffer[256];
    const char* s = style ? style : "";
    int n = snprintf(buffer, sizeof(buffer), "<circle cx=\"%f\" cy=\"%f\" r=\"%f\" style=\"%s\"/>\n", center->x, center->y, radius, s);
    if (n < 0 || n >= (int)sizeof(buffer)) {
        return SVG_ERR_IO;
    };
    return context->write_fn(context->user, buffer); 
}


/**
 * @brief Draws a rectangle element in the SVG.
 *
 * @param context  Pointer to the SVG context
 * @param top_left Top-left corner of the rectangle
 * @param size     Dimensions of the rectangle (width and height)
 * @param style    Optional CSS style string (can be NULL)
 *
 * @return SVG_OK on success, SVG_ERR_NULL if context, top_left, or size is NULL,
 *         or SVG_ERR_IO if writing fails
 */
svg_return_t svg_rect(svg_context_ptr context, const svg_point_t *top_left, const svg_size_t *size, const char* style){

    if (!context || !top_left || !size) {
        return SVG_ERR_NULL;
    }
    char buffer[256];
    const char* s = style ? style : "";
    int n = snprintf(buffer, sizeof(buffer), "<rect x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" style=\"%s\"/>\n", top_left->x, top_left->y, size->width, size->height, s);
    if (n < 0 || n >= (int)sizeof(buffer)) {
        return SVG_ERR_IO;
    }
    return context->write_fn(context->user, buffer); 
}

/**
 * @brief Draws a line segment in the SVG.
 *
 * @param context Pointer to the SVG context
 * @param start   Starting point of the line
 * @param end     Ending point of the line
 * @param style   Optional CSS style string (can be NULL)
 *
 * @return SVG_OK on success, SVG_ERR_NULL if context, start, or end is NULL,
 *         or SVG_ERR_IO if writing fails
 */
svg_return_t svg_line(svg_context_ptr context, const svg_point_t *start, const svg_point_t *end, const char* style){
    if (!context || !start || !end) {
        return SVG_ERR_NULL;
    }
    char buffer[256];
    const char* s = style ? style : "";
    int n = snprintf(buffer, sizeof(buffer), "<line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" style=\"%s\"/>\n", start->x, start->y, end->x, end->y, s);
    if (n < 0 || n >= (int)sizeof(buffer)) {
        return SVG_ERR_IO;
    }
    return context->write_fn(context->user, buffer); 
}

/**
 * @brief Begins an SVG group element.
 *
 * Writes an opening <g> tag with optional attributes.
 * Subsequent elements will belong to this group until svg_group_end() is called.
 *
 * @param context Pointer to the SVG context
 * @param attrs   Optional group attributes (can be NULL)
 *
 * @return SVG_OK on success, SVG_ERR_NULL if context is NULL,
 *         or SVG_ERR_IO if writing fails
 */
svg_return_t svg_group_begin(svg_context_ptr context, const char* attrs){

    if (!context) {
        return SVG_ERR_NULL;
    }
    char buffer[256];
    const char* s = attrs ? attrs : "";
    int n = snprintf(buffer, sizeof(buffer), "<g %s>\n", s);
    if (n < 0 || n >= (int)sizeof(buffer)) {
        return SVG_ERR_IO;
    }
    return context->write_fn(context->user, buffer); 
}

/**
 * @brief Ends the current SVG group element.
 *
 * Writes a closing </g> tag for the current group.
 *
 * @param context Pointer to the SVG context
 *
 * @return SVG_OK on success, SVG_ERR_NULL if context is NULL,
 *         or SVG_ERR_IO if writing fails
 */
svg_return_t svg_group_end(svg_context_ptr context){
    if (!context) {
        return SVG_ERR_NULL;
    }
    const char* buffer = "</g>\n";
    return context->write_fn(context->user, buffer); 
} 
