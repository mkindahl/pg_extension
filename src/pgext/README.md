# C++ Support for PostgreSQL Extensions

## Todo

### `ArrayType` iterator

These are PostgreSQL arrays, but we should be able to iterate over
them.

### `List` iterator

The `List` type is opaque, as are all PostgreSQL containers, but we
should allow types at the C++ level.

```c++
int main() {
	pgext::list<int> int_list;
	int_list.push_back(1);
	for (auto elem : int_list) {
		LOG << "Element is" << *elem;
	}
}
```

### Enable Google Test and Google Mock

Being able to use Google Test and Google Mock with the library. This
requires changes upstreams to expose the server-build libraries
`libpgcommon_srv`, `libpostgres_srv`, and `libpgport_srv` so that we
can get the server symbols available to a separate executable.
