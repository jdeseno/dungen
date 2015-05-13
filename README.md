# DUNGEN

C dungeon generation library designed for easy static embedding.

## Installation

`make libdungen.a` builds a static library you can link against.

## API

The API exposes an opaque dungeon structure that represents a grid and cells.
Different generation functions can be applied to a dungeon to create customizable effects.

#### Dungeon

* `dg_create` create a dungeon to work with
* `dg_free` cleanup memory used by a dungeon

#### Working with cells

* `dg_set` set an individual cell
* `dg_get` get an individual cell
* `dg_each` iterate over all cells
* `dg_each_room` iterate over room rects

#### Dungeon generators

* `dg_worms` lots of random wormy passageways
* `dg_randomwalk` a random walk, great for generating connected caves
* `dg_chunky` many connected adjacent rooms

## Example

``` c
#include <stdlib.h>
#include "dungen.h"

int main()
{
    dg_dungeon d = dg_create(60, 40, NULL);
    dg_worms(d);
    dg_each(d, dg_print);
    dg_free(d);

    return 0;
}
```

## SDL Demo

Make will build an animated SDL demo by default.

![screenshot](https://raw.github.com/jdeseno/dungen/master/screenshot.png)
