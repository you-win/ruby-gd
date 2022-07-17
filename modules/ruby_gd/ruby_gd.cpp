#include "ruby_gd.h"

#include "thirdparty/mruby/mruby/compile.h"
#include "thirdparty/mruby/mruby/string.h"

void Ruby::_bind_methods() {
	ClassDB::bind_method(D_METHOD("load_file", "path"), &Ruby::load_file);
	ClassDB::bind_method(D_METHOD("load_string", "text"), &Ruby::load_string);

	ClassDB::bind_method(D_METHOD("garbage_collect"), &Ruby::garbage_collect);
	ClassDB::bind_method(D_METHOD("full_garbage_collect"), &Ruby::full_garbage_collect);
}

Variant Ruby::_handle_return_value(const mrb_value *p_value) {
	if (mrb->exc) {
		mrb_value ret_val = mrb_funcall(mrb, mrb_obj_value(mrb->exc), "inspect", 0);
		return Variant(String(RSTRING_PTR(ret_val)));
	} else {
		switch (mrb_type(*p_value)) {
			case MRB_TT_UNDEF: {
				return Variant();
			} break;
			case MRB_TT_TRUE: {
				return Variant(true);
			} break;
			case MRB_TT_FALSE: {
				return Variant(false);
			} break;
			case MRB_TT_INTEGER: {
				return Variant(mrb_integer(*p_value));
			} break;
			case MRB_TT_FLOAT: {
				return Variant(mrb_float(*p_value));
			} break;
			default: {
				mrb_value ret_val = mrb_funcall(mrb, *p_value, "inspect", 0);
				return mrb_undef_p(ret_val) ? Variant(String(RSTRING_PTR(*p_value))) : Variant(String(RSTRING_PTR(ret_val)));
			}
		}
	}
}

Variant Ruby::load_file(const String &p_path) {
	FILE *file = fopen(p_path.utf8().get_data(), "r");

	int arena_index = mrb_gc_arena_save(mrb);
	mrb_value ret_val = mrb_load_file(mrb, file);
	mrb_gc_arena_restore(mrb, arena_index);

	return _handle_return_value(&ret_val);

	// return Variant(ERR_BUG);
}

Variant Ruby::load_string(const String &p_text) {
	// 2022/07/16 - mruby code comment says the GC leaks when loading
	// This can be mitigated by saving and loading the arena GC
	int arena_index = mrb_gc_arena_save(mrb);
	mrb_value ret_val = mrb_load_string(mrb, p_text.utf8().get_data());
	mrb_gc_arena_restore(mrb, arena_index);

	return _handle_return_value(&ret_val);

	// if (mrb->exc) {
	// 	if (!mrb_undef_p(ret_val)) {
	// 		mrb_print_error(mrb);
	// 	}
	// 	return Variant(ERR_BUG);
	// } else {
	// 	switch (mrb_type(ret_val)) {
	// 		case MRB_TT_UNDEF: {
	// 			return Variant();
	// 		} break;
	// 		case MRB_TT_TRUE: {
	// 			return Variant(true);
	// 		} break;
	// 		case MRB_TT_FALSE: {
	// 			return Variant(false);
	// 		} break;
	// 		case MRB_TT_INTEGER: {
	// 			return Variant(mrb_integer(ret_val));
	// 		} break;
	// 		case MRB_TT_FLOAT: {
	// 			return Variant(mrb_float(ret_val));
	// 		} break;
	// 		default: {
	// 			return Variant();
	// 		}
	// 	}
	// }
}

Variant Ruby::call_func(const String &p_func_name, const Array &p_args) {
	return Variant(OK);
}

void Ruby::garbage_collect() {
	mrb_garbage_collect(mrb);
}

void Ruby::full_garbage_collect() {
	mrb_full_gc(mrb);
}

Ruby::Ruby() {
	mrb = mrb_open();
}

Ruby::~Ruby() {
	if (mrb) {
		mrb_close(mrb);
	}
}
