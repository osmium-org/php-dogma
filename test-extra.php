<?php

require __DIR__.'/test-common.php';

/* Perform a quick sanity check of functions in dogma-extra.h. Since
 * core functions were already check in test-core.php, no need to be
 * as rigorous here with core functions. */

dogma_init_context($ctx);
dogma_add_module($ctx, TYPE_125mmGatlingAutoCannonII, $key);
dogma_get_affectors($ctx, [ DOGMA_LOC_Module, "module_index" => $key ], $affectors);

dogma_set_ship($ctx, TYPE_Rifter);
dogma_get_affectors($ctx, [ DOGMA_LOC_Module, "module_index" => $key ], $affectorswithship);

/* The Rifter should add two modifiers: turret damage and turret tracking */
assert(count($affectorswithship) === count($affectors) + 2);




assert(error_get_last() === null);

echo "All extra tests passed. Fantastic!\n";
