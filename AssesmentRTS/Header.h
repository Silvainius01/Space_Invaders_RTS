#pragma once

#include <fstream>
class Debug
{
private:
	// constructor is private! We can only call private functions
	// inside of the class
	Debug() { }
	Debug(Debug &) {}
	~Debug() {  }

	std::fstream fout;
public:
	
	//static doesn't require an instance, it's basically just a global
	//function in the namespace of the class
	static Debug &instance()
	{
		static Debug i; // static variable initializes once!
		return i;	// return type is reference!
	}

	static void log(const char *d)
	{
		std::fstream fout;
		instance().fout.open("log.txt", std::ios_base::out | std::ios_base::app);
		instance().fout << d << '\n';
		instance().fout.close();
	}
};


/*
	Globally accessible but only a single instance,
	then use singleton design pattern (or something like it)
*/

void blah()
{

	Debug::log("roar!");
}