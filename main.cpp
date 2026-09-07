// impl. polymorphism without virtual, using std::variant

// polymorphism is the ability to use one interface to work with obj. of different types,
// calling different impl. match to the type.

template <typename T>
concept VariantInterface = requires(T obj){
	{ obj.foo() } noexcept -> std::same_as<int>
};

struct A{
    constexpr int foo() const noexcept { return 1; }
};
	
struct B{
    constexpr int foo() const noexcept { return 2; }
};

struct C{
    constexpr int foo() const noexcept { return 3; }
};

using FooVariant = std::variant<A, B, C>;

// template <VariantInterface... Ts>
// using FooVariant = std::variant<Ts...>;

// using MyVariant = FooVariant<A, B, C>;

// func GetFoo: to unpack obj. and get the type contains.
// std::visit functionality:
//		call the callable-obj. with current argument (std::get<index>(variantObject))
template <VariantInterface... Ts>
constexpr auto GetFoo(const std::variant<Ts...>& v){
	return std::visit([]<typename T>(const T& obj){
		static_assert(VariantInterface<T>);
		return obj.foo();
	}, v);
}

int main(){
	FooVariant v = A{};
	
	assert(v.index() == 0);
	assert(GetFoo(v) == 1);
	
	v = B{};
	assert(std::holds_alternative<B>(v)); // check content type
	assert(GetFoo(v) == 2);
	
	v = C{};
    assert(v.index() == 2);
    assert(GetFoo(v) == 3);
}
