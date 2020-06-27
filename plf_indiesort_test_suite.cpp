#if defined(_MSC_VER)
	#if _MSC_VER >= 1600
		#define PLF_MOVE_SEMANTICS_SUPPORT
	#endif
#elif defined(__cplusplus) && __cplusplus >= 201103L // C++11 support, at least
	#define PLF_MOVE_SEMANTICS_SUPPORT
#endif


#include <functional> // std::greater
#include <vector>
#include <list>
#include <iostream>

#include "plf_indiesort.h"



void title1(const char *title_text)
{
	std::cout << std::endl << std::endl << std::endl << "*** " << title_text << " ***" << std::endl;
	std::cout << "===========================================" << std::endl << std::endl << std::endl; 
}



void title2(const char *title_text)
{
	std::cout << std::endl << std::endl << "--- " << title_text << " ---" << std::endl << std::endl;
}



void breakfail(const char *error_message)
{
	std::cout << error_message << std::endl;
	std::cin.get();
	abort();
}



struct small_struct
{
	double *empty_field_1;
	double unused_number;
	unsigned int empty_field2;
	double *empty_field_3;
	int number;
	unsigned int empty_field4;

	small_struct() : number(0) {};
	small_struct(const int num) : number(num) {};
	int operator * () const { return number; };
	bool operator == (const small_struct &source) const { return source.number == number; };
	bool operator != (const small_struct &source) const { return source.number != number; };
	bool operator > (const small_struct &source) const { return number > source.number; };
	bool operator < (const small_struct &source) const { return number < source.number; };
	bool operator >= (const small_struct &source) const { return number >= source.number; };
	bool operator <= (const small_struct &source) const { return number <= source.number; };
};



int global_counter = 0;

struct small_struct_non_trivial
{
	double *empty_field_1;
	double unused_number;
	unsigned int empty_field2;
	double *empty_field_3;
	int number;
	unsigned int empty_field4;

	small_struct_non_trivial() : number(0) {};
	small_struct_non_trivial(const int num) : number(num) {};
	small_struct_non_trivial(const small_struct_non_trivial &source) : number(source.number) {};
	small_struct_non_trivial operator = (small_struct_non_trivial &source) { number = source.number; return *this; };

	#ifdef PLF_MOVE_SEMANTICS_SUPPORT
		small_struct_non_trivial(small_struct_non_trivial &&source) : number(std::move(source.number)) {};
		small_struct_non_trivial operator = (small_struct_non_trivial &&source) { number = std::move(source.number); return *this; };
	#endif

	int operator * () const { return number; };
	bool operator == (const small_struct_non_trivial &source) const { return source.number == number; };
	bool operator != (const small_struct_non_trivial &source) const { return source.number != number; };
	bool operator > (const small_struct_non_trivial &source) const { return number > source.number; };
	bool operator < (const small_struct_non_trivial &source) const { return number < source.number; };
	bool operator >= (const small_struct_non_trivial &source) const { return number >= source.number; };
	bool operator <= (const small_struct_non_trivial &source) const { return number <= source.number; };
};



struct large_struct
{
	int numbers[100];
	char a_string[50];
	double unused_number;
	int number;
	double *empty_field_1;
	double *empty_field_2;
	unsigned int empty_field3;
	unsigned int empty_field4;

	large_struct() : number(0) {};
	large_struct(const int num) : number(num) {};
	int operator * () const { return number; };
	bool operator == (const large_struct &source) const { return source.number == number; };
	bool operator != (const large_struct &source) const { return source.number != number; };
	bool operator > (const large_struct &source) const { return number > source.number; };
	bool operator < (const large_struct &source) const { return number < source.number; };
	bool operator >= (const large_struct &source) const { return number >= source.number; };
	bool operator <= (const large_struct &source) const { return number <= source.number; };
};



// Fast xorshift+128 random number generator function (original: https://codingforspeed.com/using-faster-psudo-random-generator-xorshift/)
unsigned int xor_rand()
{
	static unsigned int x = 123456789;
	static unsigned int y = 362436069;
	static unsigned int z = 521288629;
	static unsigned int w = 88675123;

	const unsigned int t = x ^ (x << 11);

	// Rotate the static values (w rotation in return statement):
	x = y;
	y = z;
	z = w;

	return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
}



int main()
{
	freopen("error.log","w", stderr); // For catching assertion failure info when run outside of a command line prompt

	unsigned int looper = 0;

	while (++looper != 600)
	{
		title1("Indiesort");

		{
			title2("Test Basics Int");

			std::vector<int> vec;
			
			for (int i = 60; i != -1; --i)
			{
				vec.push_back(i);
			}
			
			for (int i = 0; i != 60; ++i)
			{
				vec.push_back(i);
			}
			
			plf::indiesort(vec);
			
			int last = 0;
			
			for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); ++it)
			{
				if (last > *it)
				{
					breakfail("Sequential int last > *it!.");
				}
				
				last = *it;
			}

			std::cout << "Sequential int passed\n";

			vec.clear();
			
			
			for (int i = 0; i != 70000; ++i)
			{
				vec.push_back(xor_rand() & 65535);
			}
			
			plf::indiesort(vec, std::greater<int>());
			
			last = 65535;
			
			for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); ++it)
			{
				if (last < *it)
				{
					breakfail("Random int last < *it!.");
				}
				
				last = *it;
			}

			std::cout << "Random int passed\n";
		}

		
		{
			title2("Test Basics small_struct");

			std::vector<small_struct> vec;
			
			for (int i = 60; i != -1; --i)
			{
				vec.push_back(i);
			}
			
			for (int i = 0; i != 60; ++i)
			{
				vec.push_back(i);
			}
			
			plf::indiesort(vec);
			
			small_struct last = 0;
			
			for (std::vector<small_struct>::iterator it = vec.begin(); it != vec.end(); ++it)
			{
				if (last > **it)
				{
					breakfail("Sequential small_struct last > *it!.");
				}
				
				last = **it;
			}

			std::cout << "Sequential Small Struct passed\n";

			vec.clear();
			
			
			for (int i = 0; i != 6000; ++i)
			{
				vec.push_back(xor_rand() & 65535);
			}
			
			plf::indiesort(vec, std::greater<small_struct>());
			
			last = 65535;
			
			for (std::vector<small_struct>::iterator it = vec.begin(); it != vec.end(); ++it)
			{
				if (last < *it)
				{
					breakfail("Random small_struct last < *it!.");
				}
				
				last = *it;
			}

			std::cout << "Random Small Struct passed\n";
		}



		{
			title2("Test Basics C array");

			large_struct *structs = new large_struct[500];

			for (int i = 0; i != 500; ++i)
			{
				structs[i] = xor_rand() & 65535;
			}

			plf::indiesort(&structs[0], &structs[500]);

			large_struct last = 0;

			for (int i = 0; i != 500; ++i)
			{
				if (last > structs[i])
				{
					breakfail("Random large_struct last > *it!.");
				}
				
				last = structs[i];
			}

			std::cout << "Random large_struct C array passed\n";
		}



		{
			title2("Test Basics small_struct_non_trivial");

			std::vector<small_struct_non_trivial> vec;
			
			for (int i = 60; i != -1; --i)
			{
				vec.push_back(i);
			}
			
			for (int i = 0; i != 60; ++i)
			{
				vec.push_back(i);
			}
			
			plf::indiesort(vec);

			small_struct_non_trivial last = 0;
			
			for (std::vector<small_struct_non_trivial>::iterator it = vec.begin(); it != vec.end(); ++it)
			{
				if (last > **it)
				{
					breakfail("Sequential small_struct_non_trivial last > *it!.");
				}
				
				last = **it;
			}

			std::cout << "Sequential small_struct_non_trivial passed\n";

			vec.clear();
			
			
			for (int i = 0; i != 6000; ++i)
			{
				vec.push_back(xor_rand() & 65535);
			}
			
			plf::indiesort(vec, std::greater<small_struct_non_trivial>());
			
			last = 65535;
			
			for (std::vector<small_struct_non_trivial>::iterator it = vec.begin(); it != vec.end(); ++it)
			{
				if (last < *it)
				{
					breakfail("Random small_struct_non_trivial last < *it!.");
				}
				
				last = *it;
			}

			std::cout << "Random small_struct_non_trivial passed\n";
		}



		{
			title2("Test Basics large_struct");

			std::vector<large_struct> vec;
			
			for (int i = 60; i != -1; --i)
			{
				vec.push_back(i);
			}
			
			for (int i = 0; i != 60; ++i)
			{
				vec.push_back(i);
			}
			
			plf::indiesort(vec);
			
			large_struct last = 0;
			
			for (std::vector<large_struct>::iterator it = vec.begin(); it != vec.end(); ++it)
			{
				if (last > **it)
				{
					breakfail("Sequential large_struct last > *it!.");
				}
				
				last = **it;
			}

			std::cout << "Sequential large_struct passed\n";

			vec.clear();
			
			
			for (int i = 0; i != 6000; ++i)
			{
				vec.push_back(xor_rand() & 65535);
			}
			
			plf::indiesort(vec, std::greater<large_struct>());
			
			last = 65535;
			
			for (std::vector<large_struct>::iterator it = vec.begin(); it != vec.end(); ++it)
			{
				if (last < *it)
				{
					breakfail("Random large_struct last < *it!.");
				}
				
				last = *it;
			}

			std::cout << "Random large_struct passed\n";
		}




		{
			title2("std::list int range tests");

			{		
				std::list<int> ivec;
				
				for (int i = 0; i != 2000; ++i)
				{
					ivec.push_back(xor_rand() & 65535);
				}
				
				
				std::list<int>::iterator start = ivec.begin(), end = ivec.end();

				plf::indiesort(start, end);
				

				int last = 0;

				do
				{
					if (last > *start)
					{
						breakfail("std::list sort test > *it!.");
					}
					
					last = *start;
				} while (++start != end);

				std::cout << "std::list sort test passed\n";
			}



			{
				title2("std::list small_struct range tests");

				std::list<small_struct> vec;
				
				for (int i = 0; i != 5000; ++i)
				{
					vec.push_back(xor_rand() & 65535);
				}
				
				
				std::list<small_struct>::iterator start = vec.begin(), end = vec.end();
				std::advance(start, 150);
				std::advance(end, -300);

				plf::indiesort(start, end);
				

				int last = 0;

				do
				{
					if (last > **start)
					{
						breakfail("std::list small_struct sort test > *it!.");
					}
					
					last = **start;
				} while (++start != end);

				std::cout << "std::list small_struct sort test passed\n";
			}



			{
				title2("std::list large_struct range tests");

				std::list<large_struct> vec;
				
				for (int i = 0; i != 6000; ++i)
				{
					vec.push_back(xor_rand() & 65535);
				}
				
				
				std::list<large_struct>::iterator start = vec.begin(), end = vec.end();
				std::advance(start, 15);
				std::advance(end, -200);

				plf::indiesort(start, end);
				

				int last = 0;

				do
				{
					if (last > **start)
					{
						breakfail("std::list large_struct sort test > *it!.");
					}
					
					last = **start;
				} while (++start != end);

				std::cout << "std::list large_struct sort test passed\n";
			}


		}
	}


	title1("All tests passed! Press Enter to Exit.");
	std::cin.get();

	return 0;
}
