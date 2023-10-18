#pragma once

namespace GBEmu::HW
{
	struct PPUAddressLY
	{
		uint16 Address;
		uint8 LY;
	};

	class PPUAddressLYDiff
	{
	public:
		void Clear();
		void TrackAddressLY(uint16 address, uint8 ly);

		const Array<PPUAddressLY>& History() const { return m_diffHistory; }

	private:
		Array<PPUAddressLY> m_diffHistory{};
	};
}
