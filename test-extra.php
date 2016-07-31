<?php

require __DIR__.'/test-common.php';

/* Perform a quick sanity check of functions in dogma-extra.h. Since
 * core functions were already check in test-core.php, no need to be
 * as rigorous here with core functions. */

dogma_init_context($ctx);
dogma_add_module($ctx, TYPE_125mmGatlingAutoCannonII, $key);
$mloc = [ DOGMA_LOC_Module, "module_index" => $key ];
dogma_get_affectors($ctx, $mloc, $affectors);

dogma_set_ship($ctx, TYPE_Rifter);
dogma_get_affectors($ctx, $mloc, $affectorswithship);
assert(isset($affectorswithship[0]['id']));
assert(isset($affectorswithship[0]['destid']));
assert(isset($affectorswithship[0]['value']));
assert(isset($affectorswithship[0]['operator']));
assert(isset($affectorswithship[0]['order']));
assert(isset($affectorswithship[0]['flags']));

/* The Rifter should add two modifiers: turret damage and turret tracking */
assert(count($affectorswithship) === count($affectors) + 2);

assert(dogma_type_has_effect(
	TYPE_125mmGatlingAutoCannonII, DOGMA_STATE_Online, EFFECT_HiPower, $hasit
) === DOGMA_OK);
assert($hasit === true);
assert(dogma_type_has_active_effects(TYPE_125mmGatlingAutoCannonII, $able) === DOGMA_OK);
assert($able === true);
assert(dogma_type_has_overload_effects(TYPE_125mmGatlingAutoCannonII, $able) === DOGMA_OK);
assert($able === true);
assert(dogma_type_has_projectable_effects(TYPE_StasisWebifierI, $projectable) === DOGMA_OK);
assert($projectable === true);
assert(dogma_type_base_attribute(TYPE_Rifter, ATT_LauncherSlotsLeft, $slots) === DOGMA_OK);
assert($slots === 2.0);


dogma_add_charge($ctx, $key, TYPE_BarrageS);
assert(dogma_get_number_of_module_cycles_before_reload($ctx, $key, $out) === DOGMA_OK);
assert($out === 200);

assert(dogma_get_nth_type_effect_with_attributes(TYPE_125mmGatlingAutoCannonII, 0, $effect) === DOGMA_OK);
assert(dogma_get_location_effect_attributes(
	$ctx, $mloc, $effect,
	$duration, $tracking, $discharge,
	$range, $falloff, $usagechance
) === DOGMA_OK);
assert($falloff === 11287.5);
assert($range === 1200.0);
assert($discharge === 0.0);


assert(dogma_get_capacitor($ctx, true, $delta, $stable, $p) === DOGMA_OK);
assert($delta < 0.0);
assert($stable === true);
assert($p === 100.0);

assert(dogma_get_capacitor_all($ctx, false, $caps) === DOGMA_OK);
assert(($hc = dogma_get_hashcode($ctx)) !== false);
assert(isset($caps[$hc]['capacity']));
assert(isset($caps[$hc]['delta']));
assert(isset($caps[$hc]['stable']));
assert(isset($caps[$hc]['stable_fraction']));



dogma_init_context($foo);
dogma_init_context($bar);
$baz = $foo;
$baz2 =& $bar;

assert(dogma_get_hashcode($foo) !== false);
assert(dogma_get_hashcode($bar) !== false);
assert(dogma_get_hashcode($foo) === dogma_get_hashcode($baz));
assert(dogma_get_hashcode($bar) === dogma_get_hashcode($baz2));
assert(dogma_get_hashcode($foo) !== dogma_get_hashcode($bar));



dogma_init_context($foo);
assert(dogma_get_capacitor_all($foo, false, $result) === DOGMA_OK);
assert(count($result) === 1);
assert(array_keys($result)[0] === dogma_get_hashcode($foo));



assert(error_get_last() === null);

echo "All extra tests passed. Fantastic!\n";
