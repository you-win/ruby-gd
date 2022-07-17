#ifndef RUBY_GD_H
#define RUBY_GD_H

#include "core/reference.h"

#include "thirdparty/mruby/mruby.h"

class Ruby : public Reference {
	GDCLASS(Ruby, Reference);

	mrb_state *mrb;

protected:
	static void _bind_methods();

private:
	Variant _handle_return_value(const mrb_value *p_value);

public:
	Variant load_file(const String &p_path);
	Variant load_string(const String &p_text);

	Variant call_func(const String &p_func_name, const Array &p_args);

	void garbage_collect();
	void full_garbage_collect();

	Ruby();
	~Ruby();
};

#endif // RUBY_GD_H
