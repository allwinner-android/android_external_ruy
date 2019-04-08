#ifndef TENSORFLOW_LITE_EXPERIMENTAL_RUY_SPEC_H_
#define TENSORFLOW_LITE_EXPERIMENTAL_RUY_SPEC_H_

#include <cstdint>
#include <limits>

namespace ruy {

// Our 'general' loop structure (the default) involves multi-threading and
// complicated loops aiming to optimize cache-friendliness. One may opt out of
// this and pick the 'simple' loop structure instead, which only performs well
// for small matrix sizes and only allows using one thread, in exchange for
// smaller code size.
enum class LoopStructure { kGeneral, kSimple, kAuto };

// In general we allow zero_point's to have any Scalar value. This is called
// 'asymmetric' quantization. We do take advantage of the optimization
// opportunities when zero_points happen at runtime to be 'symmetric' (e.g. the
// int8 value 0 or the uint8 value 128), but we still generate code to handle
// the general asymmetric case. By choosing kSymmetric here, one opts out of
// this and supports only the symmetric case, in exchange for smaller code size.
enum class ZeroPointSupport { kGeneral, kSymmetric };

// In general we allow all Layout's, even if we may use slow paths for some
// kinds of layouts. By choosing kPackedLinearRCC, one may opt out of this and
// only keep support for the simplest and most efficient combination of
// Layout's, in exchange for smaller code size. The case covered by
// kPackedLinearRCC is that where all matrix layouts are linear (no sub-block
// structure), packed (no striding), and where the storage orders are exactly
// the following:
//    - LHS is RowMajor
//    - RHS is ColMajor
//    - Destination is ColMajor
enum class LayoutSupport { kGeneral, kPackedLinearRCC };

// A Spec describes all about a matrix multiplication operation that isn't
// encoded in the LHS, RHS and destination matrices. Some of that information
// is encoded as compile-time constants and types (for instance, the choice
// of accumulator type, AccumScalar). Some of that information is encoded as
// runtime values (for instance, the optional bias vector).
template <typename tAccumScalar, typename tDstScalar>
struct BasicSpec {
  // Accumulator type. The type of accumulators used to compute the dot-products
  // before being ultimately casted to the destination type.
  using AccumScalar = tAccumScalar;
  // The destination scalar type.
  using DstScalar = tDstScalar;
  // The bias vector data, if not null.
  const AccumScalar* bias = nullptr;
  // Only for non-floating-point cases. The fixed-point part (i.e. the mantissa)
  // of the multiplier by which accumulators are multiplied before being casted
  // to the destination type.
  AccumScalar multiplier_fixedpoint = 0;
  // Only for non-floating-point cases. The exponent part of the aforementioned
  // multiplier.
  int multiplier_exponent = 0;
  const AccumScalar* multiplier_fixedpoint_perchannel = nullptr;
  const int* multiplier_exponent_perchannel = nullptr;
  // min clamp bound of destination values.
  DstScalar clamp_min = std::numeric_limits<DstScalar>::lowest();
  // max clamp bound of destination values.
  DstScalar clamp_max = std::numeric_limits<DstScalar>::max();
  // See above enum LoopStructure
  static constexpr LoopStructure kLoopStructure = LoopStructure::kAuto;
  // See above enum LayoutSupport
  static constexpr LayoutSupport kLayoutSupport = LayoutSupport::kGeneral;
  // See above enum ZeroPointSupport
  static constexpr ZeroPointSupport kZeroPointSupport =
      ZeroPointSupport::kGeneral;
};

}  // namespace ruy

#endif  // TENSORFLOW_LITE_EXPERIMENTAL_RUY_SPEC_H_
