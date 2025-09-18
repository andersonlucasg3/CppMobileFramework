#pragma once

#include "Defines/Types.h"

using namespace Core::Types;

namespace UI::Graphics
{
	struct SColorF
	{
		SColorF() = default;

		SColorF(Float R, Float G, Float B, Float A = 1)
		{
			Vector[0] = R;
			Vector[1] = G;
			Vector[2] = B;
			Vector[3] = A;
		}

		SColorF(const Float(&InComponents)[4])
		{
			for (UInt64 Index = 0; Index < 4; Index++)
			{
				Vector[Index] = InComponents[Index];
			}
		}

		SColorF(const SColorF& Other)
		{
			for (UInt64 Index = 0; Index < 4; Index++)
			{
				Vector[Index] = Other.Vector[Index];
			}
		}

		SColorF& operator=(const SColorF& Other)
		{
			for (UInt64 Index = 0; Index < 4; Index++)
			{
				Vector[Index] = Other.Vector[Index];
			}

			return *this;
		}

		Float& R()
		{
			return Vector[0];
		}

		Float R() const
		{
			return Vector[0];
		}

		Float& G()
		{
			return Vector[1];
		}

		Float G() const
		{
			return Vector[1];
		}

		Float& B()
		{
			return Vector[2];
		}

		Float B() const
		{
			return Vector[2];
		}

		Float& A()
		{
			return Vector[3];
		}

		Float A() const
		{
			return Vector[3];
		}

		Float& operator[](UInt64 Index)
		{
			return Vector[Index];
		}

		const Float& operator[](UInt64 Index) const
		{
			return Vector[Index];
		}

		bool operator==(const SColorF& Other) const
		{
			for (UInt64 Index = 0; Index < 4; Index++)
			{
				if (Vector[Index] != Other.Vector[Index])
				{
					return false;
				}
			}

			return true;
		}

		bool operator !=(const SColorF& Other) const
		{
			return !(*this == Other);
		}

		static const SColorF& White()
		{
			static const SColorF White = { 1.f, 1.f, 1.f, 1.f };
			return White;
		}

		static const SColorF& Black()
		{
			static const SColorF Black = { 0.f, 0.f, 0.f, 1.f };
			return Black;
		}
		
		static const SColorF& Gray()
		{
			static const SColorF Gray = { .5f, .5f, .5f, 1.f };
			return Gray;
		}

		static const SColorF& Red()
		{
			static const SColorF Red = { 1.f, 0.f, 0.f, 1.f };
			return Red;
		}
		
		static const SColorF& Green()
		{
			static const SColorF Green = { 0.f, 1.f, 0.f, 1.f };
			return Green;
		}
		
		static const SColorF& Blue()
		{
			static const SColorF Blue = { 0.f, 0.f, 1.f, 1.f }; 
			return Blue;
		}	

		static const SColorF& Yellow()
		{
			static const SColorF Yellow = { 1.f, 1.f, 0.f, 1.f };
			return Yellow;
		}
		
		static const SColorF& Magenta()
		{
			static const SColorF Magenta = { 1.f, 0.f, 1.f, 1.f };
			return Magenta;
		}
		
		static const SColorF& Cyan()
		{
			static const SColorF Cyan = { 0.f, 1.f, 1.f, 1.f };
			return Cyan;
		}

		static const SColorF& Transparent()
		{
			static const SColorF Transparent = { 0, 0, 0, 0 };
			return Transparent;
		}

	protected:
		Float Vector[4];
	};
}
