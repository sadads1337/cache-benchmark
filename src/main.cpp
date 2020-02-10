#include <benchmark/benchmark.h>

#include <string>
#include <cstdint>
#include <random>
#include <list>

namespace {

	class Random final {
	public:
		explicit Random(const std::int32_t seed)
				: generator_(seed) {
		}

	public:
		template<typename T>
		[[nodiscard]] T Next(const T &from_inclusive, const T &to_exclusive) {
			return std::uniform_int_distribution<T>(from_inclusive, to_exclusive - 1)(generator_);
		}

		[[nodiscard]] auto generator() const noexcept {
			return generator_;
		}

	private:
		std::mt19937_64 generator_;
	};

} // namespace

template<typename T>
inline auto AccessAllElements(const T *const data, const size_t size) {
	for (auto id = 0u; id < size; ++id) {
		[[maybe_unused]] volatile const auto ignored = data[id];
	}
	return 0;
}

template<typename T>
inline auto InitializeWithRandomNumbers(const std::unique_ptr<T[]> &array, const size_t size) {
	auto random = Random(0);
	std::generate(array.get(), array.get() + size, [&random] {
		return random.Next(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
	});
}

template<typename T>
inline auto MemoryAccessAllElements(benchmark::State &state) {
	constexpr auto seed = 0;
	const auto size = static_cast<size_t>(state.range(seed));
	auto array = std::make_unique<T[]>(size);
	InitializeWithRandomNumbers(array, size);
	for ([[maybe_unused]] const auto &_ : state) {
		benchmark::DoNotOptimize(AccessAllElements(array.get(), size));
	}
}

template<typename T>
inline auto MemoryAccessAllElementsRandom(benchmark::State &state) {
	constexpr auto seed = 0;
	const auto size = static_cast<size_t>(state.range(seed));
	auto list = std::list<T>(size);
	auto random = Random(0);
	std::generate(list.begin(), list.end(), [&random] {
		return random.Next(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
	});
	std::vector<std::reference_wrapper<T>> refs(list.begin(), list.end());
	std::shuffle(refs.begin(), refs.end(), random.generator());
	for ([[maybe_unused]] const auto &_ : state) {
		benchmark::DoNotOptimize(AccessAllElements(refs.data(), size));
	}
}

static auto MemoryAccessAllElementsInt32(benchmark::State &state) {
	MemoryAccessAllElements<std::int32_t>(state);
}

static auto MemoryAccessAllElementsInt64(benchmark::State &state) {
	MemoryAccessAllElements<std::int64_t>(state);
}

static auto MemoryAccessAllElementsRandomInt32(benchmark::State &state) {
	MemoryAccessAllElementsRandom<std::int32_t>(state);
}

static auto MemoryAccessAllElementsRandomInt64(benchmark::State &state) {
	MemoryAccessAllElementsRandom<std::int64_t>(state);
}

static auto CustomizeBenchmark(benchmark::internal::Benchmark *const benchmark) {
	for (auto size = 2ull; size <= (1ull << 24ull); size *= 2) {
		benchmark->Arg(size);
	}
}

BENCHMARK(MemoryAccessAllElementsInt32)->Apply(CustomizeBenchmark);
BENCHMARK(MemoryAccessAllElementsInt64)->Apply(CustomizeBenchmark);

BENCHMARK(MemoryAccessAllElementsRandomInt32)->Apply(CustomizeBenchmark);
BENCHMARK(MemoryAccessAllElementsRandomInt64)->Apply(CustomizeBenchmark);

BENCHMARK_MAIN();
