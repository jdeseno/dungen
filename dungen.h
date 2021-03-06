/*
Copyright (c) 2015 Josh DeSeno <jdeseno [at] gmail.com>
 
Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.
 
THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef __DUNGEN_H__
#define __DUNGEN_H__

#define DUNGEN_VERSION_MAJOR 0
#define DUNGEN_VERSION_MINOR 0
#define DUNGEN_VERSION_PATCH 3
#define DG_ALLOC malloc
#define DG_FREE free

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Types
 */

/** The kind of a cell, where a cell is an x, y pair inside the dungeon */
enum dg_cell_kind {
    dg_cell_stone = 0,
    dg_cell_wall,
    dg_cell_floor
};

struct dgx_dungeon;

/** The dungeon structure this is a grid of cells */
typedef struct dgx_dungeon * dg_dungeon;



/**
 * Iterators and callbacks
 */

/** Animation callback called when a cell is written */
typedef void (*dg_render_step)(dg_dungeon d, int step);

/** Each cell iterator callback */
typedef void (*dg_each_cell)(dg_dungeon d, int x, int y, enum dg_cell_kind k);

/** Rect iterator callback, where a rect is an x, y, w, h, usually represents a room */
typedef void (*dg_each_rect)(dg_dungeon d, int x, int y, int w, int h);

/** Each neighbor iterator callback, a neighbor is a cell next this one, staying within the grid */
typedef void (*dg_each_neighbor_cb)(dg_dungeon d, int x, int y, enum dg_cell_kind k, void *persist);



/**
 * Basic dungeon manipulation
 */

/** Create a dungeon of w * h cells, an animation callback is optional */
dg_dungeon dg_create(int width, int height, dg_render_step step_fn);

/** Reset all information in a dungeon (making it reusable) */
void dg_reset(dg_dungeon d);

/** Free memory used by a dungeon */
void dg_free(dg_dungeon d);


/**
 * Utilities
 */

/** Display an error message */
void dg_panic(const char *msg);

/** Set an individual cell kind */
void dg_set(dg_dungeon d, int x, int y, enum dg_cell_kind kind);

/** Get an individual cell kind */
enum dg_cell_kind dg_get(dg_dungeon d, int x, int y);

/** An ASCII printer for easier debugging can be used with dg_each iterator */
void dg_print(dg_dungeon d, int x, int y, enum dg_cell_kind kind);



/**
 * Iterators
 */

/** Iterate over every cell */
void dg_each(dg_dungeon d, dg_each_cell fn);

/** Iterate over every room */
void dg_each_room(dg_dungeon d, dg_each_rect fn);

/** Iterate over every neighbor of a cell
 *
 * A persist can be provided to maintain state
 */
void dg_each_neighbor(dg_dungeon d, int x, int y, void *persist, dg_each_neighbor_cb fn);


/**
 * Generators
 *
 * The dg_dungeon structure has state such as cell and room information.
 * Some generators reset this state or use existing state to create new state.
 */

/** Unevenly binary partition dungeon into many rooms
 *
 * resets any existing state
 */
void dg_rooms_split(dg_dungeon d);

/** Fill with tile
 *
 * ignores any existing state
 */
void dg_fill(dg_dungeon d, enum dg_cell_kind fill);

/** Fill the bordering tiles of a rectangle
 *
 * ignores any existing state
 */
void dg_fill_border(dg_dungeon d, int x, int y, int w, int h, enum dg_cell_kind fill);

/** Blur existing cells
 *
 * affected by existing state:
 * nearby cells of the same type are likely to combine
 */
void dg_blur(dg_dungeon d);


/** Fill rooms with no connected walls
  *
  * dependent on existing state:
  * rooms must exist to be filled
  */
void dg_fill_rooms(dg_dungeon d);

/** Carve an open maze-like pattern
 *
 * ignores any existing state
 */
void dg_forest(dg_dungeon d);

/** Game of life
 *
 * Apply the rules for a single turn of the game of life.
 * dg_cell_floor is an alive cell and other are dead.
 *
 * dependent on existing state:
 * cells of dg_cell_floor should exist first
 */
void dg_life(dg_dungeon d);

/** Carve a maze
 *
 * affected by existing state:
 * doesn't carve through cells of dg_cell_floor
 */
void dg_maze(dg_dungeon d);

/** Carve a maze composed of random diagonal passages
 *
 * ignores existing state
 */
void dg_maze_diagonal(dg_dungeon d);

/** Random cell noise
 *
 * ignores existing state
 */
void dg_noise(dg_dungeon d);

/** Carve floor cells in an open random walk
  *
  * ignores existing state
  */
void dg_randomwalk(dg_dungeon d);

/** Replace all cells of kind a with kind b
 *
 * affected by existing state:
 * ineffective if no cells of type a exist
 */
void dg_replace_all(dg_dungeon d, enum dg_cell_kind a, enum dg_cell_kind b);

/** Shrink rooms by 1 cell
 *
 * dependent on existing state:
 * some rooms must exist
 */
void dg_shrink(dg_dungeon d);

/** Smooth dungeon features
 *
 * affected by existing state:
 * dungeon must have some different types of cells to be smoothed
 */
void dg_smooth(dg_dungeon d);

/** Carve voronoi cells of
 *
 * ignores existing state
 */
void dg_voronoi(dg_dungeon d);

/** Carve random wormy passageways
 *
 * ignores existing state
 */
void dg_worms(dg_dungeon d);

#ifdef __cplusplus
}
#endif

#endif

