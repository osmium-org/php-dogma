/* php-dogma
 * Copyright (C) 2013 Romain "Artefact2" Dalmaso <artefact2@gmail.com>
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_dogma.h"
#include <dogma.h>
#include <dogma-extra.h>


#define DEF_DOGMA_FE(x) ZEND_FE(dogma_ ## x, arginfo_ ## x)
#define DEF_DOGMA_CONSTANT(x) REGISTER_LONG_CONSTANT(#x, x, CONST_CS | CONST_PERSISTENT | CONST_CT_SUBST)

#define GET_CTX(zctx, ctx) ZEND_FETCH_RESOURCE((ctx), dogma_context_t*, &(zctx), -1, \
                                               "Dogma context", le_dogma_context)
#define GET_CTX_NULLABLE(zctx, ctx) do {	  \
		if(Z_TYPE_P(zctx) == IS_RESOURCE) { \
			GET_CTX(zctx, ctx); \
		} else if(Z_TYPE_P(zctx) == IS_NULL) { \
			ctx = NULL; \
		} else { \
			RETURN_FALSE; \
		} \
	} while(0)
#define GET_FCTX(zfctx, fctx) ZEND_FETCH_RESOURCE((fctx), dogma_fleet_context_t*, &(zfctx), -1, \
                                                  "Dogma fleet context", le_dogma_fleet_context)

static int le_dogma_context;
static int le_dogma_fleet_context;



ZEND_BEGIN_ARG_INFO(arginfo_init_context, 0)
ZEND_ARG_INFO(1, context)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_free_context, 0)
ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO()



ZEND_BEGIN_ARG_INFO(arginfo_add_implant, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, typeid)
ZEND_ARG_INFO(1, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_remove_implant, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()



ZEND_BEGIN_ARG_INFO(arginfo_set_default_skill_level, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, default_level)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_set_skill_level, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, typeid)
ZEND_ARG_INFO(0, level)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_reset_skill_levels, 0)
ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO()



ZEND_BEGIN_ARG_INFO(arginfo_set_ship, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, typeid)
ZEND_END_ARG_INFO()



ZEND_BEGIN_ARG_INFO(arginfo_add_module, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, typeid)
ZEND_ARG_INFO(1, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_add_module_s, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, typeid)
ZEND_ARG_INFO(1, key)
ZEND_ARG_INFO(0, state)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_add_module_c, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, typeid)
ZEND_ARG_INFO(1, key)
ZEND_ARG_INFO(0, charge_typeid)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_add_module_sc, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, typeid)
ZEND_ARG_INFO(1, key)
ZEND_ARG_INFO(0, state)
ZEND_ARG_INFO(0, charge_typeid)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_remove_module, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_set_module_state, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, key)
ZEND_ARG_INFO(0, state)
ZEND_END_ARG_INFO()



ZEND_BEGIN_ARG_INFO(arginfo_add_charge, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, key)
ZEND_ARG_INFO(0, typeid)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_remove_charge, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()



ZEND_BEGIN_ARG_INFO(arginfo_add_drone, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, typeid)
ZEND_ARG_INFO(0, quantity)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_remove_drone_partial, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, typeid)
ZEND_ARG_INFO(0, quantity)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_remove_drone, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, typeid)
ZEND_END_ARG_INFO()



ZEND_BEGIN_ARG_INFO(arginfo_toggle_chance_based_effect, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, location)
ZEND_ARG_INFO(0, effectid)
ZEND_ARG_INFO(0, toggle)
ZEND_END_ARG_INFO()



ZEND_BEGIN_ARG_INFO(arginfo_target, 0)
ZEND_ARG_INFO(0, targeter_context)
ZEND_ARG_INFO(0, location)
ZEND_ARG_INFO(0, targetee_context)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_clear_target, 0)
ZEND_ARG_INFO(0, targeter_context)
ZEND_ARG_INFO(0, location)
ZEND_END_ARG_INFO()



ZEND_BEGIN_ARG_INFO(arginfo_init_fleet_context, 0)
ZEND_ARG_INFO(1, fleet_context)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_free_fleet_context, 0)
ZEND_ARG_INFO(0, fleet_context)
ZEND_END_ARG_INFO()



ZEND_BEGIN_ARG_INFO(arginfo_add_fleet_commander, 0)
ZEND_ARG_INFO(0, fleet_context)
ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_add_wing_commander, 0)
ZEND_ARG_INFO(0, fleet_context)
ZEND_ARG_INFO(0, wing)
ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_add_squad_commander, 0)
ZEND_ARG_INFO(0, fleet_context)
ZEND_ARG_INFO(0, wing)
ZEND_ARG_INFO(0, squad)
ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_add_squad_member, 0)
ZEND_ARG_INFO(0, fleet_context)
ZEND_ARG_INFO(0, wing)
ZEND_ARG_INFO(0, squad)
ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO()



ZEND_BEGIN_ARG_INFO(arginfo_remove_fleet_member, 0)
ZEND_ARG_INFO(0, fleet_context)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(1, found)
ZEND_END_ARG_INFO()



ZEND_BEGIN_ARG_INFO(arginfo_set_fleet_booster, 0)
ZEND_ARG_INFO(0, fleet_context)
ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_set_wing_booster, 0)
ZEND_ARG_INFO(0, fleet_context)
ZEND_ARG_INFO(0, wing)
ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_set_squad_booster, 0)
ZEND_ARG_INFO(0, fleet_context)
ZEND_ARG_INFO(0, wing)
ZEND_ARG_INFO(0, squad)
ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO()



ZEND_BEGIN_ARG_INFO(arginfo_get_location_attribute, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, location)
ZEND_ARG_INFO(0, attributeid)
ZEND_ARG_INFO(1, out)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_get_character_attribute, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, attributeid)
ZEND_ARG_INFO(1, out)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_get_implant_attribute, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, key)
ZEND_ARG_INFO(0, attributeid)
ZEND_ARG_INFO(1, out)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_get_skill_attribute, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, typeid)
ZEND_ARG_INFO(0, attributeid)
ZEND_ARG_INFO(1, out)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_get_ship_attribute, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, attributeid)
ZEND_ARG_INFO(1, out)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_get_module_attribute, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, key)
ZEND_ARG_INFO(0, attributeid)
ZEND_ARG_INFO(1, out)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_get_charge_attribute, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, key)
ZEND_ARG_INFO(0, attributeid)
ZEND_ARG_INFO(1, out)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_get_drone_attribute, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, typeid)
ZEND_ARG_INFO(0, attributeid)
ZEND_ARG_INFO(1, out)
ZEND_END_ARG_INFO()



ZEND_BEGIN_ARG_INFO(arginfo_get_chance_based_effect_chance, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, location)
ZEND_ARG_INFO(0, effectid)
ZEND_ARG_INFO(1, out)
ZEND_END_ARG_INFO()



ZEND_BEGIN_ARG_INFO(arginfo_get_affectors, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, location)
ZEND_ARG_INFO(1, out_array)
ZEND_END_ARG_INFO()



ZEND_BEGIN_ARG_INFO(arginfo_type_has_effect, 0)
ZEND_ARG_INFO(0, typeid)
ZEND_ARG_INFO(0, state)
ZEND_ARG_INFO(0, effectid)
ZEND_ARG_INFO(1, has_it)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_type_has_active_effects, 0)
ZEND_ARG_INFO(0, typeid)
ZEND_ARG_INFO(1, activable)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_type_has_overload_effects, 0)
ZEND_ARG_INFO(0, typeid)
ZEND_ARG_INFO(1, overloadable)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_type_base_attribute, 0)
ZEND_ARG_INFO(0, typeid)
ZEND_ARG_INFO(0, attributeid)
ZEND_ARG_INFO(1, base_value)
ZEND_END_ARG_INFO()



ZEND_BEGIN_ARG_INFO(arginfo_get_number_of_module_cycles_before_reload, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, key)
ZEND_ARG_INFO(1, num_cycles)
ZEND_END_ARG_INFO()



ZEND_BEGIN_ARG_INFO(arginfo_get_capacitor, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, include_reload_time)
ZEND_ARG_INFO(1, delta)
ZEND_ARG_INFO(1, stable)
ZEND_ARG_INFO(1, param)
ZEND_END_ARG_INFO()



ZEND_BEGIN_ARG_INFO(arginfo_get_nth_type_effect_with_attributes, 0)
ZEND_ARG_INFO(0, typeid)
ZEND_ARG_INFO(0, n)
ZEND_ARG_INFO(1, out_effectid)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_get_location_effect_attributes, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, location)
ZEND_ARG_INFO(0, effectid)
ZEND_ARG_INFO(1, duration)
ZEND_ARG_INFO(1, trackingspeed)
ZEND_ARG_INFO(1, discharge)
ZEND_ARG_INFO(1, range)
ZEND_ARG_INFO(1, falloff)
ZEND_ARG_INFO(1, fittingusagechance)
ZEND_END_ARG_INFO()



const zend_function_entry dogma_functions[] = {
	/* Core - dogma.h */

	DEF_DOGMA_FE(init_context)
	DEF_DOGMA_FE(free_context)

	DEF_DOGMA_FE(add_implant)
	DEF_DOGMA_FE(remove_implant)

	DEF_DOGMA_FE(set_default_skill_level)
	DEF_DOGMA_FE(set_skill_level)
	DEF_DOGMA_FE(reset_skill_levels)

	DEF_DOGMA_FE(set_ship)

	DEF_DOGMA_FE(add_module)
	DEF_DOGMA_FE(add_module_s)
	DEF_DOGMA_FE(add_module_c)
	DEF_DOGMA_FE(add_module_sc)
	DEF_DOGMA_FE(remove_module)
	DEF_DOGMA_FE(set_module_state)

	DEF_DOGMA_FE(add_charge)
	DEF_DOGMA_FE(remove_charge)

	DEF_DOGMA_FE(add_drone)
	DEF_DOGMA_FE(remove_drone_partial)
	DEF_DOGMA_FE(remove_drone)

	DEF_DOGMA_FE(toggle_chance_based_effect)

	DEF_DOGMA_FE(target)
	DEF_DOGMA_FE(clear_target)

	DEF_DOGMA_FE(init_fleet_context)
	DEF_DOGMA_FE(free_fleet_context)

	DEF_DOGMA_FE(add_fleet_commander)
	DEF_DOGMA_FE(add_wing_commander)
	DEF_DOGMA_FE(add_squad_commander)
	DEF_DOGMA_FE(add_squad_member)

	DEF_DOGMA_FE(remove_fleet_member)

	DEF_DOGMA_FE(set_fleet_booster)
	DEF_DOGMA_FE(set_wing_booster)
	DEF_DOGMA_FE(set_squad_booster)

	DEF_DOGMA_FE(get_location_attribute)
	DEF_DOGMA_FE(get_character_attribute)
	DEF_DOGMA_FE(get_implant_attribute)
	DEF_DOGMA_FE(get_skill_attribute)
	DEF_DOGMA_FE(get_ship_attribute)
	DEF_DOGMA_FE(get_module_attribute)
	DEF_DOGMA_FE(get_charge_attribute)
	DEF_DOGMA_FE(get_drone_attribute)

	DEF_DOGMA_FE(get_chance_based_effect_chance)

	DEF_DOGMA_FE(type_has_effect)
	DEF_DOGMA_FE(type_has_active_effects)
	DEF_DOGMA_FE(type_has_overload_effects)
	DEF_DOGMA_FE(type_base_attribute)

	DEF_DOGMA_FE(get_number_of_module_cycles_before_reload)

	DEF_DOGMA_FE(get_capacitor)

	DEF_DOGMA_FE(get_nth_type_effect_with_attributes)
	DEF_DOGMA_FE(get_location_effect_attributes)

	/* Extra - dogma-extra.h */

	DEF_DOGMA_FE(get_affectors)

	{NULL, NULL, NULL}
};

zend_module_entry dogma_module_entry = {
	STANDARD_MODULE_HEADER,
	"dogma",
	dogma_functions,
	PHP_MINIT(dogma),
	PHP_MSHUTDOWN(dogma),
	PHP_RINIT(dogma),
	PHP_RSHUTDOWN(dogma),
	PHP_MINFO(dogma),
	"0.1.0",
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_DOGMA
ZEND_GET_MODULE(dogma)
#endif



static void dogma_context_dtor(zend_rsrc_list_entry* rsrc TSRMLS_DC) {
	dogma_context_t* ctx = rsrc->ptr;
	dogma_free_context(ctx); /* XXX: assuming this always returns DOGMA_OK (it does) */
}

static void dogma_fleet_context_dtor(zend_rsrc_list_entry* rsrc TSRMLS_DC) {
	dogma_fleet_context_t* fctx = rsrc->ptr;
	dogma_free_fleet_context(fctx); /* XXX: see above */
}

static inline int dogma_get_location_from_zval(zval* zloc, dogma_location_t* loc) {
	int success = 1;

	if(Z_TYPE_P(zloc) == IS_ARRAY) {
		zval** val;

		if(zend_hash_index_find(Z_ARRVAL_P(zloc), 0, (void**)&val) == SUCCESS) {
			if(Z_TYPE_PP(val) == IS_LONG) {
				loc->type = (dogma_location_type_t)Z_LVAL_PP(val);
			} else {
				zend_error(E_WARNING, "location type is not an integer");
				success = 0;
			}
		} else {
			zend_error(E_WARNING, "location argument given as an array without a location type");
			success = 0;
		}

		if(zend_hash_find(Z_ARRVAL_P(zloc), "implant_index",
		                  sizeof("implant_index"), (void**)&val) == SUCCESS) {
			if(Z_TYPE_PP(val) == IS_LONG) {
				loc->implant_index = (dogma_key_t)Z_LVAL_PP(val);
			} else {
				zend_error(E_WARNING, "implant_index is not an integer");
				success = 0;
			}
		}

		if(zend_hash_find(Z_ARRVAL_P(zloc), "module_index",
		                  sizeof("module_index"), (void**)&val) == SUCCESS) {
			if(Z_TYPE_PP(val) == IS_LONG) {
				loc->module_index = (dogma_key_t)Z_LVAL_PP(val);
			} else {
				zend_error(E_WARNING, "module_index is not an integer");
				success = 0;
			}
		}

		if(zend_hash_find(Z_ARRVAL_P(zloc), "skill_typeid",
		                  sizeof("skill_typeid"), (void**)&val) == SUCCESS) {
			if(Z_TYPE_PP(val) == IS_LONG) {
				loc->skill_typeid = (dogma_key_t)Z_LVAL_PP(val);
			} else {
				zend_error(E_WARNING, "skill_typeid is not an integer");
				success = 0;
			}
		}

		if(zend_hash_find(Z_ARRVAL_P(zloc), "drone_typeid",
		                  sizeof("drone_typeid"), (void**)&val) == SUCCESS) {
			if(Z_TYPE_PP(val) == IS_LONG) {
				loc->drone_typeid = (dogma_key_t)Z_LVAL_PP(val);
			} else {
				zend_error(E_WARNING, "drone_typeid is not an integer");
				success = 0;
			}
		}
	} else if(Z_TYPE_P(zloc) == IS_LONG) {
		loc->type = (dogma_location_type_t)Z_LVAL_P(zloc);
	} else {
		zend_error(E_WARNING, "location argument is neither an integer nor an array");
		success = 0;
	}

	return success;
}

PHP_MINIT_FUNCTION(dogma) {
	dogma_init();
	le_dogma_context = zend_register_list_destructors_ex(
		dogma_context_dtor, NULL,
		"Dogma context", module_number
	);
	le_dogma_fleet_context = zend_register_list_destructors_ex(
		dogma_fleet_context_dtor, NULL,
		"Dogma fleet context", module_number
	);

	DEF_DOGMA_CONSTANT(DOGMA_OK);
	DEF_DOGMA_CONSTANT(DOGMA_NOT_FOUND);
	DEF_DOGMA_CONSTANT(DOGMA_NOT_APPLICABLE);

	DEF_DOGMA_CONSTANT(DOGMA_LOC_Char);
	DEF_DOGMA_CONSTANT(DOGMA_LOC_Implant);
	DEF_DOGMA_CONSTANT(DOGMA_LOC_Skill);
	DEF_DOGMA_CONSTANT(DOGMA_LOC_Ship);
	DEF_DOGMA_CONSTANT(DOGMA_LOC_Module);
	DEF_DOGMA_CONSTANT(DOGMA_LOC_Charge);
	DEF_DOGMA_CONSTANT(DOGMA_LOC_Drone);

	DEF_DOGMA_CONSTANT(DOGMA_STATE_Unplugged);
	DEF_DOGMA_CONSTANT(DOGMA_STATE_Offline);
	DEF_DOGMA_CONSTANT(DOGMA_STATE_Online);
	DEF_DOGMA_CONSTANT(DOGMA_STATE_Active);
	DEF_DOGMA_CONSTANT(DOGMA_STATE_Overloaded);

	DEF_DOGMA_CONSTANT(DOGMA_AFFECTOR_PENALIZED);
	DEF_DOGMA_CONSTANT(DOGMA_AFFECTOR_SINGLETON);

	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(dogma) {
	return SUCCESS;
}

PHP_RINIT_FUNCTION(dogma) {
	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(dogma) {
	return SUCCESS;
}

PHP_MINFO_FUNCTION(dogma) {
	php_info_print_table_start();
	php_info_print_table_header(2, "dogma support", "enabled");
	php_info_print_table_end();
}



/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */



ZEND_FUNCTION(dogma_init_context) {
	zval* zctx;
	dogma_context_t* ctx;
	int ret, res;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &zctx) == FAILURE) {
		RETURN_FALSE;
	}

	ret = dogma_init_context(&ctx);

	if(ret == DOGMA_OK) {
		convert_to_null(zctx);
		res = ZEND_REGISTER_RESOURCE(NULL, ctx, le_dogma_context);
		ZVAL_RESOURCE(zctx, res);
	}

	RETURN_LONG(ret);
}

ZEND_FUNCTION(dogma_free_context) {
	zval* zctx;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zctx) == FAILURE) {
		RETURN_FALSE;
	}
	zend_list_delete(Z_LVAL_P(zctx));
	RETURN_LONG(DOGMA_OK);
}



/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */



ZEND_FUNCTION(dogma_add_implant) {
	zval* zctx;
	dogma_context_t* ctx;
	long implant;
	zval* zkey;
	dogma_key_t key;
	int ret;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rlz", &zctx, &implant, &zkey) == FAILURE) {
		RETURN_FALSE;
	}
	GET_CTX(zctx, ctx);

	ret = dogma_add_implant(ctx, (dogma_typeid_t)implant, &key);
	convert_to_null(zkey);
	ZVAL_LONG(zkey, (long)key);

	RETURN_LONG(ret);
}

ZEND_FUNCTION(dogma_remove_implant) {
	zval* zctx;
	dogma_context_t* ctx;
	long key;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &zctx, &key) == FAILURE) {
		RETURN_FALSE;
	}
	GET_CTX(zctx, ctx);

	RETURN_LONG(dogma_remove_implant(ctx, (dogma_key_t)key));
}



/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */



ZEND_FUNCTION(dogma_set_default_skill_level) {
	zval* zctx;
	dogma_context_t* ctx;
	long ds;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &zctx, &ds) == FAILURE) {
		RETURN_FALSE;
	}
	GET_CTX(zctx, ctx);
	RETURN_LONG(dogma_set_default_skill_level(ctx, (uint8_t)ds));
}

ZEND_FUNCTION(dogma_set_skill_level) {
	zval* zctx;
	dogma_context_t* ctx;
	long skill, ds;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rll", &zctx, &skill, &ds) == FAILURE) {
		RETURN_FALSE;
	}
	GET_CTX(zctx, ctx);
	RETURN_LONG(dogma_set_skill_level(ctx, (dogma_typeid_t)skill, (uint8_t)ds));
}

ZEND_FUNCTION(dogma_reset_skill_levels) {
	zval* zctx;
	dogma_context_t* ctx;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zctx) == FAILURE) {
		RETURN_FALSE;
	}
	GET_CTX(zctx, ctx);
	RETURN_LONG(dogma_reset_skill_levels(ctx));
}



/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */



ZEND_FUNCTION(dogma_set_ship) {
	zval* zctx;
	dogma_context_t* ctx;
	long ship;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &zctx, &ship) == FAILURE) {
		RETURN_FALSE;
	}
	GET_CTX(zctx, ctx);
	RETURN_LONG(dogma_set_ship(ctx, (dogma_typeid_t)ship));
}



/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */



ZEND_FUNCTION(dogma_add_module) {
	zval* zctx;
	dogma_context_t* ctx;
	long typeid;
	zval* zkey;
	dogma_key_t key;
	int ret;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rlz", &zctx, &typeid, &zkey) == FAILURE) {
		RETURN_FALSE;
	}
	GET_CTX(zctx, ctx);

	ret = dogma_add_module(ctx, typeid, &key);
	convert_to_null(zkey);
	ZVAL_LONG(zkey, key);
	RETURN_LONG(ret);
}

ZEND_FUNCTION(dogma_add_module_s) {
	zval* zctx;
	dogma_context_t* ctx;
	long typeid;
	zval* zkey;
	dogma_key_t key;
	long state;
	int ret;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rlzl", &zctx, &typeid, &zkey, &state) == FAILURE) {
		RETURN_FALSE;
	}
	GET_CTX(zctx, ctx);

	ret = dogma_add_module_s(ctx, typeid, &key, (dogma_state_t)state);
	convert_to_null(zkey);
	ZVAL_LONG(zkey, key);
	RETURN_LONG(ret);
}

ZEND_FUNCTION(dogma_add_module_c) {
	zval* zctx;
	dogma_context_t* ctx;
	long typeid;
	zval* zkey;
	dogma_key_t key;
	long charge;
	int ret;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rlzl", &zctx, &typeid, &zkey, &charge) == FAILURE) {
		RETURN_FALSE;
	}
	GET_CTX(zctx, ctx);

	ret = dogma_add_module_c(ctx, typeid, &key, (dogma_typeid_t)charge);
	convert_to_null(zkey);
	ZVAL_LONG(zkey, key);
	RETURN_LONG(ret);
}

ZEND_FUNCTION(dogma_add_module_sc) {
	zval* zctx;
	dogma_context_t* ctx;
	long typeid;
	zval* zkey;
	dogma_key_t key;
	long state;
	long charge;
	int ret;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rlzll", &zctx, &typeid, &zkey, &state, &charge) == FAILURE) {
		RETURN_FALSE;
	}
	GET_CTX(zctx, ctx);

	ret = dogma_add_module_sc(ctx, typeid, &key, (dogma_state_t)state, (dogma_typeid_t)charge);
	convert_to_null(zkey);
	ZVAL_LONG(zkey, key);
	RETURN_LONG(ret);
}

ZEND_FUNCTION(dogma_remove_module) {
	zval* zctx;
	dogma_context_t* ctx;
	long key;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &zctx, &key) == FAILURE) {
		RETURN_FALSE;
	}
	GET_CTX(zctx, ctx);
	RETURN_LONG(dogma_remove_module(ctx, (dogma_key_t)key));
}

ZEND_FUNCTION(dogma_set_module_state) {
	zval* zctx;
	dogma_context_t* ctx;
	long key;
	long state;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rll", &zctx, &key, &state) == FAILURE) {
		RETURN_FALSE;
	}
	GET_CTX(zctx, ctx);
	RETURN_LONG(dogma_set_module_state(ctx, (dogma_key_t)key, (dogma_state_t)state));
}



/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */



ZEND_FUNCTION(dogma_add_charge) {
	zval* zctx;
	dogma_context_t* ctx;
	long key;
	long typeid;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rll", &zctx, &key, &typeid) == FAILURE) {
		RETURN_FALSE;
	}
	GET_CTX(zctx, ctx);
	RETURN_LONG(dogma_add_charge(ctx, (dogma_key_t)key, (dogma_typeid_t)typeid));
}

ZEND_FUNCTION(dogma_remove_charge) {
	zval* zctx;
	dogma_context_t* ctx;
	long key;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &zctx, &key) == FAILURE) {
		RETURN_FALSE;
	}
	GET_CTX(zctx, ctx);
	RETURN_LONG(dogma_remove_charge(ctx, (dogma_key_t)key));
}



/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */



ZEND_FUNCTION(dogma_add_drone) {
	zval* zctx;
	dogma_context_t* ctx;
	long typeid;
	long quantity;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rll", &zctx, &typeid, &quantity) == FAILURE) {
		RETURN_FALSE;
	}
	GET_CTX(zctx, ctx);
	RETURN_LONG(dogma_add_drone(ctx, (dogma_typeid_t)typeid, (unsigned int)quantity));
}

ZEND_FUNCTION(dogma_remove_drone_partial) {
	zval* zctx;
	dogma_context_t* ctx;
	long typeid;
	long quantity;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rll", &zctx, &typeid, &quantity) == FAILURE) {
		RETURN_FALSE;
	}
	GET_CTX(zctx, ctx);
	RETURN_LONG(dogma_remove_drone_partial(ctx, (dogma_typeid_t)typeid, (unsigned int)quantity));
}

ZEND_FUNCTION(dogma_remove_drone) {
	zval* zctx;
	dogma_context_t* ctx;
	long typeid;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &zctx, &typeid) == FAILURE) {
		RETURN_FALSE;
	}
	GET_CTX(zctx, ctx);
	RETURN_LONG(dogma_remove_drone(ctx, (dogma_typeid_t)typeid));
}



/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */



ZEND_FUNCTION(dogma_toggle_chance_based_effect) {
	zval* zctx;
	dogma_context_t* ctx;
	zval* zloc;
	dogma_location_t loc;
	long effect;
	zend_bool toggle;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzlb", &zctx, &zloc, &effect, &toggle) == FAILURE) {
		RETURN_FALSE;
	}
	if(!dogma_get_location_from_zval(zloc, &loc)) {
		RETURN_FALSE;
	}
	GET_CTX(zctx, ctx);
	RETURN_LONG(dogma_toggle_chance_based_effect(ctx, loc, (dogma_effectid_t)effect, (bool)toggle));
}



/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */



ZEND_FUNCTION(dogma_target) {
	zval* zctx1;
	dogma_context_t* ctx1;
	zval* zloc;
	dogma_location_t loc;
	zval* zctx2;
	dogma_context_t* ctx2;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzr", &zctx1, &zloc, &zctx2) == FAILURE) {
		RETURN_FALSE;
	}
	if(!dogma_get_location_from_zval(zloc, &loc)) {
		RETURN_FALSE;
	}
	GET_CTX(zctx1, ctx1);
	GET_CTX(zctx2, ctx2);
	RETURN_LONG(dogma_target(ctx1, loc, ctx2));
}

ZEND_FUNCTION(dogma_clear_target) {
	zval* zctx;
	dogma_context_t* ctx;
	zval* zloc;
	dogma_location_t loc;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zctx, &zloc) == FAILURE) {
		RETURN_FALSE;
	}
	if(!dogma_get_location_from_zval(zloc, &loc)) {
		RETURN_FALSE;
	}
	GET_CTX(zctx, ctx);
	RETURN_LONG(dogma_clear_target(ctx, loc));
}



/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */



ZEND_FUNCTION(dogma_init_fleet_context) {
	zval* zfctx;
	dogma_fleet_context_t* fctx;
	int ret, res;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &zfctx) == FAILURE) {
		RETURN_FALSE;
	}

	ret = dogma_init_fleet_context(&fctx);

	if(ret == DOGMA_OK) {
		convert_to_null(zfctx);
		res = ZEND_REGISTER_RESOURCE(NULL, fctx, le_dogma_fleet_context);
		ZVAL_RESOURCE(zfctx, res);
	}

	RETURN_LONG(ret);
}

ZEND_FUNCTION(dogma_free_fleet_context) {
	zval* zfctx;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zfctx) == FAILURE) {
		RETURN_FALSE;
	}
	zend_list_delete(Z_LVAL_P(zfctx));
	RETURN_LONG(DOGMA_OK);
}



/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */



ZEND_FUNCTION(dogma_add_fleet_commander) {
	zval* zfctx;
	dogma_fleet_context_t* fctx;
	zval* zctx;
	dogma_context_t* ctx;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rr", &zfctx, &zctx) == FAILURE) {
		RETURN_FALSE;
	}
	GET_FCTX(zfctx, fctx);
	GET_CTX(zctx, ctx);
	RETURN_LONG(dogma_add_fleet_commander(fctx, ctx));
}

ZEND_FUNCTION(dogma_add_wing_commander) {
	zval* zfctx;
	dogma_fleet_context_t* fctx;
	long wing;
	zval* zctx;
	dogma_context_t* ctx;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rlr", &zfctx, &wing, &zctx) == FAILURE) {
		RETURN_FALSE;
	}
	GET_FCTX(zfctx, fctx);
	GET_CTX(zctx, ctx);
	RETURN_LONG(dogma_add_wing_commander(fctx, (dogma_key_t)wing, ctx));
}

ZEND_FUNCTION(dogma_add_squad_commander) {
	zval* zfctx;
	dogma_fleet_context_t* fctx;
	long wing;
	long squad;
	zval* zctx;
	dogma_context_t* ctx;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rllr", &zfctx, &wing, &squad, &zctx) == FAILURE) {
		RETURN_FALSE;
	}
	GET_FCTX(zfctx, fctx);
	GET_CTX(zctx, ctx);
	RETURN_LONG(dogma_add_squad_commander(fctx, (dogma_key_t)wing, (dogma_key_t)squad, ctx));
}

ZEND_FUNCTION(dogma_add_squad_member) {
	zval* zfctx;
	dogma_fleet_context_t* fctx;
	long wing;
	long squad;
	zval* zctx;
	dogma_context_t* ctx;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rllr", &zfctx, &wing, &squad, &zctx) == FAILURE) {
		RETURN_FALSE;
	}
	GET_FCTX(zfctx, fctx);
	GET_CTX(zctx, ctx);
	RETURN_LONG(dogma_add_squad_member(fctx, (dogma_key_t)wing, (dogma_key_t)squad, ctx));
}



/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */



ZEND_FUNCTION(dogma_remove_fleet_member) {
	zval* zfctx;
	dogma_fleet_context_t* fctx;
	zval* zctx;
	dogma_context_t* ctx;
	zval* zfound;
	bool found;
	int ret;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rrz", &zfctx, &zctx, &zfound) == FAILURE) {
		RETURN_FALSE;
	}
	GET_FCTX(zfctx, fctx);
	GET_CTX(zctx, ctx);

	ret = dogma_remove_fleet_member(fctx, ctx, &found);
	convert_to_null(zfound);
	ZVAL_BOOL(zfound, (zend_bool)found);
	RETURN_LONG(ret);
}



/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */



ZEND_FUNCTION(dogma_set_fleet_booster) {
	zval* zfctx;
	dogma_fleet_context_t* fctx;
	zval* zctx;
	dogma_context_t* ctx;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zfctx, &zctx) == FAILURE) {
		RETURN_FALSE;
	}
	GET_CTX_NULLABLE(zctx, ctx);
	GET_FCTX(zfctx, fctx);
	RETURN_LONG(dogma_set_fleet_booster(fctx, ctx));
}

ZEND_FUNCTION(dogma_set_wing_booster) {
	zval* zfctx;
	dogma_fleet_context_t* fctx;
	long wing;
	zval* zctx;
	dogma_context_t* ctx;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rlz", &zfctx, &wing, &zctx) == FAILURE) {
		RETURN_FALSE;
	}
	GET_CTX_NULLABLE(zctx, ctx);
	GET_FCTX(zfctx, fctx);
	RETURN_LONG(dogma_set_wing_booster(fctx, wing, ctx));
}

ZEND_FUNCTION(dogma_set_squad_booster) {
	zval* zfctx;
	dogma_fleet_context_t* fctx;
	long wing;
	long squad;
	zval* zctx;
	dogma_context_t* ctx;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rllz", &zfctx, &wing, &squad, &zctx) == FAILURE) {
		RETURN_FALSE;
	}
	GET_CTX_NULLABLE(zctx, ctx);
	GET_FCTX(zfctx, fctx);
	RETURN_LONG(dogma_set_squad_booster(fctx, wing, squad, ctx));
}



/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */



ZEND_FUNCTION(dogma_get_location_attribute) {
	zval* zctx;
	dogma_context_t* ctx;
	zval* zloc;
	dogma_location_t loc;
	long attribute;
	zval* zout;
	double out;
	int ret;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzlz", &zctx, &zloc, &attribute, &zout) == FAILURE) {
		RETURN_FALSE;
	}
	if(!dogma_get_location_from_zval(zloc, &loc)) {
		RETURN_FALSE;
	}
	GET_CTX(zctx, ctx);

	ret = dogma_get_location_attribute(ctx, loc, (dogma_attributeid_t)attribute, &out);
	convert_to_null(zout);
	ZVAL_DOUBLE(zout, out);
	RETURN_LONG(ret);
}

ZEND_FUNCTION(dogma_get_character_attribute) {
	zval* zctx;
	dogma_context_t* ctx;
	long attribute;
	zval* zout;
	double out;
	int ret;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rlz", &zctx, &attribute, &zout) == FAILURE) {
		RETURN_FALSE;
	}
	GET_CTX(zctx, ctx);

	ret = dogma_get_character_attribute(ctx, (dogma_attributeid_t)attribute, &out);
	convert_to_null(zout);
	ZVAL_DOUBLE(zout, out);
	RETURN_LONG(ret);
}

ZEND_FUNCTION(dogma_get_implant_attribute) {
	zval* zctx;
	dogma_context_t* ctx;
	long key;
	long attribute;
	zval* zout;
	double out;
	int ret;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rllz", &zctx, &key, &attribute, &zout) == FAILURE) {
		RETURN_FALSE;
	}
	GET_CTX(zctx, ctx);

	ret = dogma_get_implant_attribute(ctx, (dogma_key_t)key, (dogma_attributeid_t)attribute, &out);
	convert_to_null(zout);
	ZVAL_DOUBLE(zout, out);
	RETURN_LONG(ret);
}

ZEND_FUNCTION(dogma_get_skill_attribute) {
	zval* zctx;
	dogma_context_t* ctx;
	long skill;
	long attribute;
	zval* zout;
	double out;
	int ret;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rllz", &zctx, &skill, &attribute, &zout) == FAILURE) {
		RETURN_FALSE;
	}
	GET_CTX(zctx, ctx);

	ret = dogma_get_skill_attribute(ctx, (dogma_typeid_t)skill, (dogma_attributeid_t)attribute, &out);
	convert_to_null(zout);
	ZVAL_DOUBLE(zout, out);
	RETURN_LONG(ret);
}

ZEND_FUNCTION(dogma_get_ship_attribute) {
	zval* zctx;
	dogma_context_t* ctx;
	long attribute;
	zval* zout;
	double out;
	int ret;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rlz", &zctx, &attribute, &zout) == FAILURE) {
		RETURN_FALSE;
	}
	GET_CTX(zctx, ctx);

	ret = dogma_get_ship_attribute(ctx, (dogma_attributeid_t)attribute, &out);
	convert_to_null(zout);
	ZVAL_DOUBLE(zout, out);
	RETURN_LONG(ret);
}

ZEND_FUNCTION(dogma_get_module_attribute) {
	zval* zctx;
	dogma_context_t* ctx;
	long key;
	long attribute;
	zval* zout;
	double out;
	int ret;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rllz", &zctx, &key, &attribute, &zout) == FAILURE) {
		RETURN_FALSE;
	}
	GET_CTX(zctx, ctx);

	ret = dogma_get_module_attribute(ctx, (dogma_key_t)key, (dogma_attributeid_t)attribute, &out);
	convert_to_null(zout);
	ZVAL_DOUBLE(zout, out);
	RETURN_LONG(ret);
}

ZEND_FUNCTION(dogma_get_charge_attribute) {
	zval* zctx;
	dogma_context_t* ctx;
	long key;
	long attribute;
	zval* zout;
	double out;
	int ret;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rllz", &zctx, &key, &attribute, &zout) == FAILURE) {
		RETURN_FALSE;
	}
	GET_CTX(zctx, ctx);

	ret = dogma_get_charge_attribute(ctx, (dogma_key_t)key, (dogma_attributeid_t)attribute, &out);
	convert_to_null(zout);
	ZVAL_DOUBLE(zout, out);
	RETURN_LONG(ret);
}

ZEND_FUNCTION(dogma_get_drone_attribute) {
	zval* zctx;
	dogma_context_t* ctx;
	long drone;
	long attribute;
	zval* zout;
	double out;
	int ret;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rllz", &zctx, &drone, &attribute, &zout) == FAILURE) {
		RETURN_FALSE;
	}
	GET_CTX(zctx, ctx);

	ret = dogma_get_drone_attribute(ctx, (dogma_typeid_t)drone, (dogma_attributeid_t)attribute, &out);
	convert_to_null(zout);
	ZVAL_DOUBLE(zout, out);
	RETURN_LONG(ret);
}



/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */



ZEND_FUNCTION(dogma_get_chance_based_effect_chance) {
	zval* zctx;
	dogma_context_t* ctx;
	zval* zloc;
	dogma_location_t loc;
	long effect;
	zval* zout;
	double out;
	int ret;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzlz", &zctx, &zloc, &effect, &zout) == FAILURE) {
		RETURN_FALSE;
	}
	if(!dogma_get_location_from_zval(zloc, &loc)) {
		RETURN_FALSE;
	}
	GET_CTX(zctx, ctx);

	ret = dogma_get_chance_based_effect_chance(ctx, loc, (dogma_effectid_t)effect, &out);
	convert_to_null(zout);
	ZVAL_DOUBLE(zout, out);
	RETURN_LONG(ret);
}



/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */



ZEND_FUNCTION(dogma_get_affectors) {
	zval* zctx;
	dogma_context_t* ctx;
	zval* zloc;
	dogma_location_t loc;
	zval* zout;
	dogma_simple_affector_t* list;
	size_t naffectors;
	char operator[2];
	int ret;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzz", &zctx, &zloc, &zout) == FAILURE) {
		RETURN_FALSE;
	}
	if(!dogma_get_location_from_zval(zloc, &loc)) {
		RETURN_FALSE;
	}
	GET_CTX(zctx, ctx);

	ret = dogma_get_affectors(ctx, loc, &list, &naffectors);
	convert_to_null(zout);
	array_init_size(zout, naffectors);

	for(size_t i = 0; i < naffectors; ++i) {
		dogma_simple_affector_t* affector = list + i;
		zval* arr;
		MAKE_STD_ZVAL(arr);
		array_init_size(arr, 6);
		snprintf(operator, 2, "%c", affector->operator);
		add_assoc_long_ex(arr, "id", sizeof("id"), affector->id);
		add_assoc_long_ex(arr, "destid", sizeof("destid"), affector->destid);
		add_assoc_double_ex(arr, "value", sizeof("value"), affector->value);
		add_assoc_string_ex(arr, "operator", sizeof("operator"), operator, 1);
		add_assoc_long_ex(arr, "order", sizeof("order"), affector->order);
		add_assoc_long_ex(arr, "flags", sizeof("flags"), affector->flags);
		add_next_index_zval(zout, arr);
	}

	dogma_free_affector_list(list);
	RETURN_LONG(ret);
}



/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */



ZEND_FUNCTION(dogma_type_has_effect) {
	long type;
	long state;
	long effect;
	zval* zout;
	bool out;
	int ret;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lllz", &type, &state, &effect, &zout) == FAILURE) {
		RETURN_FALSE;
	}

	ret = dogma_type_has_effect((dogma_typeid_t)type, (dogma_state_t)state, (dogma_effectid_t)effect, &out);
	convert_to_null(zout);
	ZVAL_BOOL(zout, (zend_bool)out);
	RETURN_LONG(ret);
}

ZEND_FUNCTION(dogma_type_has_active_effects) {
	long type;
	zval* zout;
	bool out;
	int ret;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lz", &type, &zout) == FAILURE) {
		RETURN_FALSE;
	}

	ret = dogma_type_has_active_effects((dogma_typeid_t)type, &out);
	convert_to_null(zout);
	ZVAL_BOOL(zout, (zend_bool)out);
	RETURN_LONG(ret);
}

ZEND_FUNCTION(dogma_type_has_overload_effects) {
	long type;
	zval* zout;
	bool out;
	int ret;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lz", &type, &zout) == FAILURE) {
		RETURN_FALSE;
	}

	ret = dogma_type_has_overload_effects((dogma_typeid_t)type, &out);
	convert_to_null(zout);
	ZVAL_BOOL(zout, (zend_bool)out);
	RETURN_LONG(ret);
}

ZEND_FUNCTION(dogma_type_base_attribute) {
	long type;
	long attribute;
	zval* zout;
	double out;
	int ret;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llz", &type, &attribute, &zout) == FAILURE) {
		RETURN_FALSE;
	}

	ret = dogma_type_base_attribute((dogma_typeid_t)type, (dogma_attributeid_t)attribute, &out);
	convert_to_null(zout);
	ZVAL_DOUBLE(zout, out);
	RETURN_LONG(ret);
}



/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */



ZEND_FUNCTION(dogma_get_number_of_module_cycles_before_reload) {
	zval* zctx;
	dogma_context_t* ctx;
	long key;
	zval* zout;
	int num_cycles;
	int ret;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rlz", &zctx, &key, &zout) == FAILURE) {
		RETURN_FALSE;
	}
	GET_CTX(zctx, ctx);

	ret = dogma_get_number_of_module_cycles_before_reload(
		ctx, (dogma_key_t)key, &num_cycles
	);
	convert_to_null(zout);
	ZVAL_LONG(zout, (long)num_cycles);
	RETURN_LONG(ret);
}



/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */



ZEND_FUNCTION(dogma_get_capacitor) {
	zval* zctx;
	dogma_context_t* ctx;
	zend_bool reload;
	zval* zdelta;
	double delta;
	zval* zstable;
	bool stable;
	zval* zparam;
	double param;
	int ret;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rbzzz", &zctx, &reload,
	                         &zdelta, &zstable, &zparam) == FAILURE) {
		RETURN_FALSE;
	}
	GET_CTX(zctx, ctx);

	ret = dogma_get_capacitor(ctx, (bool)reload, &delta, &stable, &param);
	convert_to_null(zdelta);
	convert_to_null(zstable);
	convert_to_null(zparam);
	ZVAL_DOUBLE(zdelta, delta);
	ZVAL_BOOL(zstable, (zend_bool)stable);
	ZVAL_DOUBLE(zparam, param);
	RETURN_LONG(ret);
}



/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */



ZEND_FUNCTION(dogma_get_nth_type_effect_with_attributes) {
	long type, n;
	zval* zout;
	dogma_effectid_t out;
	int ret;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llz", &type, &n, &zout) == FAILURE) {
		RETURN_FALSE;
	}

	ret = dogma_get_nth_type_effect_with_attributes((dogma_typeid_t)type, (unsigned int)n, &out);
	convert_to_null(zout);
	ZVAL_LONG(zout, out);
	RETURN_LONG(ret);
}

ZEND_FUNCTION(dogma_get_location_effect_attributes) {
	zval* zctx;
	dogma_context_t* ctx;
	zval* zloc;
	dogma_location_t loc;
	long effect;
	double dur, tra, dis, ran, fal, fuc;
	zval* zdur;
	zval* ztra;
	zval* zdis;
	zval* zran;
	zval* zfal;
	zval* zfuc;
	int ret;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzlzzzzzz", &zctx, &zloc, &effect,
	                         &zdur, &ztra, &zdis, &zran, &zfal, &zfuc) == FAILURE) {
		RETURN_FALSE;
	}
	if(!dogma_get_location_from_zval(zloc, &loc)) {
		RETURN_FALSE;
	}
	GET_CTX(zctx, ctx);

	ret = dogma_get_location_effect_attributes(
		ctx, loc, effect,
		&dur, &tra, &dis, &ran, &fal, &fuc
	);
	convert_to_null(zdur);
	convert_to_null(ztra);
	convert_to_null(zdis);
	convert_to_null(zran);
	convert_to_null(zfal);
	convert_to_null(zfuc);
	ZVAL_DOUBLE(zdur, dur);
	ZVAL_DOUBLE(ztra, tra);
	ZVAL_DOUBLE(zdis, dis);
	ZVAL_DOUBLE(zran, ran);
	ZVAL_DOUBLE(zfal, fal);
	ZVAL_DOUBLE(zfuc, fuc);
	RETURN_LONG(ret);
}

