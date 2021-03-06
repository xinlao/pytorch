#include <catch.hpp>

#include <torch/jit.h>
#include <torch/tensor.h>

#include <string>

TEST_CASE("torch script") {
  SECTION("multiple functions") {
    auto module = torch::jit::compile(R"JIT(
      def test_mul(a, b):
        return a * b
      def test_relu(a, b):
        return torch.relu(a + b)
      def test_while(a, i):
        while bool(i < 10):
          a += a
          i += 1
        return a
    )JIT");
    auto a = torch::ones(1);
    auto b = torch::ones(1);

    REQUIRE(1 == module->run_method("test_mul", a, b).toTensor().toCLong());

    REQUIRE(2 == module->run_method("test_relu", a, b).toTensor().toCLong());

    REQUIRE(
        0x200 == module->run_method("test_while", a, b).toTensor().toCLong());
  }
}
