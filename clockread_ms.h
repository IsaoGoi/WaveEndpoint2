
#ifndef PLF_NANOTIMER
#define PLF_NANOTIMER


#if defined(_WIN32)
	#if defined(_MSC_VER) && !defined(NOMINMAX)
		#define NOMINMAX // Otherwise MS compilers act like idiots when using std::numeric_limits<>::max() and including windows.h
	#endif
	
	#include <windows.h>
	
	namespace plf
	{

	class nanotimer
	{
	private:
		LARGE_INTEGER ticks1, ticks2;
		double frequency;
	public:
		nanotimer()
		{
			LARGE_INTEGER freq;
			QueryPerformanceFrequency(&freq);
			frequency = static_cast<double>(freq.QuadPart);
		}

		inline void start()
		{
			QueryPerformanceCounter(&ticks1);
		}

		double get_elapsed_ms()
		{
			QueryPerformanceCounter(&ticks2);
			return (static_cast<double>(ticks2.QuadPart - ticks1.QuadPart) * 1000.0) / frequency;
		}

		inline double get_elapsed_us()
		{
			return get_elapsed_ms() * 1000.0;
		}

		inline double get_elapsed_ns()
		{
			return get_elapsed_ms() * 1000000.0;
		}
	};
#endif



#if  defined(_WIN32)
inline void nanosecond_delay(double delay_ns)
{
	nanotimer timer;
	timer.start();

	while(timer.get_elapsed_ns() < delay_ns)
	{};
}



inline void microsecond_delay(double delay_us)
{
	nanosecond_delay(delay_us * 1000.0);
}


inline void millisecond_delay(double delay_ms)
{
	nanosecond_delay(delay_ms * 1000000.0);
}


} // namespace
#endif

#endif // PLF_NANOTIMER
