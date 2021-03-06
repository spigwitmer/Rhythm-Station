Simple Lua Binder (SLB)
-------------------------

SLB is an "simple-to-use" library to wrap C/C++ and use it from lua scripts.
SLB can be compiled as dynamic library, or it can be included in your
project, see INSTALLATION for further instructions.  

The aim of SLB is to provide an easy to use library to wrap code to lua and
extend your applications from lua. Also SLB can be used as a reflection
library for C/C++, you can access to all the registered classes and ask for
its methods, metadata, and so on.

Recently SLB is also a thread-safe library, but this feature is being tested
right now.


USING SLB
---------

The best way to learn how to use SLB is looking at the examples, they are
numbered in order of complexity. first of all SLB uses its own namespace, and
it stores everything under the SLB::Manager class (which follows the singleton
pattern), you don't normally have to care about SLB's internal classes, just
take care on your own classes and how they are handled by SLB. 

The SLB::Script class can be used as a base class to handle lua scripts, but
it is not mandatory to use it, you can still use SLB on your own lua_states,
for example:

	// Custom SLB::SCript, a simplification to use SLB
	SLB::Script s;
	s.doString(lua_code);

or, if you're creating your own lua_States then:

    // Create a lua State, using normal lua API
    lua_State *L = luaL_newstate();
    // load default functions (the current example uses print)
    // and by default, SLB::Script does this.
    luaL_openlibs(L);
    // Register SLB inside the lua_State, calling the SLB::Manager
    // that handles bindings, default functions, default values...
    SLB::Manager::getInstance().registerSLB(L);
    // No call lua API to execute the same code as above
    luaL_dostring(L, lua_code); // execute code

Once, SLB is registered on the lua_State, you can start using your wrapped
classes. 

To wrap a class, you basically declare its contents, for example:

	SLB::Class< FirstClass >("Namespace::FirstClass")
		// declares an empty constructor
        .constructor()
        .set("getString", &FirstClass::getString)
        .set("setString", &FirstClass::setString)
        .set("getInt", &FirstClass::getInt)
        .set("setInt", &FirstClass::setInt)
	;

This code must be executed somewhere before a script can make use of the
"FirstClass" class. Classes can declare just one constructor, and overloading
is not supported. The .set commands can be used for anything, not just methods
of the class, you can associate values, enums, c functions, and so on.

SLB doesn't pollute the lua global namespace, it stores everything under a
table called SLB. Even your classes are there, but you can populate a
namespace to the global table with the method "SLB.using(namespace)". For
example, without the using the above class can be instantiated from the script
with:

	fc = SLB.Namespace.FirstClass()

With the using, we can do:

	SLB.using(SLB.Namespace)
	fc = FirstClass()

Once we have received/created a class we can access to its methods, but
instead of the dot(.) notation in lua we use the colon(:) for example:

	fc:setString("a string")
	print(fc:getString())

To see more advanced use of SLB, see the examples, and wiki docs(under
construction).

INSTALLATION
------------

SLB uses cmake, but it is mainly used for the SLB's development. You are
encouraged to use SLB as part of your own library/program for the sake of
simplicity, but it is ready to be used as a dynamic library.  

To include SLB as part of your own project, grab the src/ and include/
directories and compile all the .cpp files under src/. If SLB is going to be
part of a dll, or a dll itself, then enable the macro declaration SLB_LIBRARY
that will properly adjust the SLB_EXPORT macro. 

To compile SLB using cmake, use cmake as with any other project. Right now
SLB doesn't have installation rules, but in the bin directory it will generate
dynamic and static versions of the current configuration (Debug|Release). 



