#pragma once
#include "array.hpp"
#include "key.hpp"
#include <include_transform.h>

namespace dro {

// ls-dyna_manual_volume_i_r13.pdf p. 2685
class IncludeTransform {
public:
  // Parses an INCLUDE_TRANSFORM keyword
  IncludeTransform(Keyword &kw) noexcept;
  IncludeTransform() noexcept;
  ~IncludeTransform() noexcept;

  // Parse a single card from an INCLUDE_TRANSFORM keyword. Useful when using
  // parse_with_callback
  void parse_include_transform_card(Card card, size_t card_index) noexcept;

  // For an explanation of these variables see <include_transform.h>
  inline String get_file_name() noexcept {
    return String(m_handle.file_name, false);
  }
  inline String get_prefix() noexcept { return String(m_handle.prefix, false); }
  inline String get_suffix() noexcept { return String(m_handle.suffix, false); }
  inline String get_fcttem() noexcept { return String(m_handle.fcttem, false); }

  inline int64_t get_idnoff() const noexcept { return m_handle.idnoff; }
  inline int64_t get_ideoff() const noexcept { return m_handle.ideoff; }
  inline int64_t get_idpoff() const noexcept { return m_handle.idpoff; }
  inline int64_t get_idmoff() const noexcept { return m_handle.idmoff; }
  inline int64_t get_idsoff() const noexcept { return m_handle.idsoff; }
  inline int64_t get_idfoff() const noexcept { return m_handle.idfoff; }
  inline int64_t get_iddoff() const noexcept { return m_handle.iddoff; }
  inline int64_t get_idroff() const noexcept { return m_handle.idroff; }
  inline int64_t get_incout1() const noexcept { return m_handle.incout1; }
  inline int64_t get_tranid() const noexcept { return m_handle.tranid; }

  inline double get_fctmas() const noexcept { return m_handle.fctmas; }
  inline double get_fcttim() const noexcept { return m_handle.fcttim; }
  inline double get_fctlen() const noexcept { return m_handle.fctlen; }

private:
  include_transform_t m_handle;
};

// Holds the name and parameters for every option of a DEFINE_TRANSFORMATION
class TransformationOption {
public:
  TransformationOption(transformation_option_t *handle);

  inline String get_name() noexcept { return String(m_handle->name, false); }
  inline Array<double> get_parameters() noexcept {
    return Array<double>(m_handle->parameters, 7, false);
  }

private:
  transformation_option_t *m_handle;
};

// Holds the components of a DEFINE_TRANSFORMATION keyword
class DefineTransformation {
public:
  // Parses an DEFINE_TRANSFORMATION keyword
  DefineTransformation(Keyword &kw) noexcept;
  DefineTransformation() noexcept;
  ~DefineTransformation() noexcept;

  // Parses a single card form a DEFINE_TRANSFORMATION keyword
  void parse_define_transformation_card(Card card, size_t card_index) noexcept;

  // Transform ID
  inline int64_t get_tranid() const noexcept { return m_handle.tranid; }
  // The Options like TRANSL, SCALE etc. Creates a new array and allocates
  // memory
  Array<TransformationOption> get_options() noexcept;
  // Access the C array directly without copying and allocating any new memory
  inline Array<transformation_option_t> get_raw_options() noexcept {
    return Array<transformation_option_t>(m_handle.options,
                                          m_handle.num_options, false);
  }

private:
  define_transformation_t m_handle;
};

} // namespace dro