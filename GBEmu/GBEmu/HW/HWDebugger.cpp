#include "stdafx.h"
#include "HWDebugger.h"

#include "HWEnv.h"
#include "MemoryAddress.h"
#include "GBEmu/Util/Utils.h"

// あるデバッグしたいときは、以下のどれかをコメントイン
// #define IMPL_FOUNDINSTRUCTIONDISTRIBUTION
// #define IMPL_EXECUTEDINSTRUCTIONLOG
// #define IMPL_WROTEMEMORYLOG
// #define IMPL_TRACE
// #define IMPL_STATISTICSPC

// マクロ依存関係
#ifdef IMPL_WROTEMEMORYLOG
#define IMPL_EXECUTEDINSTRUCTIONLOG
#endif
#ifdef IMPL_STATISTICSPC
#define IMPL_FOUNDINSTRUCTIONDISTRIBUTION
#endif


namespace GBEmu::HW
{
	using namespace MemoryAddress;

	constexpr int size_5 = 5;
	constexpr int size_10 = 10;
	constexpr int size_20 = 20;
	constexpr int size_50 = 50;
	constexpr int size_100 = 100;
	constexpr int size_200 = 200;
	constexpr int size_500 = 500;

#ifdef IMPL_EXECUTEDINSTRUCTIONLOG
  	struct HWDebugExecutedInstruction
	{
		uint16 CurrentPC;
		String NextInstruction;
	};
#endif

#ifdef IMPL_WROTEMEMORYLOG
	struct HWDebugWroteMemory
	{
		uint16 Address{};
		uint8 Data{};
		HWDebugExecutedInstruction PreviousInstr;
	};
#endif

	HWDebugger::ImplPtr::~ImplPtr() = default;

	class HWDebugger::Impl
	{
#ifdef IMPL_FOUNDINSTRUCTIONDISTRIBUTION
		std::array<unsigned short, 256> m_foundInstructionDistribution{};
		std::array<unsigned short, 256> m_foundInstructionCBDistribution{};
#endif
#ifdef IMPL_EXECUTEDINSTRUCTIONLOG
		std::deque<HWDebugExecutedInstruction> m_executedInstructionLog{};
#endif
#ifdef IMPL_WROTEMEMORYLOG
		std::deque<HWDebugWroteMemory> m_wroteMemoryLog{};
#endif
#ifdef IMPL_STATISTICSPC
  		Optional<uint16> m_statisticsPC{};
#endif
#ifdef IMPL_TRACE
  		int m_traceCountdown{};
		HashSet<std::string> m_tracedKey{};
#endif

#ifdef IMPL_TRACE
		void debugTrace(HWEnv& env)
		{
			Console.writeln( U"CPU: { " + env.GetCPU().StringifyInfo(env.GetMemory()) + U" }\n");
		}

		Optional<std::pair<std::string, int>> checkStartTrace(HWEnv& env) const
		{
  			auto&& cpu = env.GetCPU();
			auto&& memory = env.GetMemory();
			using pair = std::pair<std::string, int>;

			// トレース開始チェック

			// 特定のPC付近に達したら
			// if (Math::Abs(cpu.PC() - 0xC370) <= size_20 &&
			// m_tracedKey.contains("PC") == false) return pair{"PC", size_500};

			// 命令に達したら
			// if (cpu.FetchInstruction(memory).CodeUnprefixed() == CPUInstruction::LD_SP_d16_0x31 &&
			// 	m_tracedKey.contains("INSTR") == false) return pair{"INSTR", size_50};

			// EIに達したら
			// if (cpu.FetchInstruction(memory).CodeUnprefixed() == CPUInstruction::EI_0xFB &&
			// 	m_tracedKey.contains("EI") == false) return pair{"EI", size_500};

			// TimerのIEがONになったら
			// if (memory.Read(IE_0xFFFF) & (1 << 2) &&
			// 	m_tracedKey.contains("IE Timer") == false) return pair{"IE Timer", size_5};

			// HALTになったら
			// if (cpu.State() == CPUState::Halted &&
			// 	m_tracedKey.contains("HALT") == false) return pair{"HALT", size_5};]

			return none;
		}
#endif

#ifdef IMPL_FOUNDINSTRUCTIONDISTRIBUTION
		void printFoundInstructionDistribution() const
		{
			String str = U"";
			for (int i=0; i<256; ++i)
			{
				str += U"{}: {}\n"_fmt(Util::StringifyEnum(static_cast<CPUInstruction>(i)), m_foundInstructionDistribution[i]);
			}
			for (int i=0; i<256; ++i)
			{
				str += U"{}: {}\n"_fmt(Util::StringifyEnum(static_cast<CPUInstructionCB>(i)), m_foundInstructionCBDistribution[i]);
			}
			Console.writeln(str);
		}
#endif

#ifdef IMPL_WROTEMEMORYLOG
		void printWroteMemoryLog()
		{
  			// メモリ書き込み経歴を出力
			for (int i=0; i<m_wroteMemoryLog.size(); ++i)
			{
				auto&& log = m_wroteMemoryLog[i];
				Console.writeln(U"[{}] ({:04X}) <- {:02X} (PC: {:04X}, instr: {})"_fmt(
					i, log.Address, log.Data, log.PreviousInstr.CurrentPC, log.PreviousInstr.NextInstruction));
			}
		}
#endif

#ifdef IMPL_STATISTICSPC
  		static void printSearchedMemoryBlob(HWEnv& env, const Array<uint16>& blob)
		{
			for (uint16 addr = 0xFFFF;;)
			{
				auto searched = SearchMemoryBlob(env, env.GetMemory(), RangeUint16(addr + 1, 0xFFFF), blob);
				if (searched.has_value() == false) break;
				addr = searched.value();
				Console.writeln(U"{:04X}"_fmt(addr));
			}
		}

		void publishStatistics(HWEnv& env) const
		{
#ifdef IMPL_FOUNDINSTRUCTIONDISTRIBUTION
  			// 命令の実行分布
			printFoundInstructionDistribution();
#endif
			// 特定のメモリ列を探索
			// printSearchedMemoryBlob(env,
			// 	{0xF0, 0x91, 0x00, 0xE0, 0x91, 0x00, 0xF2, 0x00, 0x00});
		}
#endif
	public:
		Impl()
		{
#ifdef IMPL_EXECUTEDINSTRUCTIONLOG
			m_executedInstructionLog.resize(size_50);
#endif

#ifdef IMPL_STATISTICSPC
  			// 特定のPCになったら統計出す
			m_statisticsPC = 0xCE67;
#endif
		}

		void UpdateCycle(HWEnv& env)
		{
#ifdef IMPL_TRACE
			// トレースチェック
			const auto startedTrace = checkStartTrace(env);
			if (startedTrace.has_value())
			{
				// トレース開始
				m_tracedKey.emplace(startedTrace.value().first);
				m_traceCountdown = startedTrace.value().second;
			}

			// トレース中
			if (m_traceCountdown > 0)
			{
				m_traceCountdown--;
				debugTrace(env);
			}
#endif

#ifdef IMPL_STATISTICSPC
  			// 特定PCで統計表示
			if (env.GetCPU().PC() == m_statisticsPC)
			{
				publishStatistics(env);
				m_statisticsPC = none;
			}
#endif
		}

		void OnExecuteInstruction(const CPU& cpu, const CPUInstructionProperty& fetchedInstruction)
		{
#ifdef IMPL_EXECUTEDINSTRUCTIONLOG
  			// 命令実行経歴を残す
			m_executedInstructionLog.push_front(HWDebugExecutedInstruction{
				cpu.PC(),
				fetchedInstruction.ToString()
			});
			m_executedInstructionLog.pop_back();
#endif

#ifdef IMPL_FOUNDINSTRUCTIONDISTRIBUTION
  			// 命令出現分布を記憶
			if (fetchedInstruction.IsPrefixedCB)
				m_foundInstructionCBDistribution[fetchedInstruction.Code]++;
			else
				m_foundInstructionDistribution[fetchedInstruction.Code]++;
#endif
		}

		void OnMemoryWrite(uint16 address, uint8 data)
		{
#ifdef IMPL_WROTEMEMORYLOG
  			// メモリ書き込み経歴を残す (ダイレクトな書き込み経歴は取得できない)
			m_wroteMemoryLog.push_front(
				HWDebugWroteMemory{address, data, m_executedInstructionLog[0]});
			constexpr int wroteMemoryLogSize = size_500;
			if (m_wroteMemoryLog.size() > wroteMemoryLogSize) m_wroteMemoryLog.pop_back();
#endif
		}
	};

	HWDebugger::HWDebugger() :
		m_impl {std::make_unique<Impl>()}
	{
		for (int i=1; i<=4; ++i)
		{
			m_audioSampleBuffer[i].reserve(HWParam::AudioSampleRate / HWParam::FPS);
		}
	}

	HWDebugger::~HWDebugger() = default;

	void HWDebugger::UpdateFrame(HWEnv& env)
	{
		// フレーム間CPU稼働率のリセット
		m_cpuWorkedCycleInFrame = 0;

		// デバッグ用休止状態に
		if (KeyC.down() && KeyControl.pressed())
		{
			m_isDebugSuspend = !m_isDebugSuspend;
		}

		// オーディオを可視化
		refreshAudioGraph(m_audioGraph, m_audioSampleBuffer);
	}

	void HWDebugger::refreshAudioGraph(
		RenderTexture& graph, std::array<std::vector<float>, audioChannelCapacity>& samples)
	{
		// 描画設定
		graph.clear(ColorF{ 1.0, 0.0 });
		const ScopedRenderTarget2D target{ graph };
		const ScopedRenderStates2D blend{ BlendState::Opaque };

		constexpr std::array palette{
			Color(U"#FFFFFF"), Color(U"#FF40FF"), Color(U"#FFFF00"), Color(U"#00FFFF")};

		// すべてのサンプルごとに線を描画すると、処理が重いのである程度標本化してから描画する
		constexpr int fragSize = 32;
		const float fragW = graph.size().x / fragSize;

		// 波形を4チャンネルごとに描画
		for (int ch=1; ch<=4; ++ch)
		{
			auto&& waves = samples[ch];
			const float density = waves.size() / static_cast<float>(fragSize); // 1点に標本化する波形の個数
			float count = 0; // 現在集計中の標本数
			float sum = 0; // 現在集計中の標本合計和
			float cursorX = 0;
			float oldPlotY = 0;
			// そのチャンネルのサンプルを探索
			for (int i=0; i<waves.size(); ++i)
			{
				count++;
				sum += waves[i];
				if (count < density) continue;

				// 集計終わり
				const float newPlotY = graph.size().y * (0.5f + 0.5f * sum / count);
				sum = 0;
				count -= density;

				if (cursorX > 0)
					(void)Line(cursorX, oldPlotY, cursorX + fragW, newPlotY).draw(1.5, palette[ch - 1]);

				oldPlotY = newPlotY;
				cursorX += fragW;
			}

			// サンプルデータをクリア
			samples[ch].clear();
		}
	}

	void HWDebugger::UpdateCycle(HWEnv& env, CPUCycle cycle)
	{
		if (env.GetCPU().State() == CPUState::Running) m_cpuWorkedCycleInFrame += cycle.Count;

		m_impl->UpdateCycle(env);
	}

	void HWDebugger::OnExecuteInstruction(const CPU& cpu, const CPUInstructionProperty& fetchedInstruction)
	{
		m_lastExecutedInstruction = fetchedInstruction;

		m_impl->OnExecuteInstruction(cpu, fetchedInstruction);
	}

	void HWDebugger::OnMemoryWrite(uint16 address, uint8 data)
	{
		m_impl->OnMemoryWrite(address, data);
	}

	void HWDebugger::OnAudioSample(int channel, float wave)
	{
		m_audioSampleBuffer[channel].push_back(wave);
	}

	// データ列を受け取って、そのメモリ列が存在するならアドレスを返す
	Optional<uint16> HWDebugger::SearchMemoryBlob(HWEnv& env, Memory& memory, const RangeUint16& range, const Array<uint16>& blob)
	{
		for (uint16 i=range.Min(); i<= range.Max() - blob.size() + 1; ++i)
		{
			if (memory.Read(env, i) != blob[0]) continue;

			bool isMatch = true;
			for (int x = 1; x<blob.size(); ++x)
			{
				if (memory.Read(env, i + x) == blob[x]) continue;
				isMatch = false;
				break;
			}
			if (isMatch) return i;
		}
		return none;
	}
}
