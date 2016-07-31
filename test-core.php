<?php

require __DIR__.'/test-common.php';

/* NB: the goal of this test is NOT to test libdogma itself, just to
 * test that each wrapped function is working as intended.
 *
 * You may be thinking that I am comparing doubles for (strict)
 * equality and that I am crazy. Fortunately all these tests involve
 * integer values which can be accurately compared even when using
 * floating-point numbers. */

assert(dogma_init_context($ctx) === DOGMA_OK);
assert(gettype($ctx) === 'resource');
assert(get_resource_type($ctx) === 'Dogma context');



assert(dogma_get_location_attribute($ctx, DOGMA_LOC_Char, ATT_MaxActiveDrones, $out) === DOGMA_OK);
assert($out === 5.0);

assert(dogma_get_location_attribute($ctx, [ DOGMA_LOC_Char ], ATT_MaxActiveDrones, $out) === DOGMA_OK);
assert($out === 5.0);

assert(dogma_get_character_attribute($ctx, ATT_MaxActiveDrones, $out) === DOGMA_OK);
assert($out === 5.0);



assert(dogma_add_implant($ctx, TYPE_SnakeOmega, $key) === DOGMA_OK);

assert(dogma_get_location_attribute(
	$ctx, [ DOGMA_LOC_Implant, "implant_index" => $key ], ATT_Implantness, $out
) === DOGMA_OK);
assert($out === 6.0);

assert(dogma_get_implant_attribute($ctx, $key, ATT_Implantness, $out) === DOGMA_OK);
assert($out === 6.0);

assert(dogma_remove_implant($ctx, $key) === DOGMA_OK);
assert(dogma_remove_implant($ctx, $key) === DOGMA_NOT_FOUND);



assert(dogma_set_default_skill_level($ctx, 3) === DOGMA_OK);
assert(dogma_get_character_attribute($ctx, ATT_MaxActiveDrones, $out) === DOGMA_OK);
assert($out === 3.0);

assert(dogma_set_skill_level($ctx, TYPE_Drones, 0) === DOGMA_OK);
assert(dogma_get_character_attribute($ctx, ATT_MaxActiveDrones, $out) === DOGMA_OK);
assert($out === 0.0);

assert(dogma_get_skill_attribute($ctx, TYPE_Drones, ATT_SkillLevel, $out) === DOGMA_OK);
assert($out === 0.0);

assert(dogma_reset_skill_levels($ctx) === DOGMA_OK);
assert(dogma_get_character_attribute($ctx, ATT_MaxActiveDrones, $out) === DOGMA_OK);
assert($out === 3.0);

assert(dogma_get_location_attribute(
	$ctx, [ DOGMA_LOC_Skill, "skill_typeid" => TYPE_Drones ], ATT_MaxActiveDroneBonus, $out
) === DOGMA_OK);
assert($out === 3.0);

assert(dogma_set_default_skill_level($ctx, 5) === DOGMA_OK);
assert(dogma_get_character_attribute($ctx, ATT_MaxActiveDrones, $out) === DOGMA_OK);
assert($out === 5.0);



assert(dogma_set_ship($ctx, TYPE_Rifter) === DOGMA_OK);
assert(dogma_get_ship_attribute($ctx, ATT_MaxLockedTargets, $out) === DOGMA_OK);
assert($out === 4.0);

assert(dogma_set_ship($ctx, TYPE_Scimitar) === DOGMA_OK);
assert(dogma_get_location_attribute($ctx, DOGMA_LOC_Ship, ATT_MaxLockedTargets, $out) === DOGMA_OK);
assert($out === 10.0);



assert(dogma_add_module($ctx, TYPE_SmallAncillaryShieldBooster, $key) === DOGMA_OK);
assert(dogma_set_module_state($ctx, $key, DOGMA_STATE_Overloaded) === DOGMA_OK);
assert(dogma_get_module_attribute($ctx, $key, ATT_CapacitorNeed, $out) === DOGMA_OK);
assert($out > 0.0);
assert(dogma_remove_module($ctx, $key) === DOGMA_OK);

assert(dogma_add_module_s($ctx, TYPE_SmallAncillaryShieldBooster, $key, DOGMA_STATE_Active) === DOGMA_OK);
assert(dogma_remove_module($ctx, $key) === DOGMA_OK);

assert(dogma_add_module_c($ctx, TYPE_SmallAncillaryShieldBooster, $key, TYPE_CapBooster25) === DOGMA_OK);
assert(dogma_get_module_attribute($ctx, $key, ATT_CapacitorNeed, $out) === DOGMA_OK);
assert($out === 0.0);
assert(dogma_remove_module($ctx, $key) === DOGMA_OK);

assert(dogma_add_module_sc(
	$ctx, TYPE_SmallAncillaryShieldBooster, $key, DOGMA_STATE_Online, TYPE_CapBooster25
) === DOGMA_OK);
assert(dogma_get_module_attribute($ctx, $key, ATT_CapacitorNeed, $out) === DOGMA_OK);
assert($out === 0.0);
assert(dogma_remove_charge($ctx, $key) === DOGMA_OK);
assert(dogma_get_module_attribute($ctx, $key, ATT_CapacitorNeed, $out) === DOGMA_OK);
assert($out > 0.0);
assert(dogma_add_charge($ctx, $key, TYPE_CapBooster25) === DOGMA_OK);
assert(dogma_get_module_attribute($ctx, $key, ATT_CapacitorNeed, $out) === DOGMA_OK);
assert($out === 0.0);
assert(dogma_get_charge_attribute($ctx, $key, ATT_CapacitorBonus, $out) === DOGMA_OK);
assert($out === 25.0);
assert(dogma_remove_charge($ctx, $key) === DOGMA_OK);
assert(dogma_remove_charge($ctx, $key) === DOGMA_NOT_FOUND);
assert(dogma_remove_module($ctx, $key) === DOGMA_OK);

assert(dogma_remove_charge($ctx, $key) === DOGMA_NOT_FOUND);
assert(dogma_remove_module($ctx, $key) === DOGMA_NOT_FOUND);
assert(dogma_get_module_attribute($ctx, $key, ATT_CapacitorNeed, $out) === DOGMA_NOT_FOUND);



assert(dogma_add_drone($ctx, TYPE_WarriorI, 2) === DOGMA_OK);
assert(dogma_get_drone_attribute($ctx, TYPE_WarriorI, ATT_DroneBandwidthUsed, $out) === DOGMA_OK);
assert($out === 5.0);

assert(dogma_remove_drone_partial($ctx, TYPE_WarriorI, 1) === DOGMA_OK);
assert(dogma_get_location_attribute(
	$ctx, [ DOGMA_LOC_Drone, "drone_typeid" => TYPE_WarriorI ], ATT_DroneBandwidthUsed, $out
) === DOGMA_OK);
assert($out === 5.0);

assert(dogma_remove_drone($ctx, TYPE_WarriorI) === DOGMA_OK);
assert(dogma_get_drone_attribute($ctx, TYPE_WarriorI, ATT_DroneBandwidthUsed, $out) === DOGMA_NOT_FOUND);



assert(dogma_add_implant($ctx, TYPE_StrongBluePillBooster, $key) === DOGMA_OK);
assert(dogma_toggle_chance_based_effect(
	$ctx, [ DOGMA_LOC_Implant, "implant_index" => $key ], EFFECT_BoosterShieldCapacityPenalty, true
) === DOGMA_OK);
assert(dogma_get_chance_based_effect_chance(
	$ctx, [ DOGMA_LOC_Implant, "implant_index" => $key ], EFFECT_BoosterShieldCapacityPenalty, $out
) === DOGMA_OK);
assert($out === 0.3);
assert(dogma_remove_implant($ctx, $key) === DOGMA_OK);
assert(dogma_toggle_chance_based_effect(
	$ctx, [ DOGMA_LOC_Implant, "implant_index" => $key ], EFFECT_BoosterShieldCapacityPenalty, false
) === DOGMA_NOT_FOUND);



assert(dogma_add_module($ctx, TYPE_StasisWebifierI, $key) === DOGMA_OK);
assert(dogma_target($ctx, [ DOGMA_LOC_Module, "module_index" => $key ], $ctx) === DOGMA_OK);
assert(dogma_clear_target($ctx, [ DOGMA_LOC_Module, "module_index" => $key ]) === DOGMA_OK);
assert(dogma_remove_module($ctx, $key) === DOGMA_OK);



assert(dogma_init_fleet_context($fctx) === DOGMA_OK);
assert(gettype($fctx) === 'resource');
assert(get_resource_type($fctx) === 'Dogma fleet context');

assert(dogma_add_fleet_commander($fctx, $ctx) === DOGMA_OK);
assert(dogma_add_wing_commander($fctx, 0, $ctx) === DOGMA_OK);
assert(dogma_add_squad_commander($fctx, 0, 0, $ctx) === DOGMA_OK);
assert(dogma_add_squad_member($fctx, 0, 0, $ctx) === DOGMA_OK);

assert(dogma_remove_fleet_member($fctx, $ctx, $found) === DOGMA_OK);
assert($found === true);
assert(dogma_remove_fleet_member($fctx, $ctx, $found) === DOGMA_OK);
assert($found === false);
assert(dogma_add_squad_member($fctx, 0, 0, $ctx) === DOGMA_OK);

assert(dogma_set_fleet_booster($fctx, $ctx) === DOGMA_OK);
assert(dogma_set_fleet_booster($fctx, null) === DOGMA_OK);
assert(dogma_set_wing_booster($fctx, 0, $ctx) === DOGMA_OK);
assert(dogma_set_wing_booster($fctx, 0, null) === DOGMA_OK);
assert(dogma_set_squad_booster($fctx, 0, 0, $ctx) === DOGMA_OK);
assert(dogma_set_squad_booster($fctx, 0, 0, null) === DOGMA_OK);

assert(dogma_free_context($ctx) === DOGMA_OK);
assert(dogma_free_fleet_context($fctx) === DOGMA_OK);



assert(dogma_init_context($ctx) === DOGMA_OK);
assert(dogma_init_fleet_context($fctx) === DOGMA_OK);
assert(gettype($ctx) === 'resource');
assert(gettype($fctx) === 'resource');
assert(get_resource_type($ctx) === 'Dogma context');
assert(get_resource_type($fctx) === 'Dogma fleet context');
assert(dogma_add_squad_member($fctx, 0, 0, $ctx) === DOGMA_OK);

assert(dogma_free_fleet_context($fctx) === DOGMA_OK);
assert(dogma_free_context($ctx) === DOGMA_OK); /* Note the reversed order */



/* Test garbage collection */

/* Unfortunately, memory_get_usage() is useless here since libdogma
 * does not use emalloc()/efree() internally, so a non-portable hack
 * is used */
function get_mem_usage() {
	return 1024 * intval(trim(substr(trim(
		shell_exec("pmap ".getmypid()." | grep total")
	), 5, -1)));
}

dogma_init_context($ctx);
$oldusage = get_mem_usage();

for($i = 0; $i < 1000; ++$i) {
	dogma_init_context($ctx);
}

$newusage = get_mem_usage();
assert($newusage - $oldusage < (1 << 20));



dogma_init_fleet_context($fctx);
$oldusage = get_mem_usage();

for($i = 0; $i < 10000; ++$i) {
	dogma_init_fleet_context($fctx);
}

$newusage = get_mem_usage();
assert($newusage - $oldusage < (1 << 10));



$oldusage = get_mem_usage();

$arr = array();
for($i = 0; $i < 10000; ++$i) {
	dogma_get_affectors($ctx, DOGMA_LOC_Char, $arr);
}
unset($arr);

$newusage = get_mem_usage();
assert($newusage - $oldusage < (1 << 10));



assert(error_get_last() === null);

echo "All core tests passed. Excellent job!\n";
