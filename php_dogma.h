/* php-dogma
 * Copyright (C) 2013, 2015 Romain "Artefact2" Dalmaso <artefact2@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PHP_DOGMA_H
#define PHP_DOGMA_H

extern zend_module_entry dogma_module_entry;
#define phpext_dogma_ptr &dogma_module_entry

#ifdef PHP_WIN32
#	define PHP_DOGMA_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_DOGMA_API __attribute__ ((visibility("default")))
#else
#	define PHP_DOGMA_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(dogma);
PHP_MSHUTDOWN_FUNCTION(dogma);
PHP_RINIT_FUNCTION(dogma);
PHP_RSHUTDOWN_FUNCTION(dogma);
PHP_MINFO_FUNCTION(dogma);

ZEND_FUNCTION(dogma_init_context);
ZEND_FUNCTION(dogma_free_context);
ZEND_FUNCTION(dogma_get_hashcode);

ZEND_FUNCTION(dogma_add_area_beacon);
ZEND_FUNCTION(dogma_remove_area_beacon);

ZEND_FUNCTION(dogma_add_implant);
ZEND_FUNCTION(dogma_remove_implant);

ZEND_FUNCTION(dogma_set_default_skill_level);
ZEND_FUNCTION(dogma_set_skill_level);
ZEND_FUNCTION(dogma_reset_skill_levels);

ZEND_FUNCTION(dogma_set_ship);

ZEND_FUNCTION(dogma_add_module);
ZEND_FUNCTION(dogma_add_module_s);
ZEND_FUNCTION(dogma_add_module_c);
ZEND_FUNCTION(dogma_add_module_sc);
ZEND_FUNCTION(dogma_remove_module);
ZEND_FUNCTION(dogma_set_module_state);

ZEND_FUNCTION(dogma_add_charge);
ZEND_FUNCTION(dogma_remove_charge);

ZEND_FUNCTION(dogma_add_drone);
ZEND_FUNCTION(dogma_remove_drone_partial);
ZEND_FUNCTION(dogma_remove_drone);

ZEND_FUNCTION(dogma_toggle_chance_based_effect);

ZEND_FUNCTION(dogma_target);
ZEND_FUNCTION(dogma_clear_target);

ZEND_FUNCTION(dogma_init_fleet_context);
ZEND_FUNCTION(dogma_free_fleet_context);

ZEND_FUNCTION(dogma_add_fleet_commander);
ZEND_FUNCTION(dogma_add_wing_commander);
ZEND_FUNCTION(dogma_add_squad_commander);
ZEND_FUNCTION(dogma_add_squad_member);

ZEND_FUNCTION(dogma_remove_fleet_member);

ZEND_FUNCTION(dogma_set_fleet_booster);
ZEND_FUNCTION(dogma_set_wing_booster);
ZEND_FUNCTION(dogma_set_squad_booster);

ZEND_FUNCTION(dogma_get_location_attribute);
ZEND_FUNCTION(dogma_get_area_beacon_attribute);
ZEND_FUNCTION(dogma_get_character_attribute);
ZEND_FUNCTION(dogma_get_implant_attribute);
ZEND_FUNCTION(dogma_get_skill_attribute);
ZEND_FUNCTION(dogma_get_ship_attribute);
ZEND_FUNCTION(dogma_get_module_attribute);
ZEND_FUNCTION(dogma_get_charge_attribute);
ZEND_FUNCTION(dogma_get_drone_attribute);

ZEND_FUNCTION(dogma_get_chance_based_effect_chance);

ZEND_FUNCTION(dogma_get_affectors);

ZEND_FUNCTION(dogma_type_has_effect);
ZEND_FUNCTION(dogma_type_has_active_effects);
ZEND_FUNCTION(dogma_type_has_overload_effects);
ZEND_FUNCTION(dogma_type_has_projectable_effects);
ZEND_FUNCTION(dogma_type_base_attribute);

ZEND_FUNCTION(dogma_get_number_of_module_cycles_before_reload);

ZEND_FUNCTION(dogma_get_capacitor_all);
ZEND_FUNCTION(dogma_get_capacitor);

ZEND_FUNCTION(dogma_get_nth_type_effect_with_attributes);
ZEND_FUNCTION(dogma_get_location_effect_attributes);

#ifdef ZTS
#define DOGMA_G(v) TSRMG(dogma_globals_id, zend_dogma_globals *, v)
#else
#define DOGMA_G(v) (dogma_globals.v)
#endif





#endif
