php-dogma
=========

PHP bindings of the libdogma library.

Released under the GNU Affero General Public License, version 3 or
later. The full license text is included in the `COPYING` file.

Requires libdogma and pkg-config.

Main libdogma repository: https://github.com/Artefact2/libdogma

Contact: artefact2@gmail.com

Compiling and using the extension:

~~~
phpize
./configure
make

# Run tests
php -d "extension=modules/dogma.so" test-core.php
php -d "extension=modules/dogma.so" test-extra.php

make install

# You can now add extension=dogma.so to your php.ini
~~~

API documentation
=================

Unless otherwise stated in the section below, all the functions in
`dogma.h` and `dogma-extra.h` are defined in PHP with exactly the same
prototype and behaviour.

API notes
=========

Here is a list of differences between the libdogma C API and the
provided PHP API :

Core
----

* All PHP functions can return `false` instead of the usual `DOGMA_*`
  constants, this happens when the supplied arguments did not match
  expectations (and you will generally get a warning message). So be
  sure to use `===` to check the return values.

* There is no `dogma_init()` in PHP, this is taken care of for you
  during module initialization.

* Unlike C, PHP has a garbage collector. The contexts created by
  `dogma_init_context()` and `dogma_init_fleet_context()` are not
  persistent and will be automatically freed when no longer referenced
  by anything (just like file handles, database connections,
  etc…). Calling `dogma_free_context()` or
  `dogma_free_fleet_context()` will still free the context as
  expected, even if it is still referenced (just like `fclose()`).

* PHP has no union types, so arguments of type `dogma_location_t` are
  specified differently in PHP: they can be either one of the
  `DOGMA_LOC_*` constants, or an array of this form :

  ~~~
  [ DOGMA_LOC_*,
  	key1 => value1,
	key2 => value2,
	… ]
  ~~~

  Where `key1`, `key2` are members of the `dogma_location_t` structure
  (or the unions inside it), and the location type can be accessed as
  `$array[0]`. The keys will be processed in the order they are
  defined in the `dogma_location_t` structure; values defined last
  will have precedence, regardless of the specified type. Here are
  examples of location arguments:

  ~~~
  DOGMA_LOC_Char                                           // OK
  [ DOGMA_LOC_Char ]                                       // OK
  [ DOGMA_LOC_Module, "module_index" => 1 ]                // OK
  [ "module_index" => 1, DOGMA_LOC_Module ]                // OK
  [ DOGMA_LOC_Charge, "module_index" => 1, "drone_typeid" => 2488 ] // OK, but will use drone_typeid
                                                                    // to overwrite module_index
  DOGMA_LOC_Module                                         // Undefined behaviour (needs module_index)
  [ DOGMA_LOC_Module, 1 ]                                  // Not OK
  [ 3 => DOGMA_LOC_Skill, "skill_typeid" => 3496 ]         // Not OK
  ~~~

  In general usage, it is recommended to use the more specialized
  functions (like `dogma_get_character_attribute`,
  `dogma_get_module_attribute`, etc…) as they will be a tiny bit
  faster (since the location is hardcoded and does not need to be
  constructed from the array every time).

Extra
-----

* `dogma_get_affectors()` only take three arguments (instead of four):
  a dogma context resource, a location and the third parameter is a
  reference that will be set to an array of affectors. Each element of
  the array will have the same fields as the `dogma_simple_affector_s`
  structure. Here is an example for reference:

  ~~~
  dogma_init_context($ctx);
  dogma_get_affectors($ctx, DOGMA_LOC_Char, $arr);
  print_r($arr);

  /* Gives something similar to:

  Array
  (
      [0] => Array
          (
              [id] => 3363
              [destid] => 190
              [value] => 50
              [operator] => +
              [order] => 3
              [flags] => 0
          )

      [1] => Array
          (
              [id] => 3731
              [destid] => 190
              [value] => 250
              [operator] => +
              [order] => 3
              [flags] => 0
          )

      etc…
  )

  */
  ~~~

* The function `dogma_free_affector_list()` is not present in PHP, as
  the list is copied to the array and freed internally. The generated
  array will be garbage collected just like any other PHP array.
