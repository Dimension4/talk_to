#include <initializer_list>

auto values = {1, 2, 3, 4};
int x, y, z;

x == any_of(values);
x < any_of(values);
x < any_of(values) < y;
x > y < any_of(values) < z

x == all_of(values);

any_of(values) < all_of(values)