#pragma once
#include "APUStream.h"
#include "AudioChNoise.h"
#include "AudioChSquare.h"
#include "AudioChWave.h"
#include "AudioFrameSequencer.h"
#include "MemoryAddress.h"
#include "GBEmu/Util/Utils.h"

namespace GBEmu::HW
{
	class HWDebugger;
	class HWEnv;

	// 参考: https://github.com/voidproc/dmge/tree/main/dmge/APU
	class APU
	{
	public:
		APU();

		void UpdateFrame(HWEnv& env) const;
		void StepCycle(HWEnv& env);

		template <uint16 addr>
		uint8 ReadAddr() const;
		template <uint16 addr>
		void WriteAddr(uint8 data);

		uint8 ReadWaveRam(uint16 addr) const { return m_ch3.ReadWaveRam(addr); }
		void WriteWaveRam(uint16 addr, uint8 data) { m_ch3.WriteWaveRam(addr, data); }

	private:
		std::shared_ptr<APUStream> m_stream{};
		Audio m_audio{};

		AudioFrameSequencer m_frameSequencer{};
		AudioChSquare<1> m_ch1{};
		AudioChSquare<2> m_ch2{};
		AudioChWave m_ch3{};
		AudioChNoise m_ch4{};

		uint8 m_channelControl{}; // NR50
		uint8 m_selection{}; // NR51
		bool m_powerControl{}; // NR52 (bit 7)

		float m_outputTimer{};

		void pushSample(HWDebugger& debugger) const;
		template <int... Args>
		void writeFor(uint8 data);
	};

	template <uint16 addr>
	uint8 APU::ReadAddr() const
	{
		using namespace MemoryAddress;

		if constexpr (addr == NR10_0xFF10) return m_ch1.ReadNR<0>();
		else if constexpr (addr == NR11_0xFF11) return m_ch1.ReadNR<1>();
		else if constexpr (addr == NR12_0xFF12) return m_ch1.ReadNR<2>();
		else if constexpr (addr == NR13_0xFF13) return m_ch1.ReadNR<3>();
		else if constexpr (addr == NR14_0xFF14) return m_ch1.ReadNR<4>();
		else if constexpr (addr == NR21_0xFF16) return m_ch2.ReadNR<1>();
		else if constexpr (addr == NR22_0xFF17) return m_ch2.ReadNR<2>();
		else if constexpr (addr == NR23_0xFF18) return m_ch2.ReadNR<3>();
		else if constexpr (addr == NR24_0xFF19) return m_ch2.ReadNR<4>();
		else if constexpr (addr == NR30_0xFF1A) return m_ch3.ReadNR3x<0>();
		else if constexpr (addr == NR31_0xFF1B) return m_ch3.ReadNR3x<1>();
		else if constexpr (addr == NR32_0xFF1C) return m_ch3.ReadNR3x<2>();
		else if constexpr (addr == NR33_0xFF1D) return m_ch3.ReadNR3x<3>();
		else if constexpr (addr == NR34_0xFF1E) return m_ch3.ReadNR3x<4>();
		else if constexpr (addr == NR41_0xFF20) return m_ch4.ReadNR4x<1>();
		else if constexpr (addr == NR42_0xFF21) return m_ch4.ReadNR4x<2>();
		else if constexpr (addr == NR43_0xFF22) return m_ch4.ReadNR4x<3>();
		else if constexpr (addr == NR44_0xFF23) return m_ch4.ReadNR4x<4>();
		else if constexpr (addr == NR50_0xFF24) return m_channelControl;
		else if constexpr (addr == NR51_0xFF25) return m_selection;
		else if constexpr (addr == NR52_0xFF26)
		{
			return
				(m_ch1.ChannelEnabled() << 0) | (m_ch2.ChannelEnabled() << 1) |
				(m_ch3.ChannelEnabled() << 2) | (m_ch3.ChannelEnabled() << 3) |
				(m_powerControl << 7);
		}
		else
		{
			static_assert(Util::AlwaysFalseValue<addr>);
			return {};
		}
	}

	template <uint16 addr>
	void APU::WriteAddr(uint8 data)
	{
		using namespace MemoryAddress;

		if constexpr (addr == NR10_0xFF10) m_ch1.WriteNR<0>(data);
		else if constexpr (addr == NR11_0xFF11) m_ch1.WriteNR<1>(data);
		else if constexpr (addr == NR12_0xFF12) m_ch1.WriteNR<2>(data);
		else if constexpr (addr == NR13_0xFF13) m_ch1.WriteNR<3>(data);
		else if constexpr (addr == NR14_0xFF14) m_ch1.WriteNR<4>(data);
		else if constexpr (addr == NR21_0xFF16) m_ch2.WriteNR<1>(data);
		else if constexpr (addr == NR22_0xFF17) m_ch2.WriteNR<2>(data);
		else if constexpr (addr == NR23_0xFF18) m_ch2.WriteNR<3>(data);
		else if constexpr (addr == NR24_0xFF19) m_ch2.WriteNR<4>(data);
		else if constexpr (addr == NR30_0xFF1A) m_ch3.WriteNR3x<0>(data);
		else if constexpr (addr == NR31_0xFF1B) m_ch3.WriteNR3x<1>(data);
		else if constexpr (addr == NR32_0xFF1C) m_ch3.WriteNR3x<2>(data);
		else if constexpr (addr == NR33_0xFF1D) m_ch3.WriteNR3x<3>(data);
		else if constexpr (addr == NR34_0xFF1E) m_ch3.WriteNR3x<4>(data);
		else if constexpr (addr == NR41_0xFF20) m_ch4.WriteNR4x<1>(data);
		else if constexpr (addr == NR42_0xFF21) m_ch4.WriteNR4x<2>(data);
		else if constexpr (addr == NR43_0xFF22) m_ch4.WriteNR4x<3>(data);
		else if constexpr (addr == NR44_0xFF23) m_ch4.WriteNR4x<4>(data);
		else if constexpr (addr == NR50_0xFF24) m_channelControl = data;
		else if constexpr (addr == NR51_0xFF25) m_selection = data;
		else if constexpr (addr == NR52_0xFF26)
		{
			m_powerControl = data >> 7;
			if (m_powerControl) return;
			// 7ビット目に0が書き込まれたとき、レジスタすべてに0を書き込む
			writeFor<
				NR10_0xFF10, NR11_0xFF11, NR12_0xFF12, NR13_0xFF13, NR14_0xFF14,
				NR21_0xFF16, NR22_0xFF17, NR23_0xFF18, NR24_0xFF19,
				NR30_0xFF1A, NR31_0xFF1B, NR32_0xFF1C, NR33_0xFF1D, NR34_0xFF1E,
				NR41_0xFF20, NR42_0xFF21, NR43_0xFF22, NR44_0xFF23, NR50_0xFF24,
				NR51_0xFF25>(0);
		}
		else static_assert(Util::AlwaysFalseValue<addr>);
	}

	template <int... Args>
	void APU::writeFor(uint8 data)
	{
		(WriteAddr<Args>(data), ...);
	}
}
