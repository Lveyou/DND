//////////////////////////////////////////////////////////////////////////
//name:		DNDMath imp
//author:	Lveyou
//date:		2020-03-10

//other:
//2020-03-10: 封装stl接口。 - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_MATH_IMP_H_
#define _DND_MATH_IMP_H_


#include "DNDDLL.h"
#include "DNDUser.h"

#include <random>
#include <list>
using namespace std;

namespace DND
{
	class Math_imp
	{
	public:	
		//获取类型最小差值
		template<typename T>
		T GetTypeEpsilon()
		{
			return numeric_limits<T>::epsilon();
		}
		//获取类型最小值
		template<typename T>
		T GetTypeMax()
		{
			//括号可屏蔽宏
			return (std::numeric_limits<T>::max)();
		}
		//获取类型最大值
		template<typename T>
		T GetTypeMin()
		{
			return (std::numeric_limits<T>::min)();
		}

		//Random
		//设置种子 0代表随机
		void SetSeed(unsigned int s)
		{
			if (s == 0)
			{
				random_device rd;
				g_random.seed(g_seed = rd());
			}
			else
				g_random.seed(g_seed = s);

		}
		//返回种子值
		unsigned int GetSeed() { return g_seed; }

		//返回[min,max]区间的随机int
		int GetRandInt(int min, int max)
		{
			return GetRandInteger<int>(min, max);
		}

		//返回[min,max)区间的随机float
		float GetRandFloat(float min, float max)
		{
			return GetRandReal<float>(min, max);
		}

		//返回[min,max]区间的 整型随机值
		template <typename T>
		T GetRandInteger(T min, T max)
		{
			static uniform_int_distribution<T> dist_int;
			//设定区间
			dist_int.param(uniform_int_distribution<T>::param_type{ min, max });
			return dist_int(g_random);
		}

		//返回[min,max)区间的 实数随机值
		template <typename T>
		T GetRandReal(T min, T max)
		{
			static uniform_real_distribution<T> dist_real;
			//设定区间
			dist_real.param(uniform_real_distribution<T>::param_type{ min, max });
			return dist_real(g_random);
		}

		//返回 期望mu，标准差sigma 的正态分布随机值
		template <typename T>
		T GetRandNormal(T mu, T sigma)
		{
			static normal_distribution<T> dist_normal;
			//设定期望与标准差
			dist_normal.param(normal_distribution<T>::param_type{ mu, sigma });
			return dist_normal(g_random);
		}

		//返回 权值的 随机索引（均为size_t类型）
		UINT32 GetRandWeight(UINT32* arr_weight, UINT32 arr_size)
		{
			static discrete_distribution<size_t> dist_weight;

			vector<size_t> vec_weight((size_t)arr_size);

			for (UINT32 i = 0; i != arr_size; ++i)
				vec_weight[i] = (arr_weight[i]);

			discrete_distribution<size_t>::param_type p{ begin(vec_weight), end(vec_weight) };
			

			dist_weight.param(p);

			return dist_weight(g_random);
		}
		Math_imp() : g_seed(0)
		{}
	private:
		//种子值
		unsigned int g_seed;
		//随机数序列生成器
		default_random_engine g_random;

	};
}

#endif
