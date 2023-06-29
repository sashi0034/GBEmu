#pragma once

namespace GBEmu::Util
{
	template <typename T>
	class Range
	{
		static_assert(std::is_arithmetic_v<T>, "template argument must be an arithmetic type");
	private:
		const T m_Min;
		const T m_Max;
	public:
		constexpr Range(T minInclusive, T maxInclusive) :
			m_Min(minInclusive), m_Max(maxInclusive)
		{}

		static Range<T> WithAssert(T minInclusive, T maxInclusive)
		{
			assert(minInclusive <= maxInclusive);
			return Range<T>(minInclusive, maxInclusive);
		}

		static Range<T> WithSort(T value1, T value2)
		{
			if (value1 > value2) return Range<T>(value2, value1);
			return Range<T>(value1, value2);
		}

		T Max() const {return m_Max; }
		T Min() const {return m_Min; }

		constexpr Range<T> ExtendRange(T extension)
		{
			if (extension < 0) return WithSort(m_Min - extension, m_Max + extension);
			return Range(m_Min - extension, m_Max + extension);
		}

		constexpr T GetDiff() const{
			return m_Max - m_Min;
		}

		constexpr bool IsBetween(T targetValue) const
		{
			return m_Min <= targetValue && targetValue <= m_Max;
		}

		constexpr T Clamp(T targetValue) const
		{
			return (std::max)(m_Min, (std::min)(targetValue, m_Max));
		}

		T Normalize(T targetValue, const Range<T>&afterRange)
		{
			T normalizedRate = (targetValue - this->m_Min) / (this->m_Max - this->m_Min);
			T result = afterRange.m_Min + (afterRange.m_Max - afterRange.m_Min) * normalizedRate;
			return result;
		}
	};

	using RangeInt = Range<int>;
	using RangeDouble = Range<double>;
	using RangeF = Range<float>;
	using RangeUint8 = Range<uint8>;
	using RangeUint16 = Range<uint16>;
}

using GBEmu::Util::RangeUint8;
using GBEmu::Util::RangeUint16;
