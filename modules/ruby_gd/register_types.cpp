#include "register_types.h"

#include "ruby_gd.h"

void register_ruby_gd_types() {
	ClassDB::register_class<Ruby>();
}

void unregister_ruby_gd_types() {
}
