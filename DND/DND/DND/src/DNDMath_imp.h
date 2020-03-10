//////////////////////////////////////////////////////////////////////////
//name:		DNDMath imp
//author:	Lveyou
//date:		2020-03-10

//other:
//2020-03-10: ��װstl�ӿڡ� - Lveyou
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
		//��ȡ������С��ֵ
		template<typename T>
		T GetTypeEpsilon()
		{
			return numeric_limits<T>::epsilon();
		}
		//��ȡ������Сֵ
		template<typename T>
		T GetTypeMax()
		{
			//���ſ����κ�
			return (std::numeric_limits<T>::max)();
		}
		//��ȡ�������ֵ
		template<typename T>
		T GetTypeMin()
		{
			return (std::numeric_limits<T>::min)();
		}

		//Random
		//�������� 0�������
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
		//��������ֵ
		unsigned int GetSeed() { return g_seed; }

		//����[min,max]��������int
		int GetRandInt(int min, int max)
		{
			return GetRandInteger<int>(min, max);
		}

		//����[min,max)��������float
		float GetRandFloat(float min, float max)
		{
			return GetRandReal<float>(min, max);
		}

		//����[min,max]����� �������ֵ
		template <typename T>
		T GetRandInteger(T min, T max)
		{
			static uniform_int_distribution<T> dist_int;
			//�趨����
			dist_int.param(uniform_int_distribution<T>::param_type{ min, max });
			return dist_int(g_random);
		}

		//����[min,max)����� ʵ�����ֵ
		template <typename T>
		T GetRandReal(T min, T max)
		{
			static uniform_real_distribution<T> dist_real;
			//�趨����
			dist_real.param(uniform_real_distribution<T>::param_type{ min, max });
			return dist_real(g_random);
		}

		//���� ����mu����׼��sigma ����̬�ֲ����ֵ
		template <typename T>
		T GetRandNormal(T mu, T sigma)
		{
			static normal_distribution<T> dist_normal;
			//�趨�������׼��
			dist_normal.param(normal_distribution<T>::param_type{ mu, sigma });
			return dist_normal(g_random);
		}

		//���� Ȩֵ�� �����������Ϊsize_t���ͣ�
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
		//����ֵ
		unsigned int g_seed;
		//���������������
		default_random_engine g_random;

	};
}

#endif
